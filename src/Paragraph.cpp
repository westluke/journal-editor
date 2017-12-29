#include "paragraph.hpp"

// #define NDEBUG
#include <cassert>

// CHECK CODE!
// Replace i with loc
//
// Then start building up main so it actually works!
// start building the updater functino base.

Paragraph::Paragraph(paragraph_size line_no, line_size lw): start_line(line_no), line_width(lw){
	lines.push_back(Line());
}


paragraph_size Paragraph::size() const{
	return lines.size();
}

bool Paragraph::empty() {
	return size() == 0;
}

void Paragraph::append_ch(int ch){
	lines[size() - 1].append_ch(ch);
}

void Paragraph::delete_last_ch(){
	lines[size() - 1].delete_last_ch();
}

void Paragraph::insert_ch(PLOC ploc, int ch){
	lines[ploc.line].insert_ch(ploc.ch, fchar(ch));
}

void Paragraph::insert_fchar(PLOC ploc, fchar fch){
	lines[ploc.line].insert_ch(ploc.ch, fch);
}

void Paragraph::replace_fchar(PLOC ploc, fchar fch){
	lines[ploc.line].replace_ch(ploc.ch, fch);
}

std::vector<Line> Paragraph::get_lines(){
	return lines;
}

bool Paragraph::owns_cursor(){
	if (cursor){
		assert(size() == 0);
		return true;
	} return false;
}

void Paragraph::owns_cursor(bool b){
	if (b){
		assert(size() == 0);
		cursor = true;
	} cursor = false;
}
