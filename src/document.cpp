#include "document.hpp"
#include "debug_stream.hpp"

Document::Document(Paragraph b): base(b){
	paragraphs.push_back(base);
}

document_size Document::size(){
	return paragraphs.size();
}

fchar Document::get_fchar(DOCLOC dloc){
	return paragraphs[dloc.paragraph].get_fchar(dloc.ploc);
}

void Document::insert_fchar(DOCLOC docloc, fchar fch){
	paragraphs[docloc.paragraph].insert_fchar(docloc.ploc, fch);
}

void Document::replace_fchar(DOCLOC docloc, fchar fch){
	paragraphs[docloc.paragraph].replace_fchar(docloc.ploc, fch);
}

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

// ok my code is broken in multiple ways. It's not detecting the fact that there are multiple cursors, and its not splitting lines correctly.
// ah ok i see two problems. I think ncurses display of paragraphs is broken, and also line splitting isn't taking characters out of the first line.
// Also, it freezes when you introduce whitespace.
// I think I need to fix the paragraph display first.
// Let's test it! With custom documents.
// Ok, so now we know we're not properly removing the cursor
void Document::wrap(){
	for (document_size pi = 0; pi < size(); pi++){
		bool size_changed = paragraphs[pi].wrap();
		//print_all(debug, paragraph);


	//	debug << "yaa" << std::endl;
			// now what? What do I adjust? the next parargaph, or all future paragraphs?
			// Which is better?
			// If I just adjust the next paragraph, then I have to be calculating a new size every time, right?
			// NO. The most efficient option is this.
			// Figure out how much the size changed. Keep that number. Add to it every time a paragraph size changes.
			// But is that stable? idk. Don't do any of this yet actually. Just get wrapping.
		// NOW, I need to adjust paragraph start_lines.
		// What's the best way to do that? Well, I don't want to do this unless I have to.
		// So, I'll start only on the first wrapped paragraph. Or the first paragraph whose size changed?
		// The first paragraph whose size changed. So that will mean passing something in to paragraphs to check that.
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
	x = dloc.ploc.character;
	y = paragraphs[dloc.paragraph].start_line + dloc.ploc.line;
}

// Ahhh but we don't just assign cursors to characters. We actually have to specify before or after.
void Document::assign_cursor_to_character(DOCLOC dloc, TextStyle ts){
	assert(((ts == TextStyle::cursor_before) or (ts == TextStyle::cursor_after)));
	fchar fch = get_fchar(dloc);
	assert((!(fch.style & TextStyle::owns_cursor)));
	replace_fchar(dloc, fchar(fch.character, fch.style | ts));
	Paragraph paragraph = get_paragraph(dloc.paragraph);
	assert((!paragraph.has_cursor()));
	paragraphs[dloc.paragraph].give_cursor();
}

void Document::assign_cursor_to_paragraph(document_size pi){
	Paragraph paragraph = get_paragraph(pi);
	assert((paragraph.empty() and !paragraph.has_cursor()));
	paragraphs[pi].give_cursor();
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
