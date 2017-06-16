#include <ncurses.h>
#include <csignal>
#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>

void print_next_line(std::string);


/*
 * AAAGH ncurses handles pushing the KEY_RESIZE character itself! FAK ME!
 *
 *
 *
// Prevents the thread from putting more than one character at a time into the buffer with ungetch().
std::atomic_bool resize_flag;

// Prevents the case where the main thread sets the resize_flag to true between the call to ungetch() and the setting of resize_flag to false in the resize thread.
std::mutex resize_mtx;

bool size_changed(); 
void thread_task(void);

void thread_task(void) {
	while (true){
		if (resize_flag && size_changed()){
			resize_mtx.lock();
			ungetch(KEY_RESIZE);
			resize_flag = false;
			resize_mtx.unlock();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

bool size_changed() {
	static int prev_rows = 0, prev_cols = 0;
	int rows, cols;

	getmaxyx(stdscr, rows, cols);

	if ((prev_rows != rows) || (prev_cols != cols)){ 
		prev_rows = rows;
		prev_cols = cols;

		return true;
	}

	return false;
}
*/

void print_next_line(std::string line) {
	int y, x;
	getyx(stdscr, y, x);
	mvprintw(y + 1, 1, line.c_str());
	refresh();
}

int main() {
	initscr();	// Start curses mode
	cbreak();	// Disable line-by-line buffering
	noecho();	// Prevent entered characters from immediately appearing on screen

	// Just one of keypad() or mousemask() is necessary to prevent scrollback.

	// Makes events that have special key prefixes (like some mouse events) into single values like KEY_LEFT,
	// instead of something like the combination Fn-5
	keypad(stdscr, true);

	// Without mousemask, no mouse events are reported.
	// Scrolling gets converted to character codes instead, like KEY_UP or KEY_DOWN.
	// mousemask(ALL_MOUSE_EVENTS, NULL);

	int y, x;
	getmaxyx(stdscr, y, x);
	move(0, 0);
	vline('~', y);
	move(0, 1);
	refresh();	//Print it on to the real screen

	int int_char;

	int cnt = 0;

	while (true){
		int_char = getch();

		if (int_char == 'a'){
			endwin();	// if you don't have this the window gets very messy
			return 0;
		}

		if (int_char == 'g'){
			ungetch(KEY_RESIZE);
		}
		
		if (int_char == KEY_RESIZE){
			int y, x;
			getyx(stdscr, y, x);
			mvprintw(y + 1, 1, "Captured KEY_RESIZE, count = %d", cnt++);
			refresh();
		}
		
	}
}
