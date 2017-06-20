//#include <ncurses.h>
#include <iostream>
#include <string>
#include <sstream>

#include "Paragraph.hpp"

const char ESC = 27;

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

	//Control cntrl(stdscr);
	//cntrl.loop();
	
	Paragraph p;
	p.set_line_width(20);
	p.append_string("a");

	// looks like the function works! Later, I might want to run tests and see if it can run faster using strings as data.
	// But for now, this works just fine.

	/*
	p.append_string("This is a paragraph. I want to see whether it can effectively split this text into several lines, and how easily it does it.");
	p.append_string("Here's a lot of whitespace:                                                            ");
	p.append_string("Now take it without whitespace: ________________________________________________________");
	*/

	std::cout << p.line_width << std::endl;;
	std::cout << p.text << std::endl;;
	
	for (auto pr : p.line_markers){
		std::cout << pr.first << " " << pr.second << std::endl;
	}

	for (std::string line : p.get_lines()){
		std::cout << line << std::endl;
	}




	//endwin();
	return 0;
}
