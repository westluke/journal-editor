#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "paragraph.hpp"
#include "line.hpp"
#include "text.hpp"

struct DOCLOC {
	inline DOCLOC(document_size p, paragraph_size l, line_size ch): paragraph(p), line(l), character(ch) {}
	inline DOCLOC(): DOCLOC(0, 0, 0) {}
	inline PLOC ploc(){ return PLOC(line, character); }
	document_size paragraph;
	paragraph_size line;
	line_size character;
};

// What am I looking for here? I want the paragraphs at a particular moment I think.
// But I don't want any copying, and I don't want any access. I just want to read it.

// Here's the thing. I can get read access without copying. But if I do, i have no way of preventing it from changing beneath me.
class Document{
	public:
		Document(Paragraph b);

		// Determines what paragraph gets cloned whenever a newline is added.
		void set_base_paragraph(Paragraph p);
		document_size size();

		fchar get_fchar(DOCLOC dloc);

		// Two options. The fchar can be inserted directly, with no changes to style.
		// Or the fchar's style can be combined with the previous style in the paragraph.
		void insert_fchar(DOCLOC docloc, fchar fch);
		void insert_fchar_combine_style(DOCLOC docloc, fchar fch);
		void replace_fchar(DOCLOC docloc, fchar fch);

		void insert_paragraph(document_size pi, Paragraph paragraph);
		void delete_paragraph(document_size pi);
		void replace_paragraph(document_size pi, Paragraph paragraph);

		void wrap();

		DOCLOC get_DOCLOC(int y, int x) const;
		void get_cursor(DOCLOC dloc, int &y, int &x) const;

		// Why do I need this? Because I want to assign the cursor to the character and the paragraph in one swoop.
		// Here's a question: should cursor ownership really be a TextStyle? not sure. That's a question for another day.
		void assign_cursor_to_character(DOCLOC dloc, TextStyle ts);
		void assign_cursor_to_paragraph(document_size pi);

		// Careful with these guys. The object might be destroyed while you hold a reference, if you use them wrong.
		const std::vector<Paragraph>& get_paragraphs() const;
		const Paragraph& get_paragraph(document_size pi) const;

	private:
		std::vector<Paragraph> paragraphs;
		Paragraph base;
};

std::ostream& print_all(std::ostream& os, const Document& doc);

#endif
