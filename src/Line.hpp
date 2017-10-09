#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include "text.hpp"

// An object that represents a line of text on the display. It stores fchars to permit styling.
// It can shuffle text with neighboring lines to make text wrap.
// It has a variety of modification methods. ANY sort of modification marks it as changed, so the display can be updated accordingly.
class Line {
	public:
		// The length of the line, including whitespace.
		text_index size() const;

		// Whether or not the line exceeds this width, not counting whitespace at the end.
		bool exceeds_width_non_whitespace(text_index line_width) const;

		// Simple text modification methods
		void insert_ch(text_index i, fchar ch);
		fchar delete_ch(text_index i);
		fchar replace_ch(text_index i, fchar ch);

		// NOT IMPLEMENTED
		void insert_str(text_index i, std::string str, TextStyle style = TextStyle::none);
		std::string delete_str(text_index i, text_index length);
		std::string replace_str(text_index i, std::string str, TextStyle style = TextStyle::none);

		// NOT IMPLEMENTED
		void insert_text(text_index i, text_type txt);
		text_type delete_text(text_index i, text_index length);
		text_type replace_text(text_index i, text_type txt);

		// Style modification methods, NOT IMPLEMENTED
		void change_ch_style(text_index i, TextStyle style);
		void change_style(text_index start, TextStyle style);
		void change_style(text_index start, text_index end, TextStyle style);

		// Simple text retrieval methods
		fchar get_ch(text_index i) const;

		text_type get_text() const;
		// NOT IMPLEMENTED
		text_type get_text(text_index start) const;
		text_type get_text(text_index start, text_index end) const;

		std::string get_string() const;
		// NOT IMPLEMENTED
		std::string get_string(text_index start) const;
		std::string get_string(text_index start, text_index end) const;

		// Constructors
		Line(const text_type &t);
		Line(const std::string &str);
		Line(const char *str);
		Line();

		// Equality operator
		bool operator==(const Line &ln) const;

		bool was_changed() const;	// Returns the private changed variable.

		// FOR USE BY PARAGRAPH ONLY!
		bool _clear_changed_flag();	// Stop this Line from being marked changed.

		// Either move excess text from this line to the one specified, or flow whitespace/text back.
		// In the end, the first line is guaranteed to not exceed line_width, and the second line is guaranteed to have
		// no preceding whitespace. Really a wrapper for relieve_excess and accept_flowback.
		bool equalize(Line &ln, text_index line_width);

		// Changes the state of both lines, but preserves all characters between them. Used to shuffle text between lines
		// for the purpose of text wrapping.
		bool relieve_excess(Line &ln, text_index line_width);
		bool accept_flowback(Line &ln, text_index line_width);

	private:
		text_type text;
		bool changed;

		void mark_changed();
};

std::ostream& operator<<(std::ostream& os, const Line& ln);
std::ostream& operator<<(std::ostream& os, const std::vector<Line>& vol);

typedef std::vector<Line>::size_type line_number;

#endif
