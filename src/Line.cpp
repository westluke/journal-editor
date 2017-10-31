#include "line.hpp"
#include "text.hpp"

//#define NDEBUG
#include <cassert>




/****************
 * CONST METHODS
 ****************/

// The number of characters in the line, INCLUDING trailing whitespace.
text_index Line::size() const{
	return text.size();
}

// The number of characters in the line, EXCLUDING trailing whitespace.
//text_index Line::visual_size() const;

bool Line::exceeds_width_non_whitespace(text_index line_width) const{
	// Go backwards from the end of the line until index line_width.
	// If you hit anything that isn't whitespace, the line is too long.
	
	assert(line_width > 0);
	if (size() == 0) return false;

	for (text_index i = size() - 1; i >= line_width; i--){
		if (!text[i].isspace()){
			return true;
		}
	}
	return false;
}

bool Line::can_equalize(Line &ln, text_index line_width) const{
	assert(line_width > 0);
	return (exceeds_width_non_whitespace(line_width) || can_accept_flowback(ln, line_width));
}

bool Line::can_accept_flowback(Line &ln, text_index line_width) const{
	assert(line_width > 0);

	if (ln.size() == 0) return false;
	if (ln.text[0].isspace()) return true;

	// As soon as we find whitespace, if we fit into the line, return true.
	for (text_index i = 0; i < ln.size(); i++){
		if (ln.text[i].isspace()){
			if ((ln.size() + i) <= line_width) return true;
			else return false;
		}
	}
	return false;
}

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




/***********************
 * MODIFICATION METHODS
 ***********************/

void Line::insert_ch(text_index i, fchar fch){
	assert((i <= size()));
	text.insert(text.begin() + i, fch);
	mark_changed();
}

fchar Line::delete_ch(text_index i){
	assert((i < size()));
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

//TODO: I think the relationships between equalize and relieve_excess and flowback are inefficient. FIX THAT

/*
bool Line::equalize(Line &ln, text_index line_width){
	assert(line_width > 0);

	if (exceeds_width_non_whitespace(line_width))
		return relieve_excess(ln, line_width);
	else if (can_accept_flowback(ln, line_width))
		return accept_flowback(ln, line_width);
	else return false;
}
*/

/*
bool Line::relieve_excess(Line &ln, text_index line_width){
	assert((line_width > 0));

	if (line_width >= size()) return false;

	// Unfortunately have to do this in order to check that i becomes negative.
	text_index i;
	if (text[line_width].isspace()){
		for (i = line_width + 1; text[i].isspace() && i < size(); i++);
		// Now, i points to first non-whitespace character after line_width OR to size().
		// IF the former, we break there. If the latter, no breaking is necessary.
		if (i == size()) return false;
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
*/

// NO DONT ASSUME ANYTHING HERE.
// It's possible to break these functions into pieces of work that do NOT require assumptions.
// But is there any way to make this function shorter?`
// We can abstract the insertion and deletion.
// What does the rest of it do? It adds words to a LINE until the line exceeds a width. What width? line-width - size().
// BOOM.

/*
bool Line::accept_flowback(Line &ln, text_index line_width){
	assert(line_width > 0);
	if (ln.size() == 0) return false;
	accept_whitespace(ln);
	Line max = greatest_line_below_width(ln, line_width);
	join(max);
	delete(ln, max);
}
*/

/*
bool Line::accept_whitespace(Line &ln){
	text_index i;

	// i is the first index that we cannot take.
	for (i = 0; i < ln.size() && ln.text[i].isspace(); i++);
	if (i == 0) return false;
	else {
		append_str(ln.delete_str(0, i));
	}
}
*/

text_type Line::get_text(text_index start, text_index end) const{
	assert(start >= 0 && end <= size());
	return text_type(text.begin() + start, text.begin() + end);
}

std::vector<text_type> Line::split_text() const{
	std::vector<text_type> ret;

	text_index i = 0;
	text_index prev = 0;
	text_index end = size();

	if (end == 0) return ret;

	bool flip = text[0].isspace();

	// if flip is true, we test whitespace then non-whitespace.
	// otherwise, we test non-whitespace then whitespace.

	// so this will work fine on empty text.
	// how about just whitespace? just fine.
	// it's just that starting case.
	
	while (i < end){
		for(; i < end && (text[i].isspace() == flip); i++);
		ret.push_back(get_text(prev, i));
		prev = i;
		if (i == end) break;
		
		for(; i < end && (text[i].isspace() != flip) ; i++);
		ret.push_back(get_text(prev, i));
		prev = i;
	}

	return ret;
}

// For both accept_flowback and relieve_excess, it might actually be helpful to split the text into a vector of text,
// where each text alternates whitespace and character. Then this function here would be easy.
//
// And for relieve excess? we just keep taking off sections until we arrive at a Line below the line width.
//
// However, it does slightly increase runtime. On the other hand, the runtime here is always gonna be pretty bounded.
// These could be optimized later, but for now its better for me to undrestand then.
// Because I don't understand these functions at all, and that means I don't trust them, and that means I code slowly.

/*
bool Line::greatest_line_below_width(Line &ln, text_index line_width){
	assert(line_width > 0);

	Line max = Line("");
	text_index i;

	while (true){

	}

}
*/

void Line::append_text(text_type txt){
	text.insert(text.end(), txt.begin(), txt.end());
}

void Line::delete_text(text_index start, text_index end){
	text.erase(text.begin() + start, text.begin() + end);
}


bool Line::equalize(Line &ln, text_index line_width){
	assert(line_width != 0);

	if (exceeds_width_non_whitespace) return relieve_excess(ln, line_width);
	else return accept_flowback(ln, line_width);
}


// Great! Now we go back to testing, thoroughly this time.

bool Line::accept_flowback(Line &ln, text_index line_width){
	assert(line_width != 0);

	std::vector<text_type> split = ln.split_text();

	text_index i;
	text_index sum = 0;
	text_index old_size = size();

	for(i = 0; i < split.size(); i++){
		text_type next = split[i];

		if (next[0].isspace()) sum += next.size();
		else if (old_size + sum + next.size() <= line_width) sum += next.size();
		else break;
	}

	if (sum == 0) return false;

	append_text(ln.get_text(0, sum));
	ln.delete_text(0, sum);
	return true;
}

bool Line::relieve_excess(Line &ln, text_index line_width){
	assert(line_width != 0);

	bool ret = exceeds_width_non_whitespace(line_width);

	std::vector<text_type> split = split_text();

	Line temp = Line("");
	temp.accept_flowback(*this);
	ln.prepend(get_text());
	text = temp.text;

	return ret;

	// Some line has to accept the flowback. That line must be empty.
	// But that line must in the end become this line.
	// To me that implies constructing a duplicate, deleting my text, and refilling it. But in the end its the same thing.

	// ok so clearly i can implement this with accept flowback, which is convenient. btu how?
	// Also, i just realized its entirely possible for a line to have relieve_excess called on itself. Is that bad?
	// Actually, I could implement a numerical id system. IS that worth it?
	// Could it be useful for any other purpose? probably not. So let's only do it if it becomes necessary.
}

/*
bool Line::OLDaccept_flowback(Line &ln, text_index line_width){
	assert((line_width > 0));

	// On the other hand, its possible that we attempt to flow back from an empty line.
	// But I could just as well return true here? Nah return false, nothing was moved.
	if (ln.size() == 0) return false;
	
	text_index i;

	// Pretend we are allowed to break in the middle of words. Find the first character that cannot be included in the break. Set i to that position.
	// Wait this doesn't make any sense to me. There must be a way to write these functions better.
	// WHY DOES THIS NEED THAT
	for (i = 0; i < ln.size(); i++){
		if (	((size() > line_width) || (i >= line_width - size())) &&
			!ln.text[i].isspace()) break;
	}

	// i will be zero here only if size() is the same as line_width and the first character is non-space.
	if (i == 0) return false;

	// We only need to take further action if i lies in the middle of a word.
	if (i != ln.size() && !ln.text[i-1].isspace()){

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
*/




/********
 * OTHER
 ********/

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

bool Line::_clear_changed_flag() {
	bool temp = changed;
	changed = false;
	return temp;
}




/**********
 * PRIVATE
 **********/

void Line::mark_changed(){
	changed = true;
}




/*******************
 * STREAM OPERATORS
 *******************/

std::ostream& operator<<(std::ostream& os, const Line& ln){
	os << ln.get_string();
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Line>& vol){
	for (auto ln: vol) os << ln << std::endl;
	return os;
}
