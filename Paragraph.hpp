#ifndef PARAGRAPH_HPP
#define PARAGRAPH_HPP

#include <vector>
#include <string>
#include <cctype>

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

	inline bool isspace() {
		return isspace(static_cast <int>(ch));
	}
};

class Line {
	public:
		typedef std::vector<fchar> text_type;
		typedef std::vector<fchar>::size_type index_type;

		// The length of the line, not the number of characters contained in this object.
		// Therefore doesn't include post_space.
		index_type line_length();

		// We pop words to get a Line under a line width limit, therefore there is always a minimum number of characters to be removed.
		// This means that if there is no whitespace in the line, which is possible, we can just cut a chunk off the block of text.
		text_type break_line(index_type line_width);

		void prepend_text(text_type words);

		void insert_ch(index_type i, char ch);
		fchar delete_ch(index_type i);
	
	private:
		text_type text;

};

struct p_index {
	int line_no;
	Line::index_type index;
};

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
