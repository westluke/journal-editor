#include "Paragraph.hpp"

// #define NDEBUG
#include <cassert>

// size types are always unsigned.
// where should these things be converted to fchars?
// At the paragraph level, I think. an owning class won't know what format to apply to things, but a paragraph will.
//
// How should character insertion work? It's a little complicated. 
// I could enter an insertion mode, where I don't have to do calculations every time.
// I could do pure insertions alone.
// I could give an option to specify format, or format separately. There isn't a huge difference.
// When I insert the first character in an insertion mode, I still have to find the previous character, right?
// Yes, but ah, here's an issue. Say we are at the end a paragraph, and I apply a new format to take hold on future characters I insert.
// That can't be stored in some preceding character, can it.
// So, instead, it must be stored in a separate variable, that determines what format characters will have appended to the end of the paragraph.
//
// Ok, that solves that. How about inserting characters? How do we find the last index? just make a function for it for now.
//
// Can the default format change over the course of a paragraph? maybe. Yes. If the first character ever changes format, that's the new default format.
// It's initialized to a default format, but then it becomes whatever the first character's format is.
// And of course, what it gets initialized to is dependent on which paragraph its a child of.
//
// Should there be a unified way to update characters? Idk what that would be.
// I don't think so. Wait until that appears to make sense.
void Paragraph::insert_ch(p_index i, int ch){
	fchar fch;
	fch.character = ch;

	// Verify that the p_index is valid.
	assert((i.line_no < lines.size() && i.ch_index <= lines[i.line_no].line_length()));
	
	// how should I determine whether its the first character to be entered?
	// I could ask whether the first Line is empty, because there should never be more than one Line if the first Line is empty.
	// I could also first check that the number of Lines is one, and then that the first Line is empty. But that's almost equivalent.
	// Let's do the first, and just remember that its a possible point of breakage if I fuck up and it becomes possible to have multiple empty lines.
	// Paragraph should never have zero lines, so the .at(0) will catch that error.
	
	// If the character is the first character entered into the paragraph, use the initial format of the paragraph.
	if (lines.at(0).line_length() == 0){
		assert(lines.size() == 1);
		fch.style = initial_style;
	}

	// If it's not the first character, use the format of the preceding character.
	else if (!(i.line_no == 0 && i.ch_index == 0)) {
		fchar prev_char = get_ch(previous_index(i));
		fch.style = prev_char.style;
	}

	// If it is the first character, use the format of the following character.
	else {
		fchar next_char = get_ch(next_index(i));
		fch.style = next_char.style;
		// use following format
	}
		// set default format. But that just means that this is the very first character, so there's a better way to do this.
		// Actually, is there? It can be the first character, even when there are other characters in the line.
		// That implies I need two default formats for characters, one for the end and one for the beginning. check drive.
		// actually google drive doesn't do that. When inserting at the beginning, it just takes the format of the next character, UNLESS its also the first character to be entered in the paragraph
		// What if its between two paragraphs?
		// That depends on which paragraph creates the space. interesting. Should I mimic that behavior? Yeah I probalbly should. That's the only way to
		// make it easy to extend formatting into a paragraph above. Still, we have the same structure, with a format option maintained that applies to appending to the paragraph. Anywhere else, we use the preceding character's format, except at the beginning of the paragraph, where we use the following character's format.

//	if (prev.line_no

	lines.at(i.line_no).insert_ch(i.ch_index, fch);

	// Consider the implications of using -1 here.
	int i = 0;
	for (i = i.line_no, i < lines.size() - 1; i++){
		lines[i].relieve_excess(lines[i+1], line_width);
	}

	while (lines[i].exceeds_width_non_whitespace(line_width)){
		lines.push_back(Line());
		lines[i].relieve_excess(lines[i+1], line_width);
		i++;
	}
}

char Paragraph::delete_ch(p_index i){
	lines.at(i.line_no).delete_ch(i.ch_index);
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
