#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <cctype>
#include <string>
#include <cassert>
#include <iostream>

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

// The vector operations don't throw their own errors, so I have to do that myself!
// A Line can be empty.
//
// functions to add:
// overloaded insert_ch for character
// change_style functions
//
class Line {
	public:
		typedef std::vector<fchar> text_type;
		typedef std::vector<fchar>::size_type index_type;

		index_type line_length();
		bool exceeds_width_non_whitespace(index_type line_width);

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

	private:
		text_type text;
};

#endif
