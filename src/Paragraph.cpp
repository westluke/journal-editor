#include "Paragraph.hpp"

// #define NDEBUG
#include <cassert>

//TODO: decide if paragraph should update the changed flags in Lines, or whether the Lines themselves should do so through equalize.
//I could do them inside the Line methods, in which case I can ensure that any time a Line is changed or created it is marked so.
//But that sorta breaks abstraction, and there might be times I want to change them without marking them? In that case I can just clear them afterwards.
//Yeah, let's do that.
bool Paragraph::valid(){
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
			changed = changed || lines[i].equalize(lines[i+1], line_width);
		}
	}

	// Now, the last line could have too much text. Keep equalizing and adding lines until we have enough space.
	while (lines[lines.size()-1].exceeds_width_non_whitespace(line_width)){
		lines.push_back(Line());
		lines[lines.size()-2].equalize(lines[lines.size()-1], line_width);
		changed = true;
	}

	assert((valid()));
	return changed;
}

void Paragraph::insert_ch(p_index i, int ch){

	assert((valid()));

	fchar fch = fchar(ch);

	// Verify that the p_index is valid.
	assert((i.line_no < lines.size() && i.ch_index <= lines[i.line_no].size()));

// It's in here that the vector shit goes bad	
	if (lines.at(0).size() == 0){
		fch.style = initial_style;
	} else if (!(i.line_no == 0 && i.ch_index == 0)){
		fch.style = get_ch(previous_index(i)).style;
	} else {
		fch.style = get_ch(i).style;
	}

	lines.at(i.line_no).insert_ch(i.ch_index, fch);
	distribute();
	assert((valid()));
}

fchar Paragraph::delete_ch(p_index i){
	assert((valid()));
	assert((i.line_no < lines.size() && i.ch_index < lines[i.line_no].size()));

	fchar ch = lines.at(i.line_no).delete_ch(i.ch_index);
	distribute();

	assert((valid()));
	return ch;
}

// It's in here that it goes wrong
fchar Paragraph::get_ch(p_index i){
	return lines.at(i.line_no).get_ch(i.ch_index);
}

// This function doesn't check that the p_index is valid.
// If the index is {0, 0}, the function returns {0, 0}.
// Can later change this to use bracket operators instead of .at()
p_index Paragraph::previous_index(p_index i){
	assert(i.line_no < lines.size());
	assert(i.ch_index < lines[i.line_no].size());
	assert(!(i.line_no == 0 && i.ch_index == 0));

	if (i.ch_index){
		i.ch_index--;
	} else if (i.line_no){
		i.line_no--;
		i.ch_index = lines.at(i.line_no).size() - 1;
	} 

	// If i points to the first character (and there is no previous index) it gets returned unchanged.
	// But ideally this shouldn't happen, which is why we have an assert that checks for it.
	return i;
}

p_index Paragraph::next_index(p_index i){
	assert(i.line_no < lines.size());
	assert(i.ch_index < lines[i.line_no].size());

	i.ch_index++;
	return i;
}

//bool apply_format(p_index start, p_index end, Format f);

std::vector<Line> Paragraph::get_lines(){
	return lines;
}

void Paragraph::set_line_width(Line::index_type lw){
	assert(lw != 0);
	line_width = lw;
}

//void set_header_level(HeaderLevel hl);

Paragraph::Paragraph(Line::index_type lw): line_width(lw){
	lines.push_back(Line());
}

Paragraph::Paragraph(std::initializer_list<Line> il, Line::index_type lw): line_width(lw), lines(il){
	;
}

Paragraph::Paragraph(std::initializer_list<char*> il, Line::index_type lw): line_width(lw){
	for (auto iter = il.begin(); iter != il.end(); iter++){
		lines.push_back(Line(*iter));
	}
}

