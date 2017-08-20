#include "Paragraph.hpp"

// #define NDEBUG
#include <cassert>


// JUST REALIZED SOMETHING. Right now, I'm relying on these paragraphs being in a perfect state every time, which might work well in production.
// But, there's a slightly slower, possibly much saner method. Insert characters normally, then QUICKLY ensure correctness of entire paragraph every time.
// It's something that could probably be easily optimized out if it proves to slow the program down, but makes coding this part extremely easy.
// let's try it.


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
		if (lines[i].size() == 0 && i != 0) return false;
		if (lines[i].exceeds_width_non_whitespace(line_width)) return false;
		if (i > 0 && lines[i-1].accept_flowback(lines[i], line_width)) return false;
	}
	return true;
}

void Paragraph::insert_ch(p_index i, int ch){
	assert((valid()));
	fchar fch;
	fch.character = ch;

	// Verify that the p_index is valid.
	assert((i.line_no < lines.size() && i.ch_index <= lines[i.line_no].line_length()));
	
	if (lines.at(0).line_length() == 0){
		assert(lines.size() == 1);
		fch.style = initial_style;
	} else if (!(i.line_no == 0 && i.ch_index == 0)) {
		fchar prev_char = get_ch(previous_index(i));
		fch.style = prev_char.style;
	} else {
		fchar next_char = get_ch(next_index(i));
		fch.style = next_char.style;
	}

	lines.at(i.line_no).insert_ch(i.ch_index, fch);

	// Wait no I should stop relieving excess as soon as the relief fails.
	int i = i.line_no;
	for (; i < lines.size() - 1; i++){
		// If a relief fails, we can return immediately.
		if (!(lines[i].relieve_excess(lines[i+1], line_width)))
			return;
	}

	while (lines[i].exceeds_width_non_whitespace(line_width)){
		lines.push_back(Line());
		lines[i].relieve_excess(lines[i+1], line_width);
		i++;
	}
	assert((valid()));
}

char Paragraph::delete_ch(p_index i){
	assert((i.line_no < lines.size() && i.ch_index < lines[i.line_no].line_length()));

	lines.at(i.line_no).delete_ch(i.ch_index);

	if (i.line_no){
		lines.at(i.line_no - 1).accept_flowback(lines.at(line_no));
	}

	int ind = i.line_no + 1;
	while (ind < lines.size() && lines.at(ind - 1).accept_flowback(ind)) {
		ind++;
	}

	if (ind >= lines.size()){
		// flowed back from the last line.
		// but wait what happens if there are multiple empty lines at the end? Is that possible?
		// Also, wait, what if you flow back to a line, and then that line is eligible to flow back to the previous? Is that possible?
		// I think so, but only if was already in an invalid state. I should define what the state of this thing must be, and what invalid is,
		// and how to make sure its always valid.
		// Maybe I should set up those test cases.
	}


	// When i delete a character, its possible I deleted it from a word at the beginning of a line, which means its possible the previous
	// line can now accept flowback. I should try it. Thats the only previous line that must be considered. AFTER I DO THAT,
	// I must check if the current line can accept flowback, and move forwards from there until lines stop accepting flowback.
	// It's also now possible that lines at the front could become empty, in which case they must be deleted.
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
	return i;
}

p_index Paragraph::next_index(p_index i){
	assert(i.line_no < lines.size());
	assert(i.ch_index < lines[i.line_no].line_length());

	i.ch_index++;
	return i;
}

bool apply_format(p_index start, p_index end, Format f);

std::vector<Line::text_type> get_lines();

void set_line_width(Line::index_type lw);

void set_header_level(HeaderLevel hl);


Paragraph::Paragraph(Line::index_type lw): line_width(lw){
	;
}
