#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <cctype>
#include <string>
#include <cassert>

enum class TextStyle: unsigned {
	none = 0x00,
	bold = 0x01,
	italic = 0x02,
	underline = 0x04,
	strikethrough = 0x08,
	link = 0x10
};

void print (TextStyle a);

// Bitwise OR
inline TextStyle operator|(TextStyle a, TextStyle b){
	return static_cast<TextStyle>(static_cast<char>(a) | static_cast<char>(b));
}

// Bitwise AND
inline TextStyle operator&(TextStyle a, TextStyle b){
	return static_cast<TextStyle>(static_cast<char>(a) & static_cast<char>(b));
}

// Bitwise XOR
inline TextStyle operator^(TextStyle a, TextStyle b){
	return static_cast<TextStyle>(static_cast<char>(a) ^ static_cast<char>(b));
}

// Bitwise NOT
inline TextStyle operator~(TextStyle a){
	return static_cast<TextStyle>(~static_cast<char>(a));
}




struct fchar {
	int character;
	TextStyle style;

	inline bool isspace() {
		return std::isspace(character);
	}

	inline bool operator==(const fchar f) const {
		return ((f.character == character) && (f.style == style));
	}

	fchar (int c): character(c), style(TextStyle::none){}
	fchar (int c, TextStyle t): character(c), style(t){}
};




class Line {
	public:
		// Used in place of strings to store formatted text.
		typedef std::vector<fchar> text_type;
		// Guaranteed to be big enough to hold the size of our formatted "string" vectors.
		typedef std::vector<fchar>::size_type index_type;

		// The length of the line, including whitespace.
		index_type line_length();
		// Whether or not the line exceeds this width, not counting whitespace at the end.
		bool exceeds_width_non_whitespace(index_type line_width);

		// Either move excess text from this line to the one specified, or flow whitespace/text back.
		// In the end, the first line is guaranteed to not exceed line_width, and the second line is guaranteed to have
		// no preceding whitespace.
		bool equalize(Line &ln, index_type line_width);

		// Changes the state of both lines, but preserves all characters between them.
		bool relieve_excess(Line &ln, index_type line_width);
		bool accept_flowback(Line &ln, index_type line_width);

		void insert_ch(index_type i, fchar ch);
		fchar delete_ch(index_type i);

		static text_type string_to_text_type(const std::string &str);
		static text_type string_to_text_type(const char *str);
		static std::string text_type_to_string(const text_type &t);

		fchar get_ch(index_type);
		text_type get_text();
		std::string get_string();

		Line(const text_type &t);
		Line(const std::string &str);
		Line(const char *str);
		Line();

		bool operator==(const Line &ln) const;
	private:
		text_type text;
};

#endif
