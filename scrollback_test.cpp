#include <ncurses.h>
#include <iostream>


int main() {
	int y, x;

	initscr();			/* Start curses mode 		  */
	cbreak();
	noecho();			

	// Just one of keypad() or mousemask() is necessary to prevent scrollback.

	// Makes events that have special key prefixes (like some mouse events) into single values like KEY_LEFT,
	// instead of something like the combination Fn-5
	keypad(stdscr, true);

	// Without mousemask, no mouse events are reported.
	// Scrolling gets converted to character codes instead, like KEY_UP or KEY_DOWN.
	// mousemask(ALL_MOUSE_EVENTS, NULL);

	std::cout << "Hello World!" << std::endl;
	refresh();			/* Print it on to the real screen */

	getmaxyx(stdscr, y, x);
	std::cout << y << std::endl << x << std::endl;
	refresh();


	int int_char;

	while (true){
		int_char = getch();
		if (int_char == 'a'){
			endwin();	// if you don't have this the window gets very messy
			return 0;
		}
		else {
			std::cout << int_char << std::endl;
		}
	}
}
