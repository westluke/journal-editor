#include <ncurses.h>
#include <csignal>
#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>

// I actually DO have ncurses.

const char ESC = 27;

void print_next_line(std::string);

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
	nonl();		// Prevent enter key from being automatically turned into newline character.
	intrflush(stdscr, false);	// Prevent interrupt keys from shitting on ncurses' attempts to cleanly exit.

	// Just one of keypad() or mousemask() is necessary to prevent scrollback.

	
	// Makes events that have special key prefixes (like some mouse events) into single values like KEY_LEFT,
	// instead of something like the combination Fn-5
	// Somehow also enables the mouse, capturing scroll input and preventing that input from reaching the terminal.
	//keypad(stdscr, true);

	mousemask(ALL_MOUSE_EVENTS, nullptr);
	
	// I want to be able to pick out key presses like ESC or CONTROL, without those disappearing into other key codes.
	// That means I can't use keypad, because then pressing something like ESC causes it to pause, looking for another key.
	// Then I HAVE to use mousemask, but I don't want the mouse sending signals that include those characters, like ESC.
	//
	// If I go without keypad mode, I have to ensure there are no other sources of control characters, otherwise I have to delay and check.
	// Because even if I can detect the other characters, I don't know whether they represent user input or not.
	// If I go WITH keypad mode, it does the delaying for me and I just can't use control characters, otherwise there will be a delay.
	//
	// I have to use mousemask with all mouse events if I don't use keypad, otherwise it doesn't block scrollback
	// and that's not acceptable. 
	// Even if I found a way to enable the mouse pointer without receiving events, non-keypad still interprets keys like arrow keys shittily.
	// So I need keypad.
	// and not mousemask.
	//
	//
	// Ah wait there could be a solution, but it's not pretty: disable keypad, use mousemask, when you get an escape character turn off delay and check for ERR.
	// If ERR, then it was an escape character alone. But depending on what happened, it could be that two different inputs occurred extremely close together, in which case you interpret that as some special character and ignore it. And come to think of it, that's totally fine as an outcome. If you're pressing two characters at exactly the same time, that's what you deserve, you prick.


	int y, x;
	getmaxyx(stdscr, y, x);
	move(0, 0);
	vline('~', y);
	move(0, 1);
	refresh();	//Print it on to the real screen


	bool insert_mode = true;

	int int_char;

	int cnt = 0;

	while (true){
		int_char = getch();

		if (int_char == '='){
			endwin();	// if you don't have this the window gets very messy
			return 0;
		}

		else if ((!insert_mode) && int_char == 'i') {
			insert_mode = true;
		}

		// This is slightly dangerous, because if the program ever gets behind in its processing of inputs,
		// and there is a user-inputted OR escape sequence ESC character in the FIFO, then the program will discard everything in the rest of the stream.
		// I mean that's pretty fair. I don't see how the program could get behind in insert mode, and then an ESC would be fine.
		// In command mode, if the program gets behind then it probably should stop processing these user inputs for a bit.
		else if (insert_mode && int_char == ESC) {
			nodelay(stdscr, true);
			int ch;
			if ((ch = getch()) == ERR){
				insert_mode = false;
			}
			else {
				while ((ch = getch()) != ERR) {
					if (ch == KEY_RESIZE) {
						//schedule_resize();
					}
				}

			}
			nodelay(stdscr, false);
		}
		
		else if (int_char == KEY_RESIZE){
			std::ostringstream oss;
			oss << "Captured KEY_RESIZE, count = " << cnt++;
			print_next_line(oss.str());
		}

		else if (insert_mode){
			std::ostringstream oss;
			oss << int_char;
			print_next_line(oss.str());
		}
		
	}
}
