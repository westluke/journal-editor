#include "Paragraph.hpp"

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
void Paragraph::insert_ch(p_index i, int ch){
	// INCOMPLETE, CHANGE FORMAT

	fchar fch;
	fch.ch = ch;

	p_index prev = previous_index(i);
	if (prev == i){
		// set default format. But that just means that this is the very first character, so there's a better way to do this.
		// Actually, is there? It can be the first character, even when there are other characters in the line.
		// That implies I need two default formats for characters, one for the end and one for the beginning. check drive.
		// actually google drive doesn't do that. When inserting at the beginning, it just takes the format of the next character, UNLESS its also the first character to be entered in the paragraph
		// What if its between two paragraphs?
		// That depends on which paragraph creates the space. interesting. Should I mimic that behavior? Yeah I probalbly should. That's the only way to
		// make it easy to extend formatting into a paragraph above. Still, we have the same structure, with a format option maintained that applies to appending to the paragraph. Anywhere else, we use the preceding character's format, except at the beginning of the paragraph, where we use the following character's format.
	}

	if (prev.line_no

	fch.ft = get_ch(previous_index(i)).ft;
	lines.at(i.line_no).insert_ch(i.ch_index, FCH);
}

char Paragraph::delete_ch(p_index i){
	lines.at(i.line_no).delete_ch(i.ch_index);
}

fchar Paragraph::get_ch(p_index i){
	return lines.at(i.line_no).get_ch(i.ch_index);
}

// What if the previous line is empty? That shouldn't be able to happen. If it does, that should be an error. But this isn't the right place to throw it.
p_index Paragraph::previous_index(p_index i){
	p_index ret;

	if (i.ch_index){
		ret.ch_index = i.ch_index - 1;
		ret.line_no = i.line_no;
	} else if (i.line_no){
		ret.line_no = i.line_no - 1;
		ret.ch_index = lines.at(ret.line_no).line_length() - 1;
	} else {
		return i;
	}
}

bool apply_format(p_index start, p_index end, Format f);

std::vector<Line::text_type> get_lines();

void set_line_width(Line::index_type lw);

void set_header_level(HeaderLevel hl);


Paragraph::Paragraph(Line::index_type lw): line_width(lw){
	;
}
