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

// Handy debugging function that prints a string on the next line below the cursor.
void print_next_line(std::string line) {
	int y, x;
	getyx(stdscr, y, x);
	mvprintw(y + 1, 3, line.c_str());
	refresh();
}

int main() {
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

	// Draws a vertical line of tilde characters on the left side of the screen, like in vim.
	// Replace this later with something that adapts to lines of text.
	int y, x;
	getmaxyx(stdscr, y, x);
	move(0, 0);
	vline('~', y);
	move(0, 1);
	refresh();	//Print it on to the real screen


	// How should the real program work?
	// It always responds to characters. It waits for a character, then takes action, depending on the state of the rest of the program.
	// What kinds of states do the actions depend on? The cursor position, the lines of text, the program mode.
	// What do the lines of text look like? Program mode is simple, just a bool, and cursor position also easy, just a function call.
	// Lines of text are fundamentally a vector of string-like objects, right?
	//
	// Should I store them as lines or paragraphs? Cuz they will wrap around at a certain character limit, I know that.
	//
	// Well in the final journal they will be represented as paragraphs, so let's do that.
	// They'll have to adjust to space requirements in the website as paragraphs.
	// OK, great, and each paragraph is an object with other properties, like bolding and shit, starting and stopping at different points.
	//
	// Header formatting applies to entire paragraph
	// italics, underlines, bolds, strikethroughs, and links apply to specific regions of a paragraph.
	// The formatting is kept as separate objects in the paragraph, maybe in a vector.
	//
	// how exactly should the formatting be stored?
	// I could make separate lists for each type.
	// Or I could make one list, and have objects tell handlers their type, or have them emit certain effects.
	//
	// What do these formatting objects actually do? Lets think about that later.
	//
	// So we need a paragraph class, some formatting classes.
	// paragraphs can be blank. These are empty lines.
	// Might need a global state class.
	//
	// How do I actually do the character handling? Use lambdas, maybe with function objects, to attach new handlers?
	// Well don't do that yet. Hardcode it first, because I think that approach would run into huge issues.	
	// Make it just a big if chain.
	// But delegate the work to other structures, that's important.
	//
	// Then, of course, after receiving each char, I have to decide if the window must be updated.
	// And updating the window requires more objects.
	//
	// OK so we have some system for updating the internal representation based on the state we monitor.
	//
	// Now how do we update the window?
	// Every single character should produce a visual change in the screen, otherwise a user wouldn't know what he's doing.
	// That change can be in the text, or in meta-info on the screen. That means we need a meta display class. IT doesn't need to be part of the state,
	// cuz nothing needs to know what the surroundings look like  to act.
	// thats a little simplistic, probably need more than one such display class.
	//
	// the settings display update function should be called everytime, at the very least to move the cursor.
	// The visual change can also occur in the text, in one or more paragraphs.
	// The display update is called for every paragraph that COULD HAVE BEEN affected.
	//
	// So there's always a call to refresh.
	//
	// Lets look at paragraph display updates real quick. A few ways to do this. Maybe each paragraph handles displays itself.
	// Maybe each paragraph has an associated display object. Maybe a single display object coordinates among all paragraphs.
	// In order to properly display a paragaph, you must know on which visual line the paragraph begins.
	// The paragraphs could keep track of this themselves, but then, when one paragraph gets augmented, every following paragraph needs to be properly udpated.
	// That would be a nightmare without some managing class.
	//
	// So we have a class that manages paragraphs and updates to those paragraphs.
	// It always must work from the beginning forward.
	// Each paragraph can have two different kinds of updates: structural and formatting.
	// With a structural update, the manager might have to move around other paragraphs. It will have to change the display of the paragraph.
	// With formatting, it just has to change the display of the paragraph.
	//
	// Should I somehow store the change? Or should the manager just figure it out again every time?
	// Should the manager keep a vector of all the visual lines on the screen?
	// How would that help? A paragraph could say it added so many characters at position x. 
	// To update the display, the manager would have to know which line(s) that was.
	// And that would be tricky and prone to errors.
	//
	// Here's a better solution: The manager can ask the paragraph to output visual lines, given a line width and some parameters, like whether to break on hyphens.
	// Using what it knows about where paragraphs should be, it can determine whether more space must be allocated, and update the other paragraphs accordingly.
	//
	// Great, so so far we have Paragraph, Formatting, EditorState, ParagraphManager, MetaDisplay
	// Unclear yet how MetaDisplay should work, I think that can come later.
	//
	// What exactly should EditorState include? 
	// And how do we know how to delegate when handling an input? We have to know what paragraph the cursor belongs to.
	// ParagraphManager can output a paragraph, given a cursor position.
	// Cursor position must be updated every time the char is returned, so that threading and shit doesn't interfer with where we know the cursor to be.
	// That should be part of state.
	// Also all paragraphs, mode.
	//
	// What part should I implement first? I think paragraphs are the most basic, lets do that.
	//
	//
	//


	bool insert_mode = true;

	int int_char;

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
		}

		else if (insert_mode){
			std::ostringstream oss;
			oss << int_char;
			print_next_line(oss.str());
		}
		
	}
}
