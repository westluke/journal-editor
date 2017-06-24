#include "Line.hpp"

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
//
//
//
// Nah here's the difference between my two options.
// option 1: freedom.
// NO characters may exceed line width, and whitespace may prepend line
//
// option 2: semi-freedom
// whitespace only may exceed line width, and whitespace may not prepend line.
//
// Real difference: with option 1 we break lines every time any character breaks the line width,
// and we are not obligated to break on a working whitespace that comes first backwards.
//
// with option 2 we break lines only when a non-whitespace character breaks the line width,
// and we must always on a working whitespace that comes first backwards.
//
// There is sort of a third option. semisemi freeedom, where NO characters may exceed line width, and also whitespace may not prepend line.
// We coulld only do that bu cutting the previous line at the right point. However, we then get to a point where we may cut out a lot extra in order to avoid
// trailing whitespace. So lets go with semi freedom.
//

Line::text_type Line::break_line(Line::index_type line_width){
	Line::text_type ret;
	Line::index_type start;

	if (line_width >= text.size() || line_width <= 0) {
		throw std::domain_error("line_width either exceeds line size or is zero/negative.");
	}

	// We shouldn't throw an error, we should just let calling function know that we don't need to break.
	bool all_whitespace = true;
	for (int i = line_width; i < text.size(); i++){
		if (!text[i].isspace()) all_whitespace = false;
	}
	if (all_whitespace) return ret;

	// From here, we are guaranteed that the line_width is positive and that there are non-whitespace characters beyond line_width.

	// When we break a line normally, i should point to a non-whitespace character right after a whitespace character, like this:
	// I ate one hundred hot dogs and I enj|oy them.
	// 				    ^
	// When the line is all text, it should just point to the character right after the line limit.
	// IateonehundredhotdogsandI|enjoythem.
	// 			     ^, i = line_width
	// What about when the split happens before some whitespace? Has to be on non-whitespace character!
	// I ate one hundred hot dogs and I| enjoy them.
	// 				     ^
	// And after some whitespace?
	// I ate one hundred hot dogs and I |enjoy them.
	// 				     ^, i = line_width
	//
	// After some contiguous  whitespace after the limit?
	// I ate one hundred hot dogs and I |		enjoy them.
	// 						^
	

	// If text[line_width] is whitespace, we should search forward until we find non-whitespace. We know we will find some.
	//
	// If text[line_width] is not whitespace, we must search backwards for the first whitespace we find, then break right after.
	// If we find no whitespace, we break on line_width.
	
	int i;
	if (text[line_width].isspace()){
		for (i = line_width + 1, text[i].isspace(); i++);
		// Now, i points to first non-whitespace character after line_width, and we can break there.
		// Because of checking done earlier, we don't even need to put a condition for the size of i in the for loop.
	} else {
		for (i = line_width - 1, !text[i].isspace() && i >= 0; i00);
		// Now, either we found whitespace or we didn't. If we didn't, then i is negative.
		// If we did, i points to that first whitespace.

		if (i < 0) i = line_width;
		else i++;
	}
	
	start = text.begin() + i;
	ret = std::vector(start, text.end());
	text.erase(start, text.end());
	return ret;
}

bool Line::exceeds_width(Line::index_type line_width){
	if (preceding_whitespace_mode){
		return (text.size() > line_width);
	} else {
		for (int i = text.size(); i >= line_width; i--){
			if (!text[i].isspace()){
				return true;
			}
		}
		return false;
	}
}

// Always prepends the text. Returns whether or not the resulting Line conforms to the no-preceding-whitespace rule.
void Line::prepend_text(text_type words){
	text.insert(text.begin(), words.begin(), words.end());
	return (!words[0].isspace());
}
