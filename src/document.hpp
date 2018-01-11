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




class Document{
	public:
		Document(Paragraph b, line_size lw);


		// Determines what paragraph gets cloned whenever a newline is added.
		//void set_base_paragraph(Paragraph p);
		document_size size() const;

		fchar get_fchar(DOCLOC dloc);

		// Two options. The fchar can be inserted directly, with no changes to style.
		// Or the fchar's style can be combined with the previous style in the paragraph.
		void insert_fchar(DOCLOC docloc, fchar fch);
		//void insert_fchar_combine_style(DOCLOC docloc, fchar fch);
		void replace_fchar(DOCLOC docloc, fchar fch);

		//void insert_paragraph(document_size pi, Paragraph paragraph);
		//void delete_paragraph(document_size pi);
		//void replace_paragraph(document_size pi, Paragraph paragraph);

		void wrap();

		DOCLOC get_DOCLOC(int y, int x) const;
		void get_cursor(DOCLOC dloc, int &y, int &x) const;

		bool next_DOCLOC(DOCLOC initial, DOCLOC &target);
		bool previous_DOCLOC(DOCLOC initial, DOCLOC &target);
		/*
		bool next_line(DOCLOC initial, DOCLOC &target);
		bool previous_line(DOCLOC initial, DOCLOC &target);
		*/

		void assign_cursor_to_character(DOCLOC dloc);
		void assign_cursor_to_paragraph(document_size pi);

		void take_cursor_from_character(DOCLOC dloc);
		void take_cursor_from_paragraph(document_size pi);

		// Careful with these guys. The object might be destroyed while you hold a reference, if you use them wrong.
		const std::vector<Paragraph>& get_paragraphs() const;
		const Paragraph& get_paragraph(document_size pi) const;

		line_size get_line_width() const;

	private:
		std::vector<Paragraph> paragraphs;
		line_size line_width;

		Paragraph base;
		DOCLOC cursor;
};

std::ostream& print_all(std::ostream& os, const Document& doc);

#endif
