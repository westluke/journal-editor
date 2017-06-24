#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
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


struct fchar {
	char ch;
	Format ft = Format::none;

	inline bool isspace() {
		return isspace(static_cast <int>(ch));
	}
};

// For popping from the end of a line, all actions are basically equivalent.
// But for moving from the beginning of a line to the end of the previous, that's different.
// I will allow a calling function to define this Line as a first line. That seems like the best way to protect it.
// what if I had functions that took another Line object and passed in some text to those objects? That might work better.
// After all, that's the only thing that prepend_text, append_text, break_line, and get_beginning_text should be used for.
// Using them for anything else is misleading at best.
// We could add other functions when we need to that operate using the underlying text as they should, like inserting strings.
// But those should never be conflated with the functions that use extra state, like break_line.
// break_line is the kind of function that is used only for cosmetic purposes. It should never ever really change the structure of a paragraph.
// So yeah, lets make those functions.
class Line {
	public:
		typedef std::vector<fchar> text_type;
		typedef std::vector<fchar>::size_type index_type;

		index_type line_length();
		bool exceeds_width_non_whitespace(index_type line_width);

		// Changes the state of both lines, but preserves all characters between them.
		void relieve_excess(Line ln, index_type line_width);
		void accept_flowback(Line ln, index_type line_width);

		void insert_ch(index_type i, fchar ch);
		fchar delete_ch(index_type i);

	private:
		text_type text;
};

#endif
