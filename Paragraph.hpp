#ifndef PARAGRAPH_HPP
#define PARAGRAPH_HPP

#include "Line.hpp"

enum class HeaderLevel {none, h1, h2, h3};

struct p_index {
	std::vector<Line>::size_type line_no;
	Line::index_type ch_index;
};

class Paragraph{
	public:
		void insert_ch(p_index i, char ch);
		char delete_ch(p_index i);
		fchar get_ch(p_index i);
		p_index previous_index(p_index i);

		bool apply_format(p_index start, p_index end, Format f);

		std::vector<Line::text_type> get_lines();

		void set_line_width(Line::index_type lw);

		void set_header_level(HeaderLevel hl);

		// Constructor
		Paragraph(Line::index_type lw);

	private:
		Line::index_type line_width;
		std::vector<Line> lines;
		HeaderLevel h_level;
		Format next_format;
};

#endif
