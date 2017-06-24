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

bool Line::exceeds_width(Line::index_type line_width){
	for (int i = text.size(); i >= line_width; i--){
		if (!text[i].isspace()){
			return true;
		}
	}
	return false;
}

bool Line::relieve_excess(Line &ln, Line::index_type line_width){
	// There is no reason for this to ever happen, so we throw an error.
	if (line_width <= 0) {
		throw std::domain_error("line_width must be positive.");
	}

	// Informs the calling function that a break is unnecessary.
	if (all_whitespace) return false;

	Line::index_type i;
	if (text[line_width].isspace()){
		for (i = line_width + 1; text[i].isspace() && i < text.size(); i++);
		// Now, i points to first non-whitespace character after line_width OR to text.size().
		// IF the former, we break there. If the latter, no breaking is necessary.
		if (i == text.size()) return false;
	} else {
		for (i = line_width - 1; !text[i].isspace() && i >= 0; i--);
		// Now, either we found whitespace or we didn't. If we didn't, then i is negative.
		// If we did, i points to that first whitespace.

		if (i < 0) i = line_width;
		else i++;
	}
	
	Line::index_type start = text.begin() + i;
	ln.text.insert(ln.text.begin(), start, text.end());
	text.erase(start, text.end());
	return true;
}

bool Line::accept_flowback(Line &ln, Line::index_type line_width){

	// Unsigned, but could still be zero.
	if (line_width <= 0) throw std::domain_error("line_width must be positive.");
	if (ln.text.size() == 0) throw std::domain_error("ln must contain characters.");
	
	Line::index_type i = 0, last_index = 0;

	while (true){
		// jump past the rest of the whitespace, arriving on first non-whitespace or on ln.text.size().
		for (; ln.text[i].isspace() && i < ln.text.size(); i++);

		// jump past the rest of the word, arriving on first whitespace after a word or on ln.text.size().
		for (; !ln.text[i].isspace() && i < ln.text.size(); i++);
		
		// We don't need to catch the eventuality of landing on ln.text.size() because it's always safe to copy an entire line into the previous.
		// if the current size doesn't fit, use the last index.
		if (i > line_width - text.size() || i == ln.text.size()) break;

		last_index = i;
	}

	for (; ln.text[last_index].isspace() && last_index < ln.text.size(); i++);

	if (last_index == 0) return false;

	text.insert(text.end(), ln.text.begin(), ln.text.begin() + last_index);
	ln.text.erase(ln.text.begin(), ln.text.begin() + last_index);
	return true;
}

void Line::insert_ch(Line::index_type i, fchar fch){
	text.insert(text.begin() + i, fch);
}

fchar Line::delete_ch(Line::index_type i){
	fchar ret = text.at(i);
	text.erase(text.begin() + i);
	return ret;
}
