#include "text.hpp"
#include "line.hpp"
#include "paragraph.hpp"
#include "document.hpp"
#include "display.hpp"
#include <vector>
#include "debug_stream.hpp"

// This is gonna take ncurses.h from /usr/include, which will be a symlink to curses.h in the same folder.
#include <ncurses.h>

// #define NDEBUG
#include <cassert>

// Another thing to implement: removing cursors from the document.
// How to do that? Actually, don't need these functions at all.
// Can just be implemented in document, since we need them there anyways.
// Actually hold up. Can easily do it for characters. For paragraphs its just more tricky cuz it requires replacing a paragraph.
// But that should be fine.
// I'm gonna need functions to insert and delete paragraphs anyway. Can just make a replacement one too.

namespace Display {
	bool find_cursor_from_paragraph(const Document &doc, DOCLOC &cursor){
		assert_cursor_valid(doc);

		for (document_size pi = 0; pi < doc.size(); pi++){
			if (doc.get_paragraph(pi).has_cursor()){
				cursor = DOCLOC(pi, 0, 0);
				return true;
			}
		}

		return false;
	}

	bool find_cursor_from_character(const Document &doc, DOCLOC &cursor){
		assert_cursor_valid(doc);

		for (document_size pi = 0; pi < doc.size(); pi++){
			Paragraph paragraph = doc.get_paragraph(pi);

			for (paragraph_size li = 0; li < paragraph.size(); li++){
				Line line = paragraph.get_line(li);

				for (line_size ti = 0; ti < line.size(); ti++){
					TextStyle ts = line.get_fchar(ti).style;

					if (ts & TextStyle::cursor_after){
						cursor = DOCLOC(pi, li, ti + 1);
						return true;
					}
				}
			}
				
		}
		return false;
	}

	// A doc is valid with respect to its cursor if one paragraph is marked, XOR one character is marked.
	void assert_cursor_valid(const Document &doc){
		std::vector<Paragraph> paragraphs = doc.get_paragraphs();
		bool found_character = false;
		bool found_paragraph = false;

		for (auto paragraph: paragraphs){

			if (paragraph.has_cursor()){
				assert(not found_paragraph);
				found_paragraph = true;
			}

			for (auto line: paragraph.get_lines()){
				for (auto fch: line.get_text()){
					if (fch.style & TextStyle::cursor_after){
						assert(not found_character);
						found_character = true;
					}
				}
			}
		}
		assert(found_character or found_paragraph);
		assert(not (found_character and found_paragraph));
	}

	void display_fchar(WINDOW* win, fchar ch, int y, int x){
		TextStyle ts = ch.style;

		if (ts & TextStyle::bold){
			attr_on(A_BOLD, NULL);
		} if (ts & TextStyle::underline){
			attr_on(A_UNDERLINE, NULL);
		}

		mvaddch(y, x, ch.character);
	}

	void display_line(WINDOW* win, const Line &line, int y){
		for (line_size x = 0; x < line.size(); x++){
			display_fchar(win, line.get_fchar(x), y, x);
		}
	}

	void display_paragraph(WINDOW* win, const Paragraph &p){
		for (paragraph_size li = 0; li < p.size(); li++){
			display_line(win, p.get_line(li), p.start_line + li);
		}

	}

	// When we find the character holding the cursor, we want to know where it was.
	DOCLOC display_document(WINDOW* win, Document &doc){
		for (auto paragraph: doc.get_paragraphs()) display_paragraph(win, paragraph);

		DOCLOC cursor;

		if (find_cursor_from_paragraph(doc, cursor)){
			doc.take_cursor_from_paragraph(cursor.paragraph);
		} else if (find_cursor_from_character(doc, cursor)){
			doc.take_cursor_from_character(DOCLOC(cursor.paragraph, cursor.line, cursor.character - 1));
		} else { assert(false); }

		return cursor;
	}

	void move_right(WINDOW* win){
		int y, x;
		getyx(win, y, x);
		move(y, x + 1);
	}

	void move_left(WINDOW* win){
		int y, x;
		getyx(win, y, x);
		move(y, x - 1);
	}

	void move_up(WINDOW* win){
		int y, x;
		getyx(win, y, x);
		move(y - 1, x);
	}

	void move_down(WINDOW* win){
		int y, x;
		getyx(win, y, x);
		move(y + 1, x);
	}

	// Implement duplicate cursor later.
	void display_cursor(DOCLOC dloc, Document &doc){
		if (dloc.character >= doc.get_line_width()){
			dloc.character = doc.get_line_width() - 1;
		}

		int y, x;
		doc.get_cursor(dloc, y, x);
		move(y, x);

	}
}
