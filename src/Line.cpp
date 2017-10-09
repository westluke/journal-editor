#include "line.hpp"
#include "text.hpp"

//#define NDEBUG
#include <cassert>



text_index Line::size() const{
	return text.size();
}

bool Line::exceeds_width_non_whitespace(text_index line_width) const{
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
void Line::insert_ch(text_index i, fchar fch){
	assert((i <= text.size()));
	text.insert(text.begin() + i, fch);
	mark_changed();
}

fchar Line::delete_ch(text_index i){
	assert((i < text.size()));
	fchar ret = text[i];
	text.erase(text.begin() + i);
	mark_changed();
	return ret;
}

fchar Line::replace_ch(text_index i, fchar ch){
	fchar ret = delete_ch(i);
	insert_ch(i, ch);
	mark_changed();
	return ret;
}

//void insert_str(index_type i, std::string str, TextStyle style = TextStyle::none);
//std::string delete_str(index_type i, index_type length);
//std::string replace_str(index_type i, std::string str, TextStyle style = TextStyle::none);

//void insert_text(index_type i, text_type txt);
//text_type delete_text(index_type i, index_type length);
//text_type replace_text(index_type i, text_type txt);

//void change_ch_style(index_type i, TextStyle style);
//void change_style(index_type start, TextStyle style);
//void change_style(index_type start, index_type end, TextStyle style);

fchar Line::get_ch(text_index i) const{
	return text.at(i);
}

text_type Line::get_text() const {
	return text;
}

//text_type get_text(index_type start) const;
//text_type get_text(index_type start, index_type end) const;

std::string Line::get_string() const {
	return text_type_to_string(text);
}

// std::string get_string(index_type start) const;
// std::string get_string(index_type start, index_type end) const;


Line::Line(const text_type &t): text(t), changed(true) {}
Line::Line(const std::string &str): text(string_to_text_type(str)), changed(true) {}
Line::Line(const char *str): text(string_to_text_type(std::string(str))), changed(true) {}
Line::Line(): text(), changed(true) {}

bool Line::operator==(const Line &ln) const{
	return text == ln.text;
}

bool Line::was_changed() const{
	return changed;
}

// bool _clear_changed_flag();

//TODO: I think the relationships between equalize and relieve_excess and flowback are inefficient. FIX THAT
bool Line::equalize(Line &ln, text_index line_width){
	// Guaranteed to move something from this line to the next, leaving this one under the limit,
	// with no whitespace on the next line.
	if (exceeds_width_non_whitespace(line_width))
		return relieve_excess(ln, line_width);
	// Not guaranteed to move anything back, but still ensures that the second line has no leading whitespace.
	else
		return accept_flowback(ln, line_width);
}


bool Line::relieve_excess(Line &ln, text_index line_width){
	// This shouldn't be able to happen, so we assert.
	assert((line_width > 0));

	// This could conceivably happen, so we just return false for this.
	if (line_width >= text.size()) return false;

	// Unfortunately have to do this in order to check that i becomes negative.
	text_index i;
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

bool Line::accept_flowback(Line &ln, text_index line_width){
	// This shouldn't ever be able to happen.
	assert((line_width > 0));

	// On the other hand, its possible that we attempt to flow back from an empty line.
	// But I could just as well return true here? Nah return false, nothing was moved.
	if (ln.text.size() == 0) return false;
	
	text_index i = 0;

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

void Line::mark_changed(){
	changed = true;
}

std::ostream& operator<<(std::ostream& os, const Line& ln){
	os << ln.get_string();
	return os;
}


// make this better
std::ostream& operator<<(std::ostream& os, const std::vector<Line>& vol){
	for (auto ln: vol) os << ln << std::endl;
	return os;
}
