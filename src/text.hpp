#ifndef TEXT_HPP
#define TEXT_HPP

#include <cctype>
#include <iostream>
#include <vector>
#include <type_traits>

// OH I understand the problem. The printing function is inline. A lot of this shit is inline.
// That means when we update this file, text.cpp is recompiled. But main.cpp isn't getting info about TextStyle from text.o!
// It's getting it from text.hpp. That means main has to be recompiled as well.
// Everything that depends on text.hpp has to be recompiled everytime text.hpp is recompiled.

class TextStyle {
	public:
	static const TextStyle none;
	static const TextStyle bold;
	static const TextStyle italic;
	static const TextStyle underline;
	static const TextStyle strikethrough;
	static const TextStyle link;
	static const TextStyle cursor_after;
	static const TextStyle cursor_before;

	inline TextStyle operator|(const TextStyle ts) const{
		return TextStyle(u | ts.u);
	}

	inline TextStyle operator&(const TextStyle ts) const{
		return TextStyle(u & ts.u);
	}

	inline TextStyle operator^(const TextStyle ts) const{
		return TextStyle(u ^ ts.u);
	}

	inline TextStyle operator~() const{
		return TextStyle(~u);
	}

	inline bool operator==(const TextStyle& ts) const{
		return (u == ts.u);
	}

	inline operator bool() const{
		return !(u == none.u);
	}

	friend std::ostream& operator<<(std::ostream& os, const TextStyle &ts);

	private:
	unsigned u;

	inline TextStyle(unsigned x){
		u = x;
	}
};

inline std::ostream& operator<<(std::ostream& os, const TextStyle &ts){
	return (os << ts.u);
}



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
typedef std::vector<fchar>::size_type line_size;

static_assert(std::is_unsigned<line_size>::value, "Signed size type detected");

// Conversions between the text_type and std::string / c-style strings
text_type string_to_text_type(const std::string &str);
text_type string_to_text_type(const char *str);
std::string text_type_to_string(const text_type &t);

// Doesn't display their rich text forms.
std::ostream& operator<<(std::ostream& os, const fchar fch);
std::ostream& operator<<(std::ostream& os, const text_type t);
std::ostream& operator<<(std::ostream& os, const std::vector<const text_type> ts);

// Comparison operators between strings and text_type make debugging easier.
bool operator==(const text_type &txt, const std::string &str);
bool operator==(const std::string &str, const text_type &txt);

#endif
