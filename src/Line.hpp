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
		/***************
		 * CONSTRUCTORS
		 ***************/

		Line(const text_type &t);
		Line(const std::string &str);
		Line(const char *str);
		Line();

		

		/****************
		 * READ METHODS
		 ****************/

		// The number of characters in the line, including whitespace.
		line_size size() const;

		// The number of characters in the line, excluding whitespace.
		// line_size visual_size() const;

		fchar get_ch(line_size i) const;

		text_type get_text() const;
		//text_type get_text(line_size start) const;
		text_type get_text(line_size start, line_size end) const;

		std::string get_string() const;
		//std::string get_string(line_size start) const;
		//std::string get_string(line_size start, line_size end) const;



		/***********************
		 * WRITE METHODS
		 ***********************/

		// These are the only places where mark_changed() is called.
		// All non-const operations must be done through these methods.

		// fchar-level modification
		void insert_ch(line_size i, fchar ch);
		void delete_ch(line_size i);
		void append_ch(fchar ch);
		void delete_last_ch();
		void replace_ch(line_size i, fchar ch);

		// text-level modification
		void append_text(text_type txt);
		void prepend_text(text_type txt);
		//void insert_text(line_size i, text_type txt);
		void delete_text(line_size start, line_size end);
		//text_type replace_text(line_size i, text_type txt);

		// Style modification methods
		//void change_ch_style(line_size i, TextStyle style);
		//void change_style(line_size start, TextStyle style);
		//void change_style(line_size start, line_size end, TextStyle style);



		/***********************
		 * EQUALIZATION METHODS
		 ***********************/

		// Whether or not the line exceeds this width,
		// not counting whitespace at the end.
		bool exceeds_width_non_whitespace(line_size line_width) const;

		// Tests whether ln can flow back into this line, without actually
		// flowing back. Used in can_equalize.
		bool can_accept_flowback(Line &ln, line_size line_width) const;

		// Tests whether two lines can equalize,
		// without actually equalizing them.
		bool can_equalize(Line &ln, line_size line_width) const;

		// Redistributes text between the two lines as necessary.
		void equalize(Line &ln, line_size line_width);

		// Equalizes only if a change will be made. Returns whether a change was made.
		bool equalize_if_possible(Line &ln, line_size line_width);

		// Moves the minimum amount of words from the end of this line to 
		// ln in order to get this line under line_width.
		void relieve_excess(Line &ln, line_size line_width);

		std::vector<text_type> split_text() const;
		text_type first_chunk() const;



		/********
		 * OTHER
		 ********/
	
		// Returns the private changed variable.
		bool was_changed() const;

		// FOR USE BY PARAGRAPH ONLY!
		// Marks this line as unchanged.
		bool _clear_changed_flag();
		
		// Equality operator
		bool operator==(const Line &ln) const;

	private:
		text_type text;
		bool changed;

		void mark_changed();
};

std::ostream& operator<<(std::ostream& os, const Line& ln);
std::ostream& operator<<(std::ostream& os, const std::vector<Line>& vol);

typedef std::vector<Line>::size_type paragraph_size;
static_assert(std::is_unsigned<paragraph_size>::value, "Signed size type detected");

#endif
