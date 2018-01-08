#include "line.hpp"
#include "text.hpp"

//#define NDEBUG
#include <cassert>

#include <iostream>




/***************
 * CONSTRUCTORS
 ***************/

Line::Line(const text_type &t): text(t), changed(true) {}
Line::Line(const std::string &str): text(string_to_text_type(str)), changed(true) {}
Line::Line(const char *str): text(string_to_text_type(std::string(str))), changed(true) {}
Line::Line(): text(), changed(true) {}




/****************
 * READ METHODS
 ****************/

// The number of characters in the line, INCLUDING trailing whitespace.
line_size Line::size() const{
	return text.size();
}

fchar Line::get_fchar(line_size i) const{
	assert(i < size());
	return text[i];
}

text_type Line::get_text() const {
	return text;
}

text_type Line::get_text(line_size start, line_size end) const{
	assert((start >= 0) && (start <= end) && (end <= size()));
	return text_type(text.begin() + start, text.begin() + end);
}

std::string Line::get_string() const {
	return text_type_to_string(text);
}




/***********************
 * WRITE METHODS
 ***********************/

void Line::insert_ch(line_size i, fchar fch){
	assert(i <= size());
	text.insert(text.begin() + i, fch);
	mark_changed();
}

void Line::delete_ch(line_size i){
	assert(i < size());
	text.erase(text.begin() + i);
	mark_changed();
}

void Line::delete_last_ch(){
	text.pop_back();
	mark_changed();
}

void Line::append_ch(fchar ch){
	text.push_back(ch);
	mark_changed();
}

void Line::replace_ch(line_size i, fchar ch){
	assert(i < size());
	delete_ch(i);
	insert_ch(i, ch);
	mark_changed();
}

void Line::append_text(text_type txt){
	text.insert(text.end(), txt.begin(), txt.end());
	mark_changed();
}

void Line::prepend_text(text_type txt){
	text.insert(text.begin(), txt.begin(), txt.end());
	mark_changed();
}

void Line::delete_text(line_size start, line_size end){
	text.erase(text.begin() + start, text.begin() + end);
	mark_changed();
}




/***********************
 * EQUALIZATION METHODS
 ***********************/

bool Line::exceeds_width_non_whitespace(line_size line_width) const{
	assert(line_width > 0);

	if (size() == 0 || size() <= line_width) return false;

	// If any character beyond line_width isn't whitespace, this line is too big.
	for (line_size i = line_width; i < size(); i++){
		if (!text[i].isspace()){
			return true;
		}
	}
	return false;
}

bool Line::can_accept_flowback(Line &ln, line_size line_width) const{
	// If this line is all characters and below the line_width and ln is non-empty, return true.
	// Otherwise, make sure that the entire first chunk can fit.
	assert(line_width > 0);

	if (ln.size() == 0) return false;
	if (size() == 0) return true;

	text_type chunk = first_chunk();
	if ((!chunk[0].isspace()) and (chunk.size() == size()) and (size() < line_width)) return true;

	text_type ln_chunk = ln.first_chunk();
	if ((ln_chunk[0].isspace()) or (ln_chunk.size() + size() <= line_width)) return true;
	return false;
}

bool Line::can_equalize(Line &ln, line_size line_width) const{
	assert(line_width > 0);
	return (exceeds_width_non_whitespace(line_width) || 
		can_accept_flowback(ln, line_width));
}

void Line::equalize(Line &ln, line_size line_width){
	assert(line_width != 0);

	append_text(ln.get_text());
	ln.delete_text(0, ln.size());
	relieve_excess(ln, line_width);
}

bool Line::equalize_if_possible(Line &ln, line_size line_width){
	if (can_equalize(ln, line_width)){
		equalize(ln, line_width);
		return true;
	}
	return false;
}

void Line::relieve_excess(Line &ln, line_size line_width){
	assert(line_width > 0);

	line_size sum = 0;
	line_size sz;
	std::vector<text_type> split = split_text();

	assert(split.size() != 0);
	assert(split[0].size() != 0);

	if ((split[0].size() > line_width) and (!split[0][0].isspace())){
		sum = line_width;
	} else {
		for (std::vector<text_type>::size_type i = 0; i < split.size(); i++){
			sz = split[i].size();
			if ((!split[i][0].isspace()) and (sum + sz > line_width)) break;
			sum += sz;
		}
	}

	text_type end = get_text(sum, size());
	delete_text(sum, size());
	ln.prepend_text(end);
}

Line Line::overflow(line_size line_width){
	Line ret;
	relieve_excess(ret, line_width);
	return ret;
}

std::vector<text_type> Line::split_text() const{
	std::vector<text_type> ret;

	line_size i = 0;
	line_size prev = 0;
	line_size end = size();

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

text_type Line::first_chunk() const{
	assert(size() != 0);
	bool space = text[0].isspace();
	line_size i;
	for (i = 0; (text[i].isspace() == space) and (i < size()); i++);
	return get_text(0, i);
}




/********
 * OTHER
 ********/

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

std::ostream& print_all(std::ostream& os, const Line& ln){
	for (auto fchar: ln.get_text()){
		print_all(os, fchar);
		os << std::endl;
	}
	return os;
}

