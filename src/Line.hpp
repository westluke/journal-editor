#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include "text.hpp"

/* An object that represents a line of text on the display.
 * Internally, it uses an object of type text_type.
 * It can shuffle text with neighboring lines to make text wrap.
 * It has a variety of modification methods.
 * ANY sort of modification marks it as changed, 
 * so the display can be updated accordingly.
 */

/*
 * Right, what's my goal here today? To fix the definitions of relieve_excess and accept_flowback so that they are actually understandable, cuz I currently don't know what the fuck is going on there.
 * So what's the strategy? Take the text to be operated on and first break it into a vector alternating text and whitespace. That's not so bad.
 */

class Line {
	public:
		/****************
		 * SIMPLE CONST METHODS
		 ****************/

		// The number of characters in the line, including whitespace.
		text_index size() const;

		// The number of characters in the line, excluding whitespace.
		text_index visual_size() const;

		// Whether or not the line exceeds this width,
		// not counting whitespace at the end.
		bool exceeds_width_non_whitespace(text_index line_width) const;

		// Tests whether two lines can equalize,
		// without actually equalizing them.
		bool can_equalize(Line &ln, text_index line_width) const;

		// Tests whether ln can flow back into this line, without actually
		// flowing back. Used in can_equalize.
		bool can_accept_flowback(Line &ln, text_index line_width) const;

		fchar get_ch(text_index i) const;

		text_type get_text() const;
		//text_type get_text(text_index start) const;
		text_type get_text(text_index start, text_index end) const;

		std::string get_string() const;
		//std::string get_string(text_index start) const;
		//std::string get_string(text_index start, text_index end) const;
		
		// Equality operator
		bool operator==(const Line &ln) const;



		/***********************
		 * WRITE METHODS
		 ***********************/

		// fchar-level modification
		void insert_ch(text_index i, fchar ch);
		fchar delete_ch(text_index i);
		fchar replace_ch(text_index i, fchar ch);

		// text-level modification
		void append_text(text_type txt);
		//void insert_text(text_index i, text_type txt);
		//void delete_text(text_index start, text_index end);
		//text_type replace_text(text_index i, text_type txt);

		// Style modification methods
		//void change_ch_style(text_index i, TextStyle style);
		//void change_style(text_index start, TextStyle style);
		//void change_style(text_index start, text_index end, TextStyle style);
		
		// Applies relieve_excess or accept_flowback, whichever is
		// applicable here.
		bool equalize(Line &ln, text_index line_width);

		// Moves the minimum amount of words from the end of this line to 
		// ln in order to get this line under line_width.
		bool relieve_excess(Line &ln, text_index line_width);

		// Moves the maximum number of words from the beginning of ln to
		// this line such that this line is under line_width.
		bool accept_flowback(Line &ln, text_index line_width);

		std::vector<text_type> split_text() const;



		/********
		 * OTHER
		 ********/
	
		// Constructors
		Line(const text_type &t);
		Line(const std::string &str);
		Line(const char *str);
		Line();

		// Returns the private changed variable.
		bool was_changed() const;

		// FOR USE BY PARAGRAPH ONLY!
		// Marks this line as unchanged.
		bool _clear_changed_flag();

	private:
		text_type text;
		bool changed;

		void mark_changed();
};

std::ostream& operator<<(std::ostream& os, const Line& ln);
std::ostream& operator<<(std::ostream& os, const std::vector<Line>& vol);

typedef std::vector<Line>::size_type line_number;

#endif
