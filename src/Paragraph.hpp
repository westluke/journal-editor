#ifndef PARAGRAPH_HPP
#define PARAGRAPH_HPP

#include "line.hpp"
#include <initializer_list>

// Used to define font size of paragraph.
enum class HeaderLevel {none, h1, h2, h3};

// Defines a location within a paragraph.
struct p_index {
	std::vector<Line>::size_type line_no;
	Line::index_type ch_index;
};

// The real fundamental unit of text. Composed of Lines, but those Lines may pass text between each other.
// The text of a Paragraph is unchanged without explicit user action.
// Paragraphs created with each ENTER character typed.
// May be empty.
// What does empty paragrpaph mean? one line, empty? or no lines?
// Pretty sure it should be one line, empty.
// Because every line corresponds to a line on the xcreen.
class Paragraph{
	public:
		void insert_ch(p_index i, int ch);
		char replace_ch(p_index i, int ch);
		fchar delete_ch(p_index i);

		bool distribute();
		bool valid();

		void set_header_level(HeaderLevel hl);
		bool apply_format(p_index start, p_index end, TextStyle f);

		void set_line_width(Line::index_type lw);

		fchar get_ch(p_index i);
		std::vector<Line> get_lines();

		// Constructor
		Paragraph(Line::index_type lw);
		Paragraph(std::initializer_list<Line> il, Line::index_type lw);
		Paragraph(std::initializer_list<char*>, Line::index_type lw);

	private:
		Line::index_type line_width;
		std::vector<Line> lines;
		HeaderLevel h_level;
		TextStyle initial_style;

		p_index previous_index(p_index i);
		p_index next_index(p_index i);
};

#endif
