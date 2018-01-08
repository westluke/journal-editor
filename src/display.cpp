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
	// Implement this by making a copy of the paragraph, then removing the cursor and replacing.
	void remove_cursor_from_paragraph(Document &doc, document_size i){
		Paragraph paragraph = Paragraph(doc.get_paragraph(i));
		assert((paragraph.has_cursor() and paragraph.empty()));
		
		paragraph.remove_cursor();
		doc.replace_paragraph(i, paragraph);
	}

	void remove_cursor_from_character(Document &doc, DOCLOC dloc){
		fchar ch = doc.get_fchar(dloc);
		assert((ch.style & TextStyle::owns_cursor));
		doc.replace_fchar(dloc, fchar(ch.character, (ch.style & ~TextStyle::owns_cursor)));

		Paragraph paragraph = Paragraph(doc.get_paragraph(dloc.paragraph));
		assert(paragraph.has_cursor());
		paragraph.remove_cursor();
		doc.replace_paragraph(dloc.paragraph, paragraph);
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

	// I think this fails
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
						if (ts & TextStyle::owns_cursor){
							dloc =  DOCLOC(pi, li, ti);
							return true;
						}
					}
				}
				
				return false;
			}
		}
		return false;
	}

	// Only one paragraph can have cursor. Also, independently, zero or one character can have cursor.
	// ALSO, IF a character is found to have the cursor, that character MUST be in the paragraph that had the cursor.
	void assert_cursor_valid(const Document &doc){
		std::vector<Paragraph> paragraphs = doc.get_paragraphs();
		bool found_character = false;
		bool found_paragraph = false;
		document_size found_in;

		for (document_size pi = 0; pi < paragraphs.size(); pi++){
			for (auto line: paragraphs[pi].get_lines()){
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
				found_paragraph = true;

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

	void display_line(WINDOW* win, const Line &line, int y){
		text_type text = line.get_text();

		for (line_size x = 0; x < text.size(); x++){
			fchar ch = text[x];

			display_fchar(win, ch, y, x);
		}
	}

	void display_paragraph(WINDOW* win, const Paragraph &p){
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
			remove_cursor_from_paragraph(doc, pi);
		} else if (find_character_owning_cursor(doc, dloc)){
			int y, x;
			doc.get_cursor(dloc, y, x);
			if (doc.get_fchar(dloc).style & TextStyle::cursor_after) move(y, x+1);
			else move(y, x - 1);
			remove_cursor_from_character(doc, dloc);
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
