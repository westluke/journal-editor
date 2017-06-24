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
class Line {
	// throw error if ever someoene tries to violate the preceding whitespace mode.
	public:
		typedef std::vector<fchar> text_type;
		typedef std::vector<fchar>::size_type index_type;

		// The length of the line, not the number of characters contained in this object.
		// Therefore doesn't include post_space.
		// IMPLEMENTED
		index_type line_length();

		// IMPLEMENTED
		bool exceeds_width(index_type line_width);

		// We pop words to get a Line under a line width limit, therefore there is always a minimum number of characters to be removed.
		// This means that if there is no whitespace in the line, which is possible, we can just cut a chunk off the block of text.
		text_type break_line(index_type line_width);

		// Add text to the beginning of the line.
		void prepend_text(text_type words);

		void insert_ch(index_type i, char ch);
		fchar delete_ch(index_type i);

	private:
		text_type text;
};

#endif
