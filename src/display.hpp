#include "text.hpp"
#include "line.hpp"
#include "paragraph.hpp"
#include "document.hpp"

// This is gonna take ncurses.h from /usr/include, which will be a symlink to curses.h in the same folder.
#include <ncurses.h>

namespace Display {
	bool find_cursor_from_paragraph(const Document &doc, DOCLOC &cursor);
	bool find_cursor_from_paragraph(const Document &doc, DOCLOC &cursor);

	void assert_cursor_valid(const Document &doc);

	void display_fchar(WINDOW* win, fchar ch, int y, int x);
	void display_line(WINDOW* win, const Line &line, int y);
	void display_paragraph(WINDOW* win, const Paragraph &p);
	DOCLOC display_document(WINDOW* win, Document &doc);

	void move_right(WINDOW* win);
	void move_left(WINDOW* win);
	void move_up(WINDOW* win);
	void move_down(WINDOW* win);

	void display_cursor(DOCLOC dloc, Document &doc);
}
