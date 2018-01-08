#include "paragraph.hpp"
#include "debug_stream.hpp"

// #define NDEBUG
#include <cassert>

Paragraph::Paragraph(paragraph_size line_no, line_size lw): start_line(line_no), line_width(lw), cursor(false){
	lines.push_back(Line());
}

Paragraph::Paragraph(const Paragraph &p): start_line(p.start_line), line_width(p.line_width), lines(p.lines), cursor(p.cursor) {}

//Paragraph(std::initializer_list<char*> il, line_size lw){
//}

paragraph_size Paragraph::size() const{
	return lines.size();
}


bool Paragraph::empty() const{
	return size() == 0;
}



fchar Paragraph::get_fchar(PLOC ploc) const{
	return lines[ploc.line].get_fchar(ploc.character);
}



void Paragraph::insert_fchar(PLOC ploc, fchar fch){
	lines[ploc.line].insert_ch(ploc.character, fch);
}

void Paragraph::replace_fchar(PLOC ploc, fchar fch){
	lines[ploc.line].replace_ch(ploc.character, fch);
}

const std::vector<Line>& Paragraph::get_lines() const{
	return lines;
}

const Line& Paragraph::get_line(paragraph_size li) const{
	return lines[li];
}

// Make this as simple as possible. How might you do this? Overflow every line into the next?
// No, equalize every pair of lines.
// Ah yes, its cuz there is only one line. Doesn't deal with overflow.
// How to do that? Could just add an extra line at the end, until that line is empty after equalization.
// OR, I add a function to Line that makes this easier. Overflow to empty.
// it changes the original line, and returns a new line that can be apended.
// NICE. Now let's figure out why it freezes with whitespace
// its only after a line thats composed entirely of characters, interesting.
// first find where
bool Paragraph::wrap(){
	bool ret = false;

	debug << "\ec";
	debug << "ENTERED WRAP" << std::endl;
	if (!empty()){
		for (paragraph_size li = 0; li < size() - 1; li++){
			debug << "LINE " << li << ":" << lines[li] << std::endl;
			lines[li].equalize(lines[li + 1], line_width);
			debug << "COMPLETED EQUALIZATION" << std::endl;
		}

		// What is the while condition here? While the last line exceeds width.
		// Adds lines forever, that's why.
		// Probably because the size is changing, but it will be cached.
		// Or because im adding lines wrong.
		// So here's when it breaks, when the line is entirely characters, plus one space
		// But it also makes no sense, cuz these were equalized to put the space on one line. It's bizarre.
		while (get_line(size() - 1).exceeds_width_non_whitespace(line_width)){
			ret = true;
			Line &last_line = lines[size() - 1];
			debug << "LAST LINE:" << last_line << "|" << std::endl;
			Line overflow = last_line.overflow(line_width);
			lines.push_back(overflow);
			// huh, the overflow is the entire first line. Well that makes no sensep.
			// nope, the overflow is the line sum of both lines. that makes less sense.
			// What does it mean? sounds like it means the equalization combined them.
			debug << "OVERFLOW:" << overflow << "|" << std::endl;
			debug << "ADDED LINE" << std::endl;
		}
	}

	return ret;
}

bool Paragraph::has_cursor() const{
	return cursor;
}

void Paragraph::give_cursor(){
	cursor = true;
}

void Paragraph::remove_cursor(){
	cursor = false;
}

std::ostream& print_all(std::ostream& os, const Paragraph& paragraph){
	if (paragraph.has_cursor()) os << "Has cursor" << std::endl;
	for (paragraph_size li = 0; li < paragraph.size(); li++){
		os << "Line: " << li << std::endl;
		print_all(os, paragraph.get_line(li));
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Paragraph& paragraph){
	for (paragraph_size li = 0; li < paragraph.size(); li++){
		os << "Line: " << li << std::endl;
		os << paragraph.get_line(li) << std::endl;
	}
	return os;
}
