#include "Line.hpp"

//#define NDEBUG
#include <cassert>

void print(TextStyle a){
	std::cout << static_cast<int>(a) << std::endl;
}

Line::index_type Line::line_length(){
	return text.size();
}

bool Line::exceeds_width_non_whitespace(Line::index_type line_width){
	for (int i = text.size() - 1; i >= line_width; i--){
		if (!text[i].isspace()){
			return true;
		}
	}
	return false;
}


// Maybe test where line width is more than text sizes, currently handled with assert
bool Line::relieve_excess(Line &ln, Line::index_type line_width){
	// This shouldn't be able to happen, so we assert.
	assert((line_width > 0));

	// This could conceivably happen, so we just return false for this.
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

// RETHING TESTING PATHS
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// RETHING TESTING PATHS
//
//
//
//
//
//
//
//
//
//
//
//

// ln could be empty
// the line could be all whitespace for that first for, so it arrives at ln.text.size
// the line could  be all character for that second for
// the line could start with some whitespace and then jump past a word, where that word fits
// the line could start with some whitespace and then jump past a word, where that word doesn't fit and it has to revert. Make that two words actually.
// it could also not start with whitespace and do those things, but I don't think i need to test that.
// well, test it just once.
//
// so, thats 6 test cases. good!
//
//
// NOPE: our line could already be full (do we test that? should I test that at the beginning?) no, because we can always flow back whitespace.
bool Line::accept_flowback(Line &ln, Line::index_type line_width){
	// This shouldn't ever be able to happen.
	assert((line_width > 0));

	// On the other hand, its possible that we attempt to flow back from an empty line.
	if (ln.text.size() == 0) return false;
	
	Line::index_type i = 0, last_index = 0;

	// Better way:
	// advance one character at a time. We have a few conditions to check. Is it whitespace? Is the index past the end of ln? Do we have space left in the line?
	// When we reach the first invalid index, we copy up to that point.
	while (true){
		// jump past the rest of the whitespace, arriving on first non-whitespace or on ln.text.size().
		for (; i < ln.text.size() && ln.text[i].isspace(); i++);

		// jump past the rest of the word, arriving on first whitespace after a word or on ln.text.size().
		for (; i < ln.text.size() && !ln.text[i].isspace(); i++);
		
		// if the current size doesn't fit, use the last index.
		if ((i > line_width - text.size()) || (i == ln.text.size())) break;

		last_index = i;
	}


	// hrm whats going on here. comment this better.
	for (; ln.text[last_index].isspace() && last_index < ln.text.size(); last_index++);

	if (last_index == 0) return false;

	text.insert(text.end(), ln.text.begin(), ln.text.begin() + last_index);
	ln.text.erase(ln.text.begin(), ln.text.begin() + last_index);
	return true;
}

void Line::insert_ch(Line::index_type i, fchar fch){
	assert((i < text.size()));
	text.insert(text.begin() + i, fch);
}

fchar Line::delete_ch(Line::index_type i){
	fchar ret = text.at(i);
	text.erase(text.begin() + i);
	return ret;
}

Line::text_type Line::string_to_text_type(const std::string &str){
	text_type t;
	for (auto ch : str){
		t.push_back(fchar(ch));
	}
	return t;
}

Line::text_type Line::string_to_text_type(const char *str){
	std::string new_str = std::string(str);
	text_type t;
	for (auto ch : new_str){
		t.push_back(fchar(ch));
	}
	return t;
}

std::string Line::text_type_to_string(const Line::text_type &t){
	std::string str;
	for (auto fch : t) str.push_back(fch.character);
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
Line::Line(): text() {}
