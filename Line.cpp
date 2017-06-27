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

bool Line::exceeds_width_non_whitespace(Line::index_type line_width){
	for (int i = text.size(); i >= line_width; i--){
		if (!text[i].isspace()){
			return true;
		}
	}
	return false;
}

bool Line::relieve_excess(Line &ln, Line::index_type line_width){
	// There is no reason for this to ever happen, so we throw an error.
	if (line_width <= 0) throw std::domain_error("line_width must be positive.");
	if (line_width >= text.size()) return false;

	// Unfortunately have to do this in order to check that i becomes negative.
	Line::index_type i;
	if (text[line_width].isspace()){
		for (i = line_width + 1; text[i].isspace() && i < text.size(); i++);
		// Now, i points to first non-whitespace character after line_width OR to text.size().
		// IF the former, we break there. If the latter, no breaking is necessary.
		if (i == text.size()) return false;
	} else {
		for (i = line_width - 1; !text[i].isspace() && i > 0; i--);
		// Either we found some whitespace above index 0, or there is whitespace at index 0, or there is no whitespace.

		// If the first, then this fails.
		// If the second, this fails.
		// If the last, this is true as it should be.
		if (i == 0 && !text[i].isspace()) i = line_width;
		else i++;
	}
	
	auto start = text.begin() + i;
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
		if ((i > line_width - text.size()) || (i == ln.text.size())) break;

		last_index = i;
	}

	for (; ln.text[last_index].isspace() && last_index < ln.text.size(); last_index++);

	if (last_index == 0) return false;

	text.insert(text.end(), ln.text.begin(), ln.text.begin() + last_index);
	ln.text.erase(ln.text.begin(), ln.text.begin() + last_index);
	return true;
}

// We don't make sure that fch is an acceptable fchar, so that should be handled elsewhere.
void Line::insert_ch(Line::index_type i, fchar fch){
	if (i >= text.size()) throw std::out_of_range("Index beyond range of Line for character insertion.");
	text.insert(text.begin() + i, fch);
}

// We don't check that i is within range of the text because the vector method .at() should handle that.
fchar Line::delete_ch(Line::index_type i){
	fchar ret = text.at(i);
	text.erase(text.begin() + i);
	return ret;
}

Line::text_type Line::string_to_text_type(const std::string &str){
	text_type t;

	for (auto c : str){
		t.push_back({c, Format::none});
	}

	return t;
}

std::string Line::text_type_to_string(const Line::text_type &t){
	std::string str;

	for (auto fch : t){
		str.push_back(fch.ch);
	}

	return str;
}

fchar Line::get_ch(Line::index_type i){
	return text.at(i);
}

Line::text_type Line::get_text() {
	return text;
}

std::string Line::get_string() {
	return text_type_to_string(text);
}

Line::Line(const Line::text_type &t): text(t) {}
Line::Line(const std::string &str): text(string_to_text_type(str)) {}
Line::Line(const char *str): text(string_to_text_type(std::string(str))) {}
