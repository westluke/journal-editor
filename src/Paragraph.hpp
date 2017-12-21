#ifndef PARAGRAPH_HPP
#define PARAGRAPH_HPP

#include "line.hpp"
#include "text.hpp"
#include "PLOC.hpp"
#include <initializer_list>

// Used to define font size of paragraph.
enum class HeaderLevel {none, h1, h2, h3};

// I can only think of one solution to properly encapsulate Lines, which is to make them a private class of Paragraph.
// Because no one else needs the functionality of lines, they only need what the Lines produce: rich text, of the type text_type.
// And then, inside the Line inside Paragraph, I can friend a Paragraph private method to alter lines. Yes?
// Ah but shit I would need a forward declaration of Line, and then clear_changes would have to take a pointer to a Line.
// I mean that's fine actually, just sorta weird.

// The real fundamental unit of text. Composed of Lines, but those Lines may pass text between each other.
// Cannot contain zero lines, may contain a single empty line.
//
// SHIT wait this means Line becomes untestable. Or I would have to switch from public to private every time.
// no fuck that. Keep it out.
//
// It has to stay out. So how do I clear lines? Is there any way for that to be a function in Paragraph?
// Then Paragraph has to be defined before Line. So Line would have to include Paragraph.
// Which is probably possible, just really fucking confusing.
// And I'm not giving Paragraph full access to Line, nor am I making the clear changes thing public.
// And if I make it an external function, then anything that includes Paragraph could use the function, right?
// Ah but what if it was only included in Paragraph.cpp? NOPE, because Line has to include it.
//
// At this point, it's either make it public or find a way to friend a Paragraph function.
// AGH ok fuck it just make it public with a preceding underscore.
//
// And we're keeping the PLOC;

struct PLOC {
	paragraph_index pn;
	line_index n;
};


class Paragraph{
	public:
		// Constructor
		Paragraph(line_index lw);
		//Paragraph(std::initializer_list<Line> il, line_index lw);
		//Paragraph(std::initializer_list<char*>, line_index lw);

		// Returns length of paragraph in number of lines.
		line_number size() const;
		bool empty();

		// Distribute
		// bool valid();
		// bool distribute();

		// Simple text modifications
		// void insert_ch(PLOC i, int ch);
		// void insert_ch(PLOC i, fchar ch); // NOT IMPLEMENTED;
		// fchar replace_ch(PLOC i, int ch); // NOT IMPLEMENTED;
		// fchar replace_ch(PLOC i, fchar ch); // NOT IMPLEMENTED;
		// fchar delete_ch(PLOC i);

		void append_ch(int ch);
		void delete_last_ch();
		void insert_ch(PLOC ploc, int ch);

		// NOT IMPLEMENTED
		// void insert_str(PLOC i, std::string str);
		// std::string delete_str(PLOC i, line_index length);
		// std::string replace_str(PLOC i, std::string str);

		// NOT IMPLEMENTED
		// void insert_text(PLOC i, text_type txt);
		// text_type delete_text(PLOC i, line_index length);
		// text_type replace_text(PLOC i, text_type txt);

		// NOT IMPLEMENTED
		// void set_header_level(HeaderLevel hl);
		// bool apply_format(PLOC start, PLOC end, TextStyle f);

		// void set_line_width(line_index lw);

		// fchar get_ch(PLOC i);
		std::vector<Line> get_lines();

		// PLOC last_index() const;

		// NOT IMPLEMENTED
		// std::vector<numbered_line> get_changed_lines();	// Returns changed lines

		// bool was_changed();

		// For use by Updater only.
		// void _clear_changed_flags();

	private:
		line_index line_width;
		std::vector<Line> lines;

		//HeaderLevel h_level;
		//TextStyle initial_style;

		//PLOC previous_index(PLOC i);
		//PLOC next_index(PLOC i);

		//bool changed;
		//void mark_changed();

		// Records where on the page this paragraph begins.
		int line_no;
};

typedef std::vector<Paragraph>::size_type document_index;
static_assert(std::is_unsigned<document_index>::value, "Signed index type detected");

#endif
