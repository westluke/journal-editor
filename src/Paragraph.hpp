#ifndef PARAGRAPH_HPP
#define PARAGRAPH_HPP

#include "line.hpp"
#include "text.hpp"
#include "PLOC.hpp"
#include <initializer_list>

// Used to define font size of paragraph.
enum class HeaderLevel {none, h1, h2, h3};

// Defines a location within a particular paragraph.
struct PLOC {
	paragraph_size line;
	line_size character;
};

class Paragraph{
	public:
		// Constructor
		Paragraph(line_size lw, paragraph_size line_number);
		Paragraph(const Paragraph &p);
		//Paragraph(std::initializer_list<Line> il, line_size lw);
		Paragraph(std::initializer_list<char*> il, line_size lw);

		// Returns length of paragraph in number of lines.
		paragraph_size size() const;
		bool empty() const;

		fchar get_fchar(PLOC ploc) const;

		// Simple text modifications
		void insert_fchar(PLOC ploc, fchar fch);
		void replace_fchar(PLOC ploc, fchar fch);

		bool has_cursor() const;
		void give_cursor();
		void remove_cursor();

		const std::vector<Line>& get_lines() const;
		const Line& get_line(paragraph_size li) const;

		bool wrap();

		// Records where in the document this paragraph begins.
		paragraph_size start_line;

	private:
		line_size line_width;
		std::vector<Line> lines;

		bool cursor;
};

typedef std::vector<Paragraph>::size_type document_size;
static_assert(std::is_unsigned<document_size>::value, "Signed size type detected");

std::ostream& print_all(std::ostream& os, const Paragraph& paragraph);
std::ostream& operator<<(std::ostream& os, const Paragraph& paragraph);

#endif
