// This is gonna take ncurses.h from /usr/include, which will be a symlink to curses.h in the same folder.
#include <ncurses.h>
#include <iostream>

// #define NDEBUG
#include <cassert>


int main() {
	WINDOW* stdwin = initscr();	// Start curses mode
	cbreak();	// Disable line-by-line buffering
	noecho();	// Prevent entered characters from immediately appearing on screen
	nonl();		// Prevent enter key from being automatically turned into newline character.
	intrflush(stdscr, false);	// Prevent interrupt keys from shitting on ncurses' attempts to cleanly exit.
	
	keypad(stdscr, false);

	int input;
	
	while(true){
		input = getch();

		/*
		if (input == 27){
			break;
		}
		*/

		std::cout << input << std::endl;
	}

	endwin();
	return 0;
}
