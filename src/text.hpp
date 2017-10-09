#ifndef TEXT_HPP
#define TEXT_HPP

#include <cctype>
#include <iostream>
#include <vector>

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

/*
inline std::ostream& operator<<(std::ostream& os, const TextStyle& ts){
	return (os << static_cast<int>(ts));
}
*/




// Struct that defines a formatted character, a character with styling applied.
struct fchar {
	int character;
	TextStyle style;

	// Returns true if the character is any sort of whitespace, including newlines and tabs.
	inline bool isspace() const {
		return std::isspace(character);
	}

	inline bool operator==(const fchar f) const {
		return ((f.character == character) && (f.style == style));
	}

	// Constructors (no default constructors)
	fchar (int c): character(c), style(TextStyle::none){}
	fchar (int c, TextStyle t): character(c), style(t){}
};




// Used in place of strings to store formatted text.
typedef std::vector<fchar> text_type;
// Guaranteed to be big enough to hold the size of our formatted "string" vectors. Used to hold index of fchar within text.
typedef std::vector<fchar>::size_type text_index;

// Conversions between the text_type and std::string / c-style strings
text_type string_to_text_type(const std::string &str);
text_type string_to_text_type(const char *str);
std::string text_type_to_string(const text_type &t);

// Doesn't display their rich text forms.
std::ostream& operator<<(std::ostream& os, const fchar fch);
std::ostream& operator<<(std::ostream& os, const text_type t);

#endif
