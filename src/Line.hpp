#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <cctype>
#include <string>
#include <cassert>
#include <iostream>




// Simple enum that defines the styling applied to a particular character.
enum class TextStyle: unsigned {
	none = 0x00,
	bold = 0x01,
	italic = 0x02,
	underline = 0x04,
	strikethrough = 0x08,
	link = 0x10
};

// Bitwise OR
inline TextStyle operator|(const TextStyle a, const TextStyle b){
	return static_cast<TextStyle>(static_cast<char>(a) | static_cast<char>(b));
}

// Bitwise AND
inline TextStyle operator&(const TextStyle a, const TextStyle b){
	return static_cast<TextStyle>(static_cast<char>(a) & static_cast<char>(b));
}

// Bitwise XOR
inline TextStyle operator^(const TextStyle a, const TextStyle b){
	return static_cast<TextStyle>(static_cast<char>(a) ^ static_cast<char>(b));
}

// Bitwise NOT
inline TextStyle operator~(const TextStyle a){
	return static_cast<TextStyle>(~static_cast<char>(a));
}

std::ostream& operator<<(std::ostream& os, const TextStyle& ts);





// Struct that defines a formatted character, a character with styling applied.
struct fchar {
	int character;
	TextStyle style;

	// Returns true if the character is any sort of whitespace, including newlines and tabs.
	inline bool isspace() {
		return std::isspace(character);
	}

	inline bool operator==(const fchar f) const {
		return ((f.character == character) && (f.style == style));
	}

	// Constructors (no default constructors)
	fchar (int c): character(c), style(TextStyle::none){}
	fchar (int c, TextStyle t): character(c), style(t){}
};





// An object that represents a line of text on the display. It stores fchars to permit styling.
// It can shuffle text with neighboring lines to make text wrap.
// It has a variety of modification methods. ANY sort of modification marks it as changed, so the display can be updated accordingly.
class Line {
	public:
		// Used in place of strings to store formatted text.
		typedef std::vector<fchar> text_type;
		// Guaranteed to be big enough to hold the size of our formatted "string" vectors. Used to hold index of fchar within text.
		typedef std::vector<fchar>::size_type index_type;

		// The length of the line, including whitespace.
		index_type size();
		// Whether or not the line exceeds this width, not counting whitespace at the end.
		bool exceeds_width_non_whitespace(index_type line_width);

		// Simple text modification methods
		void insert_ch(index_type i, fchar ch);
		fchar delete_ch(index_type i);
		fchar replace_ch(index_type i, fchar ch);

		void insert_str(index_type i, std::string str, TextStyle style = TextStyle::none);
		std::string delete_str(index_type i, index_type length);
		std::string replace_str(index_type i, std::string str, TextStyle style = TextStyle::none);

		void insert_text(index_type i, text_type txt);
		text_type delete_text(index_type i, index_type length);
		text_type replace_text(index_type i, text_type txt);

		// Style modification methods
		void change_ch_style(index_type i, TextStyle style);
		void change_style(index_type start, TextStyle style);
		void change_style(index_type start, index_type end, TextStyle style);

		// Simple text retrieval methods
		fchar get_ch(index_type i);

		text_type get_text();
		text_type get_text(index_type start);
		text_type get_text(index_type start, index_type end);

		std::string get_string() const;
		std::string get_string(index_type start) const;
		std::string get_string(index_type start, index_type end) const;

		// Conversions between the string type used internally and std::string / c-style strings
		static text_type string_to_text_type(const std::string &str);
		static text_type string_to_text_type(const char *str);
		static std::string text_type_to_string(const text_type &t);

		// Constructors
		Line(const text_type &t);
		Line(const std::string &str);
		Line(const char *str);
		Line();

		// Equality operator
		bool operator==(const Line &ln) const;

		// These methods deal with the changed variable, which informs other code about whether this Line
		// was changed since the last display update and refresh.
		bool was_changed();	// Returns the private changed variable.
		void clear_changes();
		void mark_changed();

		// Either move excess text from this line to the one specified, or flow whitespace/text back.
		// In the end, the first line is guaranteed to not exceed line_width, and the second line is guaranteed to have
		// no preceding whitespace. Really a wrapper for relieve_excess and accept_flowback.
		bool equalize(Line &ln, index_type line_width);

		// Changes the state of both lines, but preserves all characters between them. Used to shuffle text between lines
		// for the purpose of text wrapping.
		bool relieve_excess(Line &ln, index_type line_width);
		bool accept_flowback(Line &ln, index_type line_width);

	private:
		text_type text;
		bool changed;
};

std::ostream& operator<<(std::ostream& os, const Line& ln);
std::ostream& operator<<(std::ostream& os, const std::vector<Line>& vol);

#endif
