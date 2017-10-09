#include "printer.hpp"

Printer::Printer() {
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
	keypad(stdscr, true);
	//notimeout(stdscr, true);


	// By capturing all mouse events, we prevent scrollback in the terminal.
	// However, because keypad mode is not enabled, these mouse events come as escape sequences to getchar().
	// That could confuse things, because the sequences include normal alphabetical characters.
	// We deal with this later, by taking special action whenever the ESC character (27) is received.
	//mousemask(ALL_MOUSE_EVENTS, nullptr);
}

Printer::~Printer(){
	endwin();
}

void Printer::print(std::string s, Paragraph::index_type line_no){
	mvprintw(line_no, 0, s.c_str());
	refresh();
}

void weird_print(std::string s){
	static int y = 0;
	y += 1;
	y %= 10;
	if (y == 0){
		erase();
	}
	mvprintw(y, 0, s.c_str());
	refresh();
}

void bottom_iter_print(std::string s){
	;
}
