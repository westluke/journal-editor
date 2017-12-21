#include "line.hpp"
#include "text.hpp"

//#define NDEBUG
#include <cassert>

#include <iostream>




/****************
 * READ METHODS
 ****************/

// The number of characters in the line, INCLUDING trailing whitespace.
text_index Line::size() const{
	return text.size();
}

fchar Line::get_ch(text_index i) const{
	assert(i < size());
	return text[i];
}

text_type Line::get_text() const {
	return text;
}

text_type Line::get_text(text_index start, text_index end) const{
	assert((start >= 0) && (start <= end) && (end <= size()));
	return text_type(text.begin() + start, text.begin() + end);
}

std::string Line::get_string() const {
	return text_type_to_string(text);
}




/***********************
 * WRITE METHODS
 ***********************/

void Line::insert_ch(text_index i, fchar fch){
	assert(i <= size());
	text.insert(text.begin() + i, fch);
	mark_changed();
}

void Line::delete_ch(text_index i){
	assert(i < size());
	text.erase(text.begin() + i);
	mark_changed();
}

//void Line::replace_ch(text_index i, fchar ch){
//	assert(i < size());
//	delete_ch(i);
//	insert_ch(i, ch);
//	mark_changed();
//}

void Line::append_text(text_type txt){
	text.insert(text.end(), txt.begin(), txt.end());
	mark_changed();
}

void Line::prepend_text(text_type txt){
	text.insert(text.begin(), txt.begin(), txt.end());
	mark_changed();
}

void Line::delete_text(text_index start, text_index end){
	text.erase(text.begin() + start, text.begin() + end);
	mark_changed();
}




/***********************
 * EQUALIZATION METHODS
 ***********************/

bool Line::exceeds_width_non_whitespace(text_index line_width) const{
	assert(line_width > 0);

	if (size() == 0 || size() <= line_width) return false;

	// If any character beyond line_width isn't whitespace, this line is too big.
	for (text_index i = line_width; i < size(); i++){
		if (!text[i].isspace()){
			return true;
		}
	}
	return false;
}

// This paradigm needs to change, because this is wrong. It doesn't account for all-character lines. Right now my primitive is accept_flowback, but I think it could just as easily be relieve_excess. Which is the better choice, and how do I implement can_equalize with that choice?
bool Line::can_accept_flowback(Line &ln, text_index line_width) const{
	assert(line_width > 0);

	if (ln.size() == 0) return false;
	
	// Really we only need the first chunk.
	std::vector<text_type> split = ln.split_text();

	if (	split[0][0].isspace() ||
		size() + split[0].size() <= line_width) return true;
	return false;
}

bool Line::can_equalize(Line &ln, text_index line_width) const{
	assert(line_width > 0);
	return (exceeds_width_non_whitespace(line_width) || 
		can_accept_flowback(ln, line_width));
}

bool Line::equalize(Line &ln, text_index line_width){
	assert(line_width != 0);

	if (exceeds_width_non_whitespace(line_width)) return relieve_excess(ln, line_width);
	else return accept_flowback(ln, line_width);
}


// Currently I have no way to break up a word between lines in relieve excess or accept_flowback. But I think I only need to implement one of these, and can use it to make the other. So which one do I implement, and how do I implement it correctly?
// I think I could do it with relieve_excess. Pretty simple to check if the only element in the thing is made of characters.
// But then could I do it with accept_flowback by the same token? I would have to check that everything in the current line is made of characters, and the first thing being taken is also all made of characters, and also it overflows. So let's implement relieve_excess.
//
// Then, with relieve_excess, how do I implement accept_flowback? I simply take the entire second line and append it to the first, then relieve excess. Not bad.
// so let's do that.
bool Line::accept_flowback(Line &ln, text_index line_width){
	assert(line_width != 0);

	std::vector<text_type> split = ln.split_text();

	std::vector<text_type>::size_type i;
	text_index sum = 0;
	text_index current = size();

	for(i = 0; i < split.size(); i++){
		text_type next = split[i];

		if (next[0].isspace()) sum += next.size();
		else if (current + sum + next.size() <= line_width) sum += next.size();
		else break;
	}

	if (sum == 0) return false;

	append_text(ln.get_text(0, sum));
	ln.delete_text(0, sum);
	mark_changed();
	return true;
}

bool Line::relieve_excess(Line &ln, text_index line_width){
	assert(line_width > 0);

	if not(exceeds_width_non_whitespace(line_width)) return false;

	text_index sum = 0;
	std::vector<text_type> split = ln.split_text();
	// assert that split is not empty.
	// Make no assumptions. assert that every element of split is not empty.
	// That test belongs here, with the assumption.
	if (split.size() == 1) and (split[0].size() != 0) and (!split[0][0].isspace()){
		sum = line_width
	} else {

	//	Because we know that if the thing isn't entirely made of characters, it WILL be split at a word boundary.

		// BETTER IDEA. TRACK THE SUM, THEN PREPEND WHAT YOU DELETE FROM THE END.
		for (std::vector<text_type>::size_type i = 0; i < split.size(); i++){
			sz = split[i].size();
			if (!split[i][0].isspace()){
				if (sum + sz > line_width) break;
			}
			sum += sz;
		}
	}

	ln.prepend_text(delete_text(sum, size()));




	/*
	Line temp = Line("");
	temp.accept_flowback(*this, line_width);
	ln.prepend_text(get_text());
	text = temp.text;

	if (ret) mark_changed();

	return ret;
	*/
}

std::vector<text_type> Line::split_text() const{
	std::vector<text_type> ret;

	text_index i = 0;
	text_index prev = 0;
	text_index end = size();

	if (end == 0) return ret;

	bool flip = text[0].isspace();

	while (i < end){
		for(; i < end && (text[i].isspace() == flip); i++);
		ret.push_back(get_text(prev, i));
		prev = i;
		if (i == end) break;
		
		for(; i < end && (text[i].isspace() != flip); i++);
		ret.push_back(get_text(prev, i));
		prev = i;
	}

	return ret;
}



/********
 * OTHER
 ********/

Line::Line(const text_type &t): text(t), changed(true) {}
Line::Line(const std::string &str): text(string_to_text_type(str)), changed(true) {}
Line::Line(const char *str): text(string_to_text_type(std::string(str))), changed(true) {}
Line::Line(): text(), changed(true) {}

bool Line::was_changed() const{
	return changed;
}

bool Line::_clear_changed_flag() {
	bool temp = changed;
	changed = false;
	return temp;
}

bool Line::operator==(const Line &ln) const{
	return text == ln.text;
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
