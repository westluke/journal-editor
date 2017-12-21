#include "paragraph.hpp"

// #define NDEBUG
#include <cassert>

// CHECK CODE!
// Replace i with loc
//
// Then start building up main so it actually works!
// start building the updater functino base.

Paragraph::Paragraph(text_index lw): line_width(lw){
	lines.push_back(Line());
}


line_number Paragraph::size() const{
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

void insert_ch(PLOC ploc, int ch){
	lines[ploc.pn].insert_ch(ploc.n, ch);
}

std::vector<Line> Paragraph::get_lines(){
	return lines;
}
