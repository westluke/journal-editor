#include "Paragraph.hpp"

// #define NDEBUG
#include <cassert>

// I'm actually ready to start building the visual interface, very simply at first of course.
// I've got to think about how that works. Something has to keep track of each paragraph and where the cursor is, so it can update the paragraphs properly.
// And something has to be displaying stuff to the screen. Forget the display optimizations for now, display everything every time.
// So, let's figure out how to display selected paragraphs to the screen. Also, how do I handle resize events again? I think I just handle them like normal keypresses.
// Another thing: make this thing permit an initializer list interface to make testing easier.
//
// How do I make displaying easy? I want to build this up slowly. Let's actually start with displaying lines. That way I don't have to test this paragraph yet until
// it's easier to do. I could actually implement character insertion using lines only. 
//
// I think the display program should be an instance of a class, since I could have several windows. Makes sense to be initialized with the particular window.
// And then, we simply have a public method that takes a Line and a number, and prints out that line on that line number. done.
//
//
//

// JUST REALIZED SOMETHING. Right now, I'm relying on these paragraphs being in a perfect state every time, which might work well in production.
// But, there's a slightly slower, possibly much saner method. Insert characters normally, then QUICKLY ensure correctness of entire paragraph every time.
// It's something that could probably be easily optimized out if it proves to slow the program down, but makes coding this part extremely easy.
// let's try it.
// this function also returns a bool if it took any weird action.
// So how does it work?
// One single pass.
// After the first line, removes empty lines.
// How does it deal with overflows? A line overflow is illegal, but so is the potential of flowback.
//
// Another idea: using overflow and accept_flowback is a little silly since they accomplish basically the same
// thing: equalizing between two lines. and they occur in the same situations. Is there any reason not to? Could this be made more efficient or cleaner if I didn't?
// Possibly, if I optimize.
// But this makes everything easier to conceptualize, so this is what I will do.


/* Invalid states:
 * If there is an empty line and more than one line
 * If any line exceeds with non-whitespace
 *
 * If any line can flow back into the previous. Actually includes the following two:
 * If any line contains beginning whitespace, besides the first line.
 * If any line is entirely whitespace
 */

/* Given a paragraph, how might I pass through and ensure validity?
 * First relieve excess on all lines. Now no lines exceed width.
 * Now delete any empty lines.
 * Now flowback all lines. But now we have an issue. Consider a paragraph like this:
 *
 * Words		|
 * more words		|
 * more			|
 * Alonglinethatwontflow|
 *
 * Relieving excess and deleting empty lines does nothing. After flowing back, we get this:
 *
 * Wordsmore words	|
 * more			|
 * Alonglinethatwontflow|
 * 			|
 *
 * Now more is just stranded there, even though it could flow back. Solution? Delete lines that become empty.
 * And we can't do this validation every time. That would take forever. Have to have each function accept a valid state, and guaranteed to produce a valid state.
 * In the meantime, I can do assert checks for valid state.
 *
 */

bool Paragraph::valid(){
	for (int i = 0; i < lines.size(); i++){
		if (lines[i].line_length() == 0 && i != 0) return false;
		if (lines[i].exceeds_width_non_whitespace(line_width)) return false;
		if (i > 0 && lines[i-1].equalize(lines[i], line_width)) return false;
	}
	return true;
}

bool Paragraph::distribute(){
	bool changed = false;

	// Go through each line. If a line is empty, remove it.
	// Otherwise, equalize that line with the following line.
	for (int i = 0; i < lines.size(); i++){
		// Remove a line if its empty, but not if that line is the only line.
		if (lines[i].line_length() == 0 && lines.size() > 1){
			lines.erase(lines.begin() + i);
			i--;
			changed = true;
			continue;
		}

		if (i < lines.size() - 1){
			changed = changed || lines[i].equalize(lines[i+1], line_width);
		}
	}

	// Now, the last line could have too much text. Keep equalizing and adding lines until we have enough space.
	while (lines[lines.size()-1].exceeds_width_non_whitespace(line_width)){
		lines.push_back(Line());
		lines[lines.size()-2].equalize(lines[lines.size()-1], line_width);
		changed = true;
	}

	assert((valid()));
	return changed;
}

void Paragraph::insert_ch(p_index i, int ch){
	assert((valid()));

	fchar fch = fchar(ch);

	// Verify that the p_index is valid.
	assert((i.line_no < lines.size() && i.ch_index <= lines[i.line_no].line_length()));
	
	if (lines.at(0).line_length() == 0)
		fch.style = initial_style;
	else if (!(i.line_no == 0 && i.ch_index == 0))
		fch.style = get_ch(previous_index(i)).style;
	else
		fch.style = get_ch(next_index(i)).style;

	lines.at(i.line_no).insert_ch(i.ch_index, fch);
	distribute();
	assert((valid()));
}

fchar Paragraph::delete_ch(p_index i){
	assert((valid()));
	assert((i.line_no < lines.size() && i.ch_index < lines[i.line_no].line_length()));

	fchar ch = lines.at(i.line_no).delete_ch(i.ch_index);
	distribute();

	assert((valid()));
	return ch;
}

fchar Paragraph::get_ch(p_index i){
	return lines.at(i.line_no).get_ch(i.ch_index);
}

// This function doesn't check that the p_index is valid.
// If the index is {0, 0}, the function returns {0, 0}.
// Can later change this to use bracket operators instead of .at()
p_index Paragraph::previous_index(p_index i){
	assert(i.line_no < lines.size());
	assert(i.ch_index < lines[i.line_no].line_length());
	assert(!(i.line_no == 0 && i.ch_index == 0));

	if (i.ch_index){
		i.ch_index--;
	} else if (i.line_no){
		i.line_no--;
		i.ch_index = lines.at(i.line_no).line_length() - 1;
	} 

	// If i points to the first character (and there is no previous index) it gets returned unchanged.
	// But ideally this shouldn't happen, which is why we have an assert that checks for it.
	return i;
}

p_index Paragraph::next_index(p_index i){
	assert(i.line_no < lines.size());
	assert(i.ch_index < lines[i.line_no].line_length());

	i.ch_index++;
	return i;
}

//bool apply_format(p_index start, p_index end, Format f);

std::vector<Line> Paragraph::get_lines(){
	return lines;
}

void Paragraph::set_line_width(Line::index_type lw){
	assert(lw != 0);
	line_width = lw;
}

//void set_header_level(HeaderLevel hl);

Paragraph::Paragraph(Line::index_type lw): line_width(lw){
	;
}

Paragraph::Paragraph(std::initializer_list<Line> il, Line::index_type lw): line_width(lw), lines(il){
	;
}

Paragraph::Paragraph(std::initializer_list<char*> il, Line::index_type lw): line_width(lw){
	for (auto iter = il.begin(); iter != il.end(); iter++){
		lines.push_back(Line(*iter));
	}
}
