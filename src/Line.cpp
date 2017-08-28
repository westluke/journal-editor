#include "line.hpp"

//#define NDEBUG
#include <cassert>




// Printing operator for TextStyle
std::ostream& operator<<(std::ostream& os, const TextStyle& ts){
	os << static_cast<int>(ts);
	return os;
}




Line::index_type Line::size(){
	return text.size();
}

bool Line::exceeds_width_non_whitespace(Line::index_type line_width){
	// Go backwards from the end of the line until the technical line limit, where the line should end.
	// If you hit anything that isn't whitespace, the line is too long.
	for (int i = text.size() - 1; i >= 0 && i >= line_width; i--){
		if (!text[i].isspace()){
			return true;
		}
	}
	return false;
}

// JUST REALIZED: right now, paragaph takes care of using the proper format, like the one that came before this character.
// Maybe Line should do that instead. NOPE, cuz Line doesn't and can't know the starting format. All text that comes in MUST be formatted, or have a format specified.
void Line::insert_ch(Line::index_type i, fchar fch){
	assert((i <= text.size()));
	text.insert(text.begin() + i, fch);
	mark_changed();
}

fchar Line::delete_ch(Line::index_type i){
	assert((i < text.size()));
	fchar ret = text[i];
	text.erase(text.begin() + i);
	mark_changed();
	return ret;
}

fchar Line::replace_ch(Line::index_type i, fchar ch){
	fchar ret = delete_ch(i);
	insert_ch(i, ch);
	mark_changed();
	return ret;
}

//void Line::insert_str(Line::index_type i, std::string str, TextStyle style = TextStyle::none);
//std::string Line::delete_str(Line::index_type i, Line::index_type length);
//std::string Line::replace_str(Line::index_type i, std::string str, TextStyle style = TextStyle::none);

//void Line::insert_text(Line::index_type i, Line::text_type t);
//
fchar Line::get_ch(Line::index_type i){
	return text.at(i);
}

Line::text_type Line::get_text() {
	return text;
}

std::string Line::get_string() const {
	return text_type_to_string(text);
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


Line::Line(const Line::text_type &t): text(t), changed(true) {}
Line::Line(const std::string &str): text(string_to_text_type(str)), changed(true) {}
Line::Line(const char *str): text(string_to_text_type(std::string(str))), changed(true) {}
Line::Line(): text(), changed(true) {}

bool Line::operator==(const Line &ln) const{
	return text == ln.text;
}


bool Line::was_changed(){
	return changed;
}

void Line::clear_changes(){
	changed = false;
}

void Line::mark_changed(){
	changed = true;
}

//TODO: I think the relationships between equalize and relieve_excess and flowback are inefficient. FIX THAT
bool Line::equalize(Line &ln, Line::index_type line_width){
	// Guaranteed to move something from this line to the next, leaving this one under the limit,
	// with no whitespace on the next line.
	if (exceeds_width_non_whitespace(line_width))
		return relieve_excess(ln, line_width);
	// Not guaranteed to move anything back, but still ensures that the second line has no leading whitespace.
	else
		return accept_flowback(ln, line_width);
}


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

	mark_changed();
	return true;
}

bool Line::accept_flowback(Line &ln, Line::index_type line_width){
	// This shouldn't ever be able to happen.
	assert((line_width > 0));

	// On the other hand, its possible that we attempt to flow back from an empty line.
	// But I could just as well return true here? Nah return false, nothing was moved.
	if (ln.text.size() == 0) return false;
	
	Line::index_type i = 0;

	// Pretend we are allowed to break in the middle of words. Find the first character that cannot be included in the break. Set i to that position.
	while (true){
		if (i == ln.text.size()) break;
		if (	((text.size() > line_width) || (i >= line_width - text.size())) &&
			!ln.text[i].isspace()) break;
		i++;
	}

	// i will be zero here only if text.size() is the same as line_width and the first character is non-space.
	if (i == 0) return false;

	// We only need to take further action if i lies in the middle of a word.
	if (i != ln.text.size() && !ln.text[i-1].isspace()){

		while (true){
			i--;
			if (ln.text[i].isspace()){
				i++;	// Stop at the character just after the whitespace.
				break;
			}
			if (i == 0) return false; // We can't break if the characters continue all the way to the beginning.
		}
	}

	text.insert(text.end(), ln.text.begin(), ln.text.begin() + i);
	ln.text.erase(ln.text.begin(), ln.text.begin() + i);

	mark_changed();
	return true;
}

std::ostream& operator<<(std::ostream& os, const Line& ln){
	os << ln.get_string();
	return os;
}


// make this better
std::ostream& operator<<(std::ostream& os, const std::vector<Line>& vol){
	for (auto ln: vol){
		std::cout << ln << "\n";
	}
	return os;
}
