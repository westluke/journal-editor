#include "paragraph.hpp"
#include <vector>
#include "cursor.hpp"

// This is gonna take ncurses.h from /usr/include, which will be a symlink to curses.h in the same folder.
#include <ncurses.h>

// #define NDEBUG
#include <cassert>

cursor get_cursor(WINDOW* win){
	int y, x;
	getyx(win, y, x);
	return {y, x};
}

const char ESC_CODE = 27;

// What should be my sanity check program?  oh shit I need a cursor position for any interaction. well, not really.



int main() {
	WINDOW* stdwin = initscr();	// Start curses mode
	cbreak();	// Disable line-by-line buffering
	noecho();	// Prevent entered characters from immediately appearing on screen
	nonl();		// Prevent enter key from being automatically turned into newline character.
	intrflush(stdscr, false);	// Prevent interrupt keys from shitting on ncurses' attempts to cleanly exit.
	
	// If this is set to true, then certain commands, like right-arrow, get reduced to a single character of extended size.
	// If it is set to false, then those commands are broken into a series, like 27-91-67
	// esc - [ - C
	// 27 is the escape key, and that is an escape sequence.
	// There is a trick here: If it is set to true, then pressing escape once starts a delay while the program looks for other characters to bind into one.
	//
	// BUT, I can use notimeout to negate this useless effect, especially useless considering that ncurses doesn't even accept user-typed escape sequences.
	// Except that the maintainers of ncurses are lazy fuckers and notimeout doesn't actually work.
	// So, do I use escape and wait or just go? Well I like the vim-style way of pressing escape, right? But I could just as easily use ` or jkl as escapes.
	// Let's keep it on true for now. That way, I don't have to interpret escape sequences at all!
	keypad(stdscr, false);
	//notimeout(stdscr, true);
	//


	// By capturing all mouse events, we prevent scrollback in the terminal.
	// However, because keypad mode is not enabled, these mouse events come as escape sequences to getchar().
	// That could confuse things, because the sequences include normal alphabetical characters.
	// We deal with this later, by taking special action whenever the ESC character (27) is received.
	//mousemask(ALL_MOUSE_EVENTS, nullptr);
	//
	//
	//NCURSES keys start at 400. But normal keys are much earlier. So, for instance, a = 97.
	//
	//Goal: support use of normal keys and enter.
	//So we can make new paragraphs and add to existing paragraphs.

	std::vector<Paragraph>paras = std::vector<Paragraph>();
	int lw = 20;
	paras.push_back(Paragraph(lw));

	int input;

	while (true){
		input = getch();

		switch(input){
			case ESC_CODE:	endwin();
					return 0;
			case KEY_ENTER: paras.push_back(Paragraph(lw));
					cursor c = get_cursor(stdwin);
					wmove(stdwin, c.y+1, 0);
			default:	cursor c = get_cursor(stdwin);

					mvwaddch(stdwin, c.y+1, 0, input);
					break;
		}

		//erase();
		
		/*
		if (input == KEY_RESIZE){
		}
		*/

		if (input == ESC_CODE){
			endwin();
			return 0;
		}
	}
}






/*
void Control::loop() {
	int input;

	while (true){
		pos.update();
		
		input = getch();

		if (input == KEY_RESIZE){
		//	resize();
		}

		else if (mode == EditorMode::insert){
			handle_insert_mode_input(input);

		}

		else if (mode == EditorMode::normal){
			handle_normal_mode_input(input);
		}
	}
}

// No matter what, keyboard shortcuts using option and command will cut through the program and affect the terminal, 
// it happens in vim too, and there's nothing I can do about it.
void Control::handle_insert_mode_input(int input) {
	if (input == ESC){
		nodelay(stdscr, true);

		// If the escape character came alone, then it was user input and we should go to normal mode.
		if (getch() == ERR){
			mode = EditorMode::normal;
		}

		// Otherwise, it came from some nefarious source and we must clear the input buffer.
		else {
			int possible_resize;
			while ((possible_resize = getch()) != ERR) {
				if (possible_resize == KEY_RESIZE) {
					resize();
				}
			}
		}
		nodelay(stdscr, false);
	}
	
	// If it wasn't the escape character, then we should print it.
	// We don't actually need a fancy display class. The text manager can probably do it all.
	// Just insert a character where we tell you.
	else{
		manager.insert_ch(pos);
	}
}

// What objects does this function need to have access to?
// If the user pushed an arrow key, it just needs to move the cursor.
// That means access to Metadata object.
// It needs to call all display functions.
// And it doesn't know where in a paragraph to add text. It might request that information, but that's too much responsibility.
// I think it makes more sense for it to make calls to a text manager which knows, based on cursor position, where to put text.
// And of course it must make calls to display managers of some sort.
// Should I write unit tests?
void Control::handle_normal_mode_input (int input){
	if (input == '='){
		endwin();
		return;
	}

	if (input == 'i'){
		mode = EditorMode::insert;
	}
}
*/

