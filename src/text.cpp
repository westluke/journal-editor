#include "text.hpp"

const TextStyle TextStyle::none = TextStyle(0x00);
const TextStyle TextStyle::bold = TextStyle(0x01);
const TextStyle TextStyle::italic = TextStyle(0x02);
const TextStyle TextStyle::underline = TextStyle(0x04);
const TextStyle TextStyle::strikethrough = TextStyle(0x08);
const TextStyle TextStyle::link = TextStyle(0x10);
const TextStyle TextStyle::cursor_after = TextStyle(0x20);


// Conversions between the text_type and std::string / c-style strings
text_type string_to_text_type(const std::string &str){
	text_type t;
	for (auto ch : str) t.push_back(fchar(ch));
	return t;
}

text_type string_to_text_type(const char *str){
	std::string new_str = std::string(str);
	return string_to_text_type(new_str);
}

std::string text_type_to_string(const text_type &t){
	std::string str;
	for (auto fch : t) str.push_back(fch.character);
	return str;
}

inline std::ostream& operator<<(std::ostream& os, const TextStyle &ts){
	return (os << ts.u);
}

std::ostream& operator<<(std::ostream& os, const fchar fch){
	return (os << static_cast<char>(fch.character));
}

std::ostream& operator<<(std::ostream& os, const text_type t){
	for (auto fch : t) os << fch;
	os << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<const text_type> ts){
	for (auto t: ts) os << t;
	return os;
}

std::ostream& print_all(std::ostream& os, const fchar fch){
	os << fch << " |";
	TextStyle ts = fch.style;
	if (ts & TextStyle::bold) os << " Bold";
	if (ts & TextStyle::italic) os << " Italic";
	if (ts & TextStyle::underline) os << " Underline";
	if (ts & TextStyle::strikethrough) os << " Strikethrough";
	if (ts & TextStyle::link) os << " Link";
	if (ts & TextStyle::cursor_after) os << " After";
	if (ts & TextStyle::bold) os << " Bold";
	return os;
}

bool operator==(const text_type &txt, const std::string &str){
	return text_type_to_string(txt) == str;
}

bool operator==(const std::string &str, const text_type &txt){
	return txt == str;
}
