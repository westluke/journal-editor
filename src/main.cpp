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

void wmove(WINDOW* win, cursor c){
	wmove(win, c.y, c.x);
}


void print_line(WINDOW* win, cursor c, text_type &line){
	wmove(win, c);

	for (auto fch: line){
		waddch(win, fch.character);
	}
}

// BETTER WAY TO DO THIS?
void print_lines(WINDOW* win, cursor c, std::vector<text_type> &lines){
	for (auto ln: lines){
		print_line(win, c, ln);
		c.y++;
	}
	if (lines.size() > 0){
		int last_length = lines[lines.size() - 1].size();
		wmove(win, {c.y + lines.size() - 1, c.x + last_length})
	} else {
		wmove(win, c);
	}
}

void print_para(WINDOW* win, cursor c, Paragraph &p){
	print_lines(win, c, p.get_lines());
}

void print_paras(WINDOW* win, cursor c, std::vector<Paragraph> &paras){
	for (auto p: paras){
		print_para(win, c, p);
		c.y += p.size();
	}
}

// THESE ARE ALL THE OPERATIONS THAT WE HAVE IN INSERT MODE. IMPLEMENT THEM ALL.
cursor delete_ch(WINDOW* win, cursor c, std::vector<Paragraph> &paras){
}

cursor insert_ch(WINDOW* win, cursor c, std::vector<Paragraph> &paras){
}

cursor insert_tab(WINDOW* win, cursor c, std::vector<Paragraph> &paras){
}

cursor move_right(WINDOW* win, cursor c, std::vector<Paragraph> &paras){
}

cursor move_left(WINDOW* win, cursor c, std::vector<Paragraph> &paras){
}

cursor move_up(WINDOW* win, cursor c, std::vector<Paragraph> &paras){
}

cursor move_down(WINDOW* win, cursor c, std::vector<Paragraph> &paras){
}

cursor new_paragraph(WINDOW* win, cursor c, std::vector<Paragraph> &paras){
}

// Start lets this function handle border offsets.
void update_paragraph_display(WINDOW* win, cursor start, std::vector<Paragraph> &paras){
	for (auto p: paras){
		if (p.was_changed()){
			std::vector<text_type> &lines

		}
	}
	// Check every paragraph for changes, then update at te right places.
	// That means paragraphs must know when they are changed, like lines.

}



const int ESC = 27;
const int ENTER = 13;
const int DEL = 127;

// Without keypad = true, these become escape sequences.
const int RIGHT = 261;
const int LEFT = 260;
const int UP = 259;
const int DOWN = 258;

// What should be my sanity check program?  oh shit I need a cursor position for any interaction. well, not really.
//
//
// What is the flow? A user presses a key. Based on the key, the cursor, and the paragraphs, we update the paragraphs.
// Based on the previous cursor and the change made to the paragraphs, we update the cursor. How do we know what change was made?
// It's not dependent solely on which operation was done, because it also depends on how the paragraph reacts.
//
// LEt's redefine an operation real quick. Operations can ONLY change text in contiguous chunks. Otherwise it's a set of multiple operations.
// With that said, what can we say about cursor positions before and after an operation? The number of characters before the cursor after the operation
// is equal to the number before, minus the number of characters deleted before the cursor during the operation, plus those added.
//
// The calls in the switch shouldn't know how operations are being implemented. So those operations should be changing the cursor.
// They should return cursors. And we can make the actual calculations function-dependent for efficiency.
//
// THEN, we update the display. Based on what? Based SOLELY on the paragraphs.
//
// How should we update display? I could check EVERY paragraph for changes, and then update the right lines inside those paragraphs.
// That does seem like the best move actually. Checking every paragraph for changes is actually a tiny overhead.
// Yeah, let's do that.


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
	keypad(stdscr, true);
	//notimeout(stdscr, true);
	//


	// By capturing all mouse events, we prevent scrollback in the terminal.
	// However, because keypad mode is not enabled, these mouse events come as escape sequences to getchar().
	// That could confuse things, because the sequences include normal alphabetical characters.
	// We deal with this later, by taking special action whenever the ESC character (27) is received.
	//mousemask(ALL_MOUSE_EVENTS, nullptr);

	std::vector<Paragraph>paras = std::vector<Paragraph>();
	int lw = 20;
	paras.push_back(Paragraph(lw));

	int input;
	

	// HOW ARE PARAGRAPHS COPIED?
	// Well one vector is assigned to another. How? all the elements of the vector are assigned.
	// So all the Lines are assigned. which means all the text_types are assigned. Which means all the fchars are assigned.
	// So these are actually nice data structures, they behave nicely. Which means I have to reference them instead of just copying.

	while(true){
		Paragraph &last = paras[paras.size() - 1];
		input = getch();
		cursor c = get_cursor(stdwin);

		switch(input){
			case ESC:	break;
			case TAB:	break;

			case ENTER:	paras.push_back(Paragraph(lw));
					break;

			case DEL:	last.delete_last_ch();
					werase(stdwin);
					print_paras(stdwin, c, paras);
					refresh();
					break;

			case UP:	wmove(stdwin, c.y-1, c.x);
					break;

			case DOWN:	wmove(stdwin, c.y+1, c.x);
					break;

			case RIGHT:	wmove(stdwin, c.y, c.x+1);
					break;

			case LEFT:	wmove(stdwin, c.y, c.x-1);
					break;

			default:	last.append_ch(input);
					werase(stdwin);
					print_paras(stdwin, c, paras);
					refresh();
					break;
		}
	}
}

	/*Paragraph last = paras[paras.size() - 1];

	last.append_ch(97);
	print_lines(stdwin, 0, last.get_lines());
	refresh();
	last.append_ch(97);
	print_lines(stdwin, 0, last.get_lines());
	refresh();
	last.append_ch(97);
	print_lines(stdwin, 0, last.get_lines());
	refresh();

	print_lines(stdwin, 0, last.get_lines());
	refresh();
	input = getch();
	endwin();
	return 0;
	*/



	/*
	while (true){
		input = getch();

		switch(input){
			case ESC_CODE:	endwin();
					return 0;
			case KEY_ENTER: paras.push_back(Paragraph(lw));
					cursor c = get_cursor(stdwin);
					wmove(stdwin, c.y+1, 0);
					break;
			default:	Paragraph last = paras[paras.size() - 1];
					last.append_ch(input);
					print_lines(stdwin, 0, last.get_lines());
					refresh();
					break;
		}

		
		//if (input == KEY_RESIZE){
		//}

		if (input == ESC_CODE){
			endwin();
			return 0;
		}
	}
	*/





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
	//
	//
	//NCURSES keys start at 400. But normal keys are much earlier. So, for instance, a = 97.
	//
	//Goal: support use of normal keys and enter.
	//So we can make new paragraphs and add to existing paragraphs.
	//
	//How do I calculate where a cursor should be?
	//I could pass the cursor to the paragraph as well as the other data.
	//I could also calculate a para_cursor by the number of characters offset since the start of the paragraph.
	//OR, each paragraph operation could return an informative struct that tells you how many lines were added or something.
	//
	//I think it might be time for me to get an idea of what sorts of operations I intend to support.
	//How many modes should I have? normal and insert at least. Colon commands are included in normal mode.
	//And then visual mode I guess
	//
	//insert mode: character insertion, arrow key movement, and escape to normal mode.
	//normal mode: 	commands to begin inserting (all easy)
	//		selection commands
	//		deletion (depends on how many things are deleted, where.)
	//		copying (easy)
	//		pasting (depends on how many things are pasted, where.)
	//		search (relatively easy.)
	//		movement (markers, gg, etc. all easy.)
	//
	//		The only things that make cursor movement tricky are those things that change the text.
	//		That necessarily means insertion or deletion.
	//		Insertion or deletion WITHIN a pargraph, of contiguous text, is not too bad. Use paracursor. Repeated
	//		acts can just be done consecutively, no problem.
	//
	//		But what about insertion or deletion ACROSS paragraphs? Actually, in any of these cases I would presume that we have a p_index,
	//		which makes things much easier. I just don't have it when we append.
	//
	//		Wait no im being silly. because cursors will always be dependent on the result of get_changed_lines.
	//
	//		I should have a print lines function.
	//
	//		Yes, we can associate certain formats with chtype characters, and yes, lines iwll wrap themselves.
	//		But pooorly, and inconsistently. I have to do it myself.

