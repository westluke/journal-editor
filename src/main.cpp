#include "paragraph.hpp"
#include "document.hpp"
#include "display.hpp"
#include "debug_stream.hpp"
#include <vector>

// This is gonna take ncurses.h from /usr/include, which will be a symlink to curses.h in the same folder.
#include <ncurses.h>

// #define NDEBUG
#include <cassert>


/* Alright, here are the changes to implement:
 * Only cursor_before, with paragraphs marked with cursor_at_end.
 * changes integrated into the paragraphs
 * Alright, as usual, start at the top. interface first.
 */


const int ESC = 27;
const int ENTER = 13;
const int DEL = 127;

// Without keypad = true, these become escape sequences.
const int RIGHT = 261;
const int LEFT = 260;
const int UP = 259;
const int DOWN = 258;

int main() {	
	WINDOW* stdwin = initscr();	// Start curses mode
	cbreak();	// Disable line-by-line buffering
	noecho();	// Prevent entered characters from immediately appearing on screen
	nonl();		// Prevent enter key from being automatically turned into newline character.
	intrflush(stdscr, false);	// Prevent interrupt keys from shitting on ncurses' attempts to cleanly exit.
	keypad(stdscr, true);

	debug << std::endl << std::endl;

	Document doc = Document(Paragraph(0), 10);
	int input;
	DOCLOC cursor = DOCLOC(0, 0, 0);


	std::cout << '\a' << std::endl;

	while(true){
		input = getch();

		switch(input)
		{
			case ESC:	goto kill;
					break;

					/*
			case DEL:	// Actually I have to delete the character or paragraph right before this dloc.
					// I have to set that this cursor is before the character at the dloc.
					// to implement: deletions, previous dloc, deleting paragraphs
					// This is interesting. highly dependent.
					// See, if the line is empty, what can we do? We have to first delete the paragrph, then see if there is anything to bind to
					// in the previous line, right? What's the solution there?
					// The analogous thing would be that its right before the NEXT newline.
					// But that doesn't actually really make sense.
					// Cuz if we start incorporating newlines as characters, we have to decide which paragraph a newline belongs to, which is really shitty.
					// so let's not do that.
					
					if (doc.get_paragraph(dloc.paragraph).is_empty()){
						assert(dloc.line == 0 and dloc.character == 0);
						doc.delete_paragraph

					}


					doc.assign_cursor_to_character(dloc, TextStyle::cursor_before);

					if (dloc.character == 0){
						doc.delete_newline(dloc.paragraph);
					} else {
						doc.delete_fchar(doc.previous_dloc(dloc));
					}
					*/


			case UP:	break;
					/*
					DOCLOC next;
					if (doc.next_DOCLOC(next)){
						// do shit
					} else {
					}

					if (cursor.character 
					Display::move_up(stdwin);
					
					// increase line by 1, if possible, otherwise play tone
					break;
					*/

			case DOWN:	Display::move_down(stdwin);
					// decrease line by 1, if possible, otherwise play tone
					break;

					// Shit this is gonna get complicated. Cuz remember the cursor has to do double duty at the boundaries between lines.
					// HOW DO I DEAL WITH THAT?
					// Thye shouldn't both be valid DOCLOC positions. FUCK. Cuz they are the same DOCLOC!
					// HOW DO I DEAL WITH THIS? Oh, I know, this is easy.
					// DOCLOC's do not extend beyond the line. they can go up to the last character, but not beyond (unless it's at the end of a paragraph?)
					// However, when we have a DOCLOC with character 0, we have to add a second visual cursor. agh.
					// I may need to systematize this a little more, to get it through my head.
					//
					// I need a function that takes a DOCLOC and a document, and then prints the cursor.
					// Can also be used with the marking and rediscovery procedure.
					// I also need to make sure not to double-count DOCLOCs.
					// So I need to decide exactly how to do that. Which one is valid? 0, or extra?
					// Either one has problems. Make zero valid, easier to check and understand.
					// And then, it's also valid to be one past the end of a paragraph.
			case RIGHT:
				{	DOCLOC next;

					if (doc.next_DOCLOC(cursor, next)){
						cursor = next;
					} else {
						std::cout << "\a" << std::flush;
					}
					
					break;
				}

			case LEFT:
				{	DOCLOC prev;

					if (doc.previous_DOCLOC(cursor, prev)){
						cursor = prev;
					} else {
						std::cout << "\a" << std::flush;
					}

					break;
				}
					
			default:	doc.insert_fchar(cursor, input);
					doc.assign_cursor_to_character(cursor);
					
					// All this is where optimization will have to happen
					doc.wrap();
					werase(stdwin);
					cursor = Display::display_document(stdwin, doc);
					refresh();
					break;

					/*
					doc.insert_fchar(dloc, input);
					doc.assign_cursor_to_character(dloc, TextStyle::cursor_after);
					doc.wrap();
					werase(stdwin);
					Display::display_document(stdwin, doc);
					debug << std::endl << "Finished printing" << std::endl;
					refresh();
					break;
					*/
					// Will I need an internal model of where the cursor is? Not sure.
					// Shit I might. think about it this way. What happens when we have spacess at the end of a line?
					// Think about the steps required for that.
					// Say there are a bunch of spaces at the end.
					//
					// We go down to the first space. All fine. We advance spaces until we reach the last space on that line.
					// Do we allow the user to continue advancing spaces? Those spaces have to be there. But are they reachable?
					// They should be. That means the user can keep goign right, and the program knows where to insert or delete a character.
					// So marks can still keep data during shuffles.
					// But after the cursor is placed, I have to know not just where it is, but where the imaginary cursor is.
					// This seems dumb. Is there a better way to do this? Any other way requires that the visual cursor be the TRUE cursor position.
					// But I can't let the cursor extend beyond the length of the line.
					// That means I actually can't let the line extend that far. But what would that mean? It would mean absolutely nothing.
					// That's not possible.
					//
					// On the other hand, do I have to let them access those spaces in the MiDDLE? Maybe just the beginning and end?
					// No, that wouldn't make sense.
					//
					// What does this mean? It means I have to handle my own document cursor, which then interacts with the real cursor.
					// But they cannot be the same thing. There must be a barrier.
					// So I will have to dupliate the ncurses functions.
					//
					// I need to define exactly how that will work.
					//
					// In this file, we get the document cursor to know where to place shit.
					// Do we ever deal with the real cursor? I'm not sure, that would sorta break the consistency.
					// We could do without ever using the real cursor, just having the doc do that for us.
					// Let's try that.
					//
					// So it's gotta go through the doc.
					// The doc is just storing a DOCLOC, all the time.
					//
					// If I'm making this really correctly, I should probably make the Document object distinct from the window object.
					// Such that every window object contains one document object.
					// And what would the window contain? an Ncurses WINDOW, and also a special cursor location.
		}
		Display::display_cursor(cursor, doc);
	}

kill:
	endwin();
	debug.close();
}
