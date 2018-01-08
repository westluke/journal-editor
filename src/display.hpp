#include "text.hpp"
#include "line.hpp"
#include "paragraph.hpp"
#include "document.hpp"

// This is gonna take ncurses.h from /usr/include, which will be a symlink to curses.h in the same folder.
#include <ncurses.h>

namespace Display {
	void remove_cursor_from_paragraph(Document &doc, document_size i);
	void remove_cursor_from_character(Document &doc, DOCLOC dloc);

	bool find_paragraph_owning_cursor(const Document &doc, document_size &i);
	bool find_character_owning_cursor(const Document &doc, DOCLOC &dloc);

	void assert_cursor_valid(const Document &doc);

	void display_fchar(WINDOW* win, fchar ch, int y, int x);
	void display_line(WINDOW* win, const Line &line, int y);
	void display_paragraph(WINDOW* win, const Paragraph &p);
	void display_document(WINDOW* win, Document &doc);

	void move_right(WINDOW* win);
	void move_left(WINDOW* win);
	void move_up(WINDOW* win);
	void move_down(WINDOW* win);
}
