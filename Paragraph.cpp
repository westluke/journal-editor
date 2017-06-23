#include "Paragraph.hpp"

// Bitwise OR
inline Format operator|(Format a, Format b){
	return static_cast<Format>(static_cast<char>(a) | static_cast<char>(b));
}

// Bitwise AND
inline Format operator&(Format a, Format b){
	return static_cast<Format>(static_cast<char>(a) & static_cast<char>(b));
}

// Bitwise XOR
inline Format operator^(Format a, Format b){
	return static_cast<Format>(static_cast<char>(a) & static_cast<char>(b));
}

// Bitwise NOT
inline Format operator~(Format a){
	return static_cast<Format>(~static_cast<char>(a));
}

Line::index_type Line::line_length(){
	return text.size();
}

//Problem, big problem. What the hell happens at the end of a line, when you add spaces or tabs?
//
//just think about spaces for now cuz they are equivalent.
//but also newlines will be a problem. nah, cuz they get reinterpreted.
//
//ok so heres the thing. when i get to the end of a line and start adding spaces, i want to see the cursor move with those spaces.
//That means those spaces are being added to the line, the visible line. But no matter what, the cursor must never EVER exceed the line width.
//So once the cursor reaches the end of the line, it can either stick there or wrap around to the next. If it wrapped around to the next, then adding more
//space would have to just be adding space to the end of the previous line, which is a little weird.
//The other alternative is it sticks at the end of the line. Either it sticks at the end of that line, or sticks at the beginning of the next.
//
//and how would i implement that. hmmm.
//
//I could for a last, maybe more pure option: whitespace is treated as a proper character, but linebreaks happen naturally on whitespace so it isn't really noticed.
//And everything that isn't inputted characters has a background in a slightly different color to show me shit.
//that seems more in keeping with my style
//
//seems like  most text editors make spaces stop abruptly at the end of the line, but tabs use tabstops.
//I'm not using tabstops right now.
//should i use tabstops? idek how that would work. Every change in a line would force me to adjust all the tab stops in the line
//which might not actually be terrible. Also it would usually be only one tabstop.
//
//In any case, ignore tabstops for now. just spaces. stop at end of line? how? alternative is easy.

// Ahhh, problem. Stopping at the end of the line requires having a different sort of cursor location. I would have to know, at all times, where the cursor was in relationasdf to the paragraph, and operate on that instead of on the location relative to the screen.
// Would that be a good idea? Makes a little more sense when I introduce Lines.
//
// How would this work? the line stores everything, including whitespace, and displays everything, all the way out to the line width.
// It permits an entire line of whitespace, but not more than one in succession, because the line will ONLY wrap when a NON-WHITESPACE character extends beyond
// the line_width. You can add whitespace at the beginning of a line, but it is automatically appended to the previous line.
// The only difference between this and that other idea is with the other idea it wraps when ANY character extends beyond the line, just tries to do it on some whitespace, and preceding whitespace is not appended to the previous. So I could probably switch back and forth with flags. Lets do that!
Line::text_type pop_words(Line::index_type min_size){
//	"abc defg"
//	size 8, whitespace at 3
//	would output word length 4
//
	Line::text_type ret;
	std::vector::iterator start;

	if (min_size >= text.size() || min_size < 0){
		throw std::domain_error("Number of characters popped from a Line must be less than the size of the Line and positive.");
	}
	
	for (int i = text.size() - 1; i >= 0; i++){
		if (text[i].isspace()){
			// This breakpoint satisfies the minimum size
			if ((text.size() - (i + 1)) >= min_size){
				start = text.begin() + i;
				ret = std::vector(start, text.end());
				text.erase(start, text.end());
				return ret;

				// that's our mark
				// some combination of swap, erase, assign.
			}

			// otherwise, keep going until you hit non whitespace again, or until you hit -1.
			while (text[i].isspace() && i >= 0){
				i++
			}
		}
	}

	start = text.end() - min_size;
	ret = std::vector(start, text.end());
	text.erase(start, text.end());
	return ret;

	// there is no whitespace that works

	update_first_and_last_space();
}


Paragraph::Paragraph(Line::index_type lw): line_width(lw){
	;
}
