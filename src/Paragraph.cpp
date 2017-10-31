#include "paragraph.hpp"

// #define NDEBUG
#include <cassert>

// CHECK CODE!
// Replace i with loc
//
// Then start building up main so it actually works!
// start building the updater functino base.

line_number Paragraph::size() const{
	return lines.size();
}

bool Paragraph::empty() {
	assert(valid());
	return (lines.size() == 1 and lines[0].size() == 0);
}

// Lines mark themselves changed.
//
// SEGFAULT. GREAT.

// FIX THIS SO IT CAN BE CONST
bool Paragraph::valid(){
	if (lines.size() <= 0) return false;
	for (int i = 0; i < lines.size(); i++){
		if (lines[i].size() == 0 && i != 0) return false;
		if (lines[i].exceeds_width_non_whitespace(line_width)) return false;
		if (i > 0 && lines[i-1].equalize(lines[i], line_width)) return false;
	}
	return true;
}

bool Paragraph::distribute(){
	bool changed = false;

	// Go through each line. If a line is empty, remove it.
	// Otherwise, equalize that line with the following line.
	for (int i = 0; i < lines.size(); i++){

		// Remove a line if its empty, but not if that line is the only line.
		if (lines[i].size() == 0 && lines.size() > 1){
			lines.erase(lines.begin() + i);
			i--;
			changed = true;
			continue;
		}

		if (i < lines.size() - 1){
			changed = lines[i].equalize(lines[i+1], line_width) || changed;
		}
	}

	// Now, the last line could have too much text. Keep equalizing and adding lines until we have enough space.
	while (lines[lines.size()-1].exceeds_width_non_whitespace(line_width)){
		lines.push_back(Line());
		lines[lines.size()-2].equalize(lines[lines.size()-1], line_width);
		changed = true;
	}

	assert((valid()));
	mark_changed();
	return changed;
}

void Paragraph::insert_ch(PLOC loc, int ch){
	assert((valid()));

	fchar fch = fchar(ch);

	// Verify that the PLOC is valid.
	assert((loc.n < lines.size() && loc.ind <= lines[loc.n].size()));

	if (lines.at(0).size() == 0){
		fch.style = initial_style;
	} else if (!(loc.n == 0 && loc.ind == 0)){
		fch.style = get_ch(previous_index(loc)).style;
	} else {
		fch.style = get_ch(loc).style;
	}

	lines.at(loc.n).insert_ch(loc.ind, fch);
	distribute();

	assert((valid()));
	mark_changed();
}

fchar Paragraph::delete_ch(PLOC loc){
	assert((valid()));
	assert((loc.n < lines.size() && loc.ind < lines[loc.n].size()));

	fchar ch = lines.at(loc.n).delete_ch(loc.ind);
	distribute();

	assert((valid()));
	return ch;
	mark_changed();
}

void Paragraph::append_ch(int ch){
	PLOC loc;
	loc.n = size() - 1;
	loc.ind = lines[loc.n].size();

	insert_ch(loc, ch);
	mark_changed();
}

void Paragraph::delete_last_ch(){
	assert(!empty());
	PLOC loc;
	loc.n = lines.size() - 1;
	loc.ind = lines[loc.n].size() - 1;
	delete_ch(loc);
	mark_changed();
}

// MAKE THIS ACTUALLY DO SHIT
void Paragraph::set_line_width(text_index lw){
	assert(lw != 0);
	line_width = lw;
	distribute();
	mark_changed();
}

fchar Paragraph::get_ch(PLOC loc){
	return lines.at(loc.n).get_ch(loc.ind);
}

std::vector<text_type> Paragraph::get_lines(){
	std::vector<text_type> ret;

	for (auto ln: lines) ret.push_back(ln.get_text());
	return ret;
}

PLOC Paragraph::last_index() const{
	PLOC loc;
	loc.n = size() - 1;
	loc.ind = lines[loc.n].size();
	return loc;
}

Paragraph::Paragraph(text_index lw): line_width(lw){
	lines.push_back(Line());
	mark_changed();
}

Paragraph::Paragraph(std::initializer_list<Line> il, text_index lw): line_width(lw), lines(il){
	mark_changed();
}

Paragraph::Paragraph(std::initializer_list<char*> il, text_index lw): line_width(lw){
	for (auto iter = il.begin(); iter != il.end(); iter++){
		lines.push_back(Line(*iter));
	}
	mark_changed();
}

bool Paragraph::was_changed(){
	return changed;
}

void Paragraph::_clear_changed_flag(){
	changed = false;
}

void Paragraph::mark_changed(){
	changed = true;
}

// This function doesn't check that the PLOC is valid.
// If the index is {0, 0}, the function returns {0, 0}.
// Can later change this to use bracket operators instead of .at()
PLOC Paragraph::previous_index(PLOC loc){
	assert(loc.n < lines.size());
	assert(loc.ind <= lines[loc.n].size());
	assert(!(loc.n == 0 && loc.ind == 0));

	if (loc.ind){
		loc.ind--;
	} else if (loc.n){
		loc.n--;
		loc.ind = lines.at(loc.n).size() - 1;
	} 

	// If i points to the first character (and there is no previous index) it gets returned unchanged.
	// But ideally this shouldn't happen, which is why we have an assert that checks for it.
	return loc;
}

