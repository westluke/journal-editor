//#include <ncurses.h>
// #define NDEBUG	// Uncomment to disable assert()
#include <iostream>
#include <string>
#include <sstream>

#include "Line.hpp"

// #define NDEBUG
#include <cassert>

//const char ESC = 27;

/*
// Handy debugging function that prints a string on the next line below the cursor.
void print_next_line(std::string line, int x) {
	int y, x_throw_away;
	getyx(stdscr, y, x_throw_away);
	mvprintw(y + 1, x, line.c_str());
	refresh();
}
*/

int main() {
	/*
	initscr();	// Start curses mode
	cbreak();	// Disable line-by-line buffering
	noecho();	// Prevent entered characters from immediately appearing on screen
	nonl();		// Prevent enter key from being automatically turned into newline character.
	intrflush(stdscr, false);	// Prevent interrupt keys from shitting on ncurses' attempts to cleanly exit.

	// By capturing all mouse events, we prevent scrollback in the terminal.
	// However, because keypad mode is not enabled, these mouse events come as escape sequences to getchar().
	// That could confuse things, because the sequences include normal alphabetical characters.
	// We deal with this later, by taking special action whenever the ESC character (27) is received.
	mousemask(ALL_MOUSE_EVENTS, nullptr);
	*/

//	assert(false);

	Line l1 ("This is a line!");
	Line l2 ("This is a second line!");

	l1.relieve_excess(l2, 0);

	std::cout << l1.get_string() << std::endl;
	std::cout << l2.get_string() << std::endl;

	//endwin();
	return 0;
}
