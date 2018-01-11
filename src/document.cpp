#include "document.hpp"
#include "debug_stream.hpp"

Document::Document(Paragraph b, line_size lw): line_width(lw), base(b){
	paragraphs.push_back(base);
}

document_size Document::size() const{
	return paragraphs.size();
}

fchar Document::get_fchar(DOCLOC dloc){
	return paragraphs[dloc.paragraph].get_fchar(dloc.ploc());
}

//
void Document::insert_fchar(DOCLOC docloc, fchar fch){
	paragraphs[docloc.paragraph].insert_fchar(docloc.ploc(), fch);
}

void Document::replace_fchar(DOCLOC docloc, fchar fch){
	paragraphs[docloc.paragraph].replace_fchar(docloc.ploc(), fch);
}

/*
void Document::insert_paragraph(document_size pi, Paragraph paragraph){
	paragraphs.insert(paragraphs.begin() + pi, paragraph);
}

void Document::delete_paragraph(document_size pi){
	paragraphs.erase(paragraphs.begin() + pi);
}

void Document::replace_paragraph(document_size pi, Paragraph paragraph){
	paragraphs.erase(paragraphs.begin() + pi);
	paragraphs.insert(paragraphs.begin() + pi, paragraph);
}
*/

void Document::wrap(){
	for (document_size pi = 0; pi < size(); pi++){
		paragraphs[pi].wrap(line_width);
	}
}

DOCLOC Document::get_DOCLOC(int y, int x) const{
	for (int i = 0; i < paragraphs.size(); i++){
		if (paragraphs[i].start_line <= y){
			return DOCLOC(i, y - paragraphs[i].start_line, x);
		}
	}

	return DOCLOC(paragraphs.size() - 1, y - paragraphs[paragraphs.size() - 1].start_line, x);
}

void Document::get_cursor(DOCLOC dloc, int &y, int &x) const{
	x = dloc.character;
	y = paragraphs[dloc.paragraph].start_line + dloc.line;
}

// The cursor CAN be at the end of the paragraph, but NOT at the end of a line.
// How do I represent that? Better question, WHEN CAN I DO THAT?
bool Document::next_DOCLOC(DOCLOC initial, DOCLOC &target){
	Paragraph paragraph = paragraphs[initial.paragraph];
	Line line = paragraph.get_line(initial.line);

	if (	((initial.character + 1) < line.size()) or 
		(((initial.character + 1) == line.size()) and ((initial.line + 1) == paragraph.size()))){
		target = DOCLOC(initial.paragraph, initial.line, initial.character + 1);
		return true;
	} else if ((initial.line + 1) < paragraphs[initial.paragraph].size()) {
		target = DOCLOC(initial.paragraph, initial.line + 1, 0);
		return true;
	} else if ((initial.paragraph + 1) < size()){
		target = DOCLOC(initial.paragraph + 1, 0, 0);
		return true;
	}

	return false;
}

bool Document::previous_DOCLOC(DOCLOC initial, DOCLOC &target){
	if (initial.character > 0){
		target = DOCLOC(initial.paragraph, initial.line, initial.character - 1);
		return true;
	} else if (initial.line > 0) {
		Line prev_line = paragraphs[initial.paragraph].get_line(initial.line - 1);
		target = DOCLOC(initial.paragraph, initial.line - 1, prev_line.size() - 1);
		return true;
	} else if (initial.paragraph > 0){
		Paragraph prev_paragraph = paragraphs[initial.paragraph - 1];
		Line last_line = prev_paragraph.get_line(prev_paragraph.size() - 1);
		target = DOCLOC(initial.paragraph - 1, prev_paragraph.size() - 1, last_line.size());
		return true;
	}

	return false;
}

/*
bool Document::next_line(DOCLOC initial, DOCLOC &target);
bool Document::previous_line(DOCLOC initial, DOCLOC &target);
*/

// We only use cursor_after.
void Document::assign_cursor_to_character(DOCLOC dloc){
	fchar fch = get_fchar(dloc);
	assert((!(fch.style & TextStyle::cursor_after)));
	replace_fchar(dloc, fchar(fch.character, fch.style | TextStyle::cursor_after));
}

void Document::assign_cursor_to_paragraph(document_size pi){
	Paragraph paragraph = get_paragraph(pi);
	assert(!paragraph.has_cursor());
	paragraphs[pi].give_cursor();
}

void Document::take_cursor_from_character(DOCLOC dloc){
	fchar ch = get_fchar(dloc);
	assert((ch.style & TextStyle::cursor_after));
	replace_fchar(dloc, fchar(ch.character, (ch.style & ~TextStyle::cursor_after)));
}

void Document::take_cursor_from_paragraph(document_size pi){
	assert((paragraphs[pi].has_cursor()));
	paragraphs[pi].remove_cursor();
}

// Careful with these guys. The object might be destroyed while you hold a reference, if you use them wrong.
const std::vector<Paragraph>& Document::get_paragraphs() const{
	return paragraphs;
}

const Paragraph& Document::get_paragraph(document_size pi) const{
	return paragraphs[pi];
}

std::ostream& print_all(std::ostream& os, const Document& doc){
	for (auto paragraph: doc.get_paragraphs()) print_all(os, paragraph);
	return os;
}

line_size Document::get_line_width() const{
	return line_width;
}
