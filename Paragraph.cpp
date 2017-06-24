#include "Paragraph.hpp"

// Bitwise OR
inline Format operator|(Format a, Format b){
	return static_cast<Format>(static_cast<char>(a) | static_cast<char>(b));
}

// Bitwise AND
inline Format operator&(Format a, Format b){
	return static_cast<Format>(static_cast<char>(a) & static_cast<char>(b));
}

// Bitwise XOR
inline Format operator^(Format a, Format b){
	return static_cast<Format>(static_cast<char>(a) & static_cast<char>(b));
}

// Bitwise NOT
inline Format operator~(Format a){
	return static_cast<Format>(~static_cast<char>(a));
}


Paragraph::Paragraph(Line::index_type lw): line_width(lw){
	;
}
