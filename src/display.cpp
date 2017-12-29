#include "text.hpp"
#include "line.hpp"
#include "paragraph.hpp"
#include "document.hpp"
#include <vector>

// This is gonna take ncurses.h from /usr/include, which will be a symlink to curses.h in the same folder.
#include <ncurses.h>

// #define NDEBUG
#include <cassert>

namespace Display {
	void clear_paragraph_owning_cursor(Document &doc, document_size i){
		Paragraph paragraph = doc.get_paragraphs()[i];
		assert((paragraph.has_cursor() and paragraph.size() == 0));
		doc.remove_cursor(i);
	}

	void clear_character_owning_cursor(Document &doc, DOCLOC dloc){
		fchar ch = doc.get_ch(dloc);
		assert(((ch.style & TextStyle::cursor_before) or (ch.style & TextStyle::cursor_after)));
		doc.remove_cursor(dloc);
	}

	bool find_paragraph_owning_cursor(const Document &doc, document_size &i){
		assert_cursor_valid(doc);
		std::vector<Paragraph> paragraphs = doc.get_paragraphs();

		for (document_size pi = 0; pi < paragraphs.size(); pi++){
			Paragraph paragraph = paragraphs[pi];
			if (paragraph.has_cursor() and paragraph.size() == 0){
				i = pi;
				return true;
			}
		}

		return false;
	}

	bool find_character_owning_cursor(const Document &doc, DOCLOC &dloc){
		assert_cursor_valid(doc);

		std::vector<Paragraph> paragraphs = doc.get_paragraphs();

		for (document_size pi = 0; pi < paragraphs.size(); pi++){
			Paragraph paragraph = paragraphs[pi];

			if (paragraph.has_cursor()){
				std::vector<Line> lines = paragraph.get_lines();

				for (paragraph_size li = 0; li < lines.size(); li++){
					text_type text = lines[li].get_text();

					for (line_size ti = 0; ti < text.size(); ti++){
						TextStyle ts = text[ti].style;
						if ((ts & TextStyle::cursor_after) or (ts & TextStyle::cursor_before)){
							dloc =  DOCLOC(pi, li, ti);
							return true;
						}
					}
				}
				
				return false;
			}
		}
	}

	// What must be true for this to be valid?
	// Only one paragraph can have cursor. Also, independently, zero or one character can have cursor.
	// ALSO, IF a character is found to have the cursor, that character MUST be in the paragraph that had the cursor.
	DOCLOC assert_cursor_valid(const Document &doc){
		std::vector<Paragraph> paragraphs = doc.get_paragraphs();
		bool found_character = false;
		bool found_paragraph = false;
		document_size found_in;

		for (document_size pi = 0; pi < paragraphs.size(); pi++){
			for (auto line: paragraphs[i].get_lines()){
				for (auto ch: line.get_text()){
					if ((ch.style & TextStyle::cursor_after) or (ch.style & TextStyle::cursor_before)){
						assert((!found_character));
						found_character = true;
						found_in = pi;
					}
				}
			}
		}

		for (document_size pi = 0; pi < paragraphs.size(); pi++){
			Paragraph paragraph = paragraphs[pi];
			if (paragraph.has_cursor()){
				assert((!found_paragraph));

				if (found_character){
					assert(found_in == pi);
				} else {
					assert(paragraph.size() == 0);
				}
			}
		}

		assert(found_paragraph);
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

	void display_line(WINDOW* win, Line line, int y){
		text_type text = line.get_text();

		for (line_size x = 0; x < text.size(); x++){
			fchar ch = text[x];

			display_fchar(win, ch, y, x);
		}
	}

	void display_paragraph(WINDOW* win, Paragraph p){
		std::vector<Line> lines = p.get_lines();

		for(paragraph_size li = 0; li < lines.size(); li++){
			display_line(win, lines[li], p.start_line + li);
		}
	}

	void display_document(WINDOW* win, Document &doc){
		for (auto paragraph: doc.get_paragraphs()) display_paragraph(win, paragraph);

		document_size pi;
		DOCLOC dloc;

		if (find_paragraph_owning_cursor(doc, pi)){
			move(doc.get_paragraph(pi).start_line, 0);
			clear_paragraph_owning_cursor(pi);
		} else if (find_character_owning_cursor(doc, dloc)){
			int y, x;
			doc.get_cursor(dloc, y, x);
			move(y, x);
			clear_character_owning_cursor(dloc);
		} else { assert(false); }
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
}
