#ifndef PARAGRAPH_HPP
#define PARAGRAPH_HPP

#include <vector>
#include <string>
#include <cctype>

// class definitions must be unique per translation unit, not per application.
// the language recognizes that classes would be useless without this.
// However, definitions must be identical in all respects.

// so, using a namespace for the bit flags would not be as nice.

// Defines the types of formatting an fchar can use.
enum class Format: char {
	none = 0x00,
	bold = 0x01,
	italic = 0x02,
	underline = 0x04,
	strikethrough = 0x08,
	link = 0x10
};

enum class HeaderLevel {none, h1, h2, h3};

struct fchar {
	char ch;
	Format ft = Format::none;
};

class Line {
	public:
		typedef std::vector<fchar> text_type;
		typedef std::vector<fchar>::size_type index_type;

		// The length of the line, not the number of characters contained in this object.
		// Therefore doesn't include post_space.
		index_t line_length();

		// Uses last_space to remove the last word from the text and remove it.
		text_type pop_word();

		void prepend_word(text_type word);

		void insert_ch(index_type i, char ch);
	
	private:
		text_type text;
		text_type post_space;

		index_type first_space;
		index_type last_space;

		void update_first_and_last_space();
};

struct p_index {
	int line_no;
	Line::index_type index;
}

class Paragraph{
	public:
		void insert_ch(p_index i, char ch);
		char delete_ch(p_index i);

		bool apply_modifier(p_index start, p_index end, Format f);

		std::vector<Line::text_type> get_lines();

		void set_line_width(Line::index_type lw);

		// Constructor
		Paragraph(Line::index_type lw);

	private:
		Line::index_type line_width;
		std::vector<Line> lines;

		void adjust_lines_to_width();
};

#endif
