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

	//debug = std::fstream("/dev/ttys001");
	debug << std::endl << std::endl;

	Document doc = Document(Paragraph(0, 10));
	// fix this up
	//display_document(stdwin, doc);

	int input;

	while(true){
		input = getch();

		int y, x;
		getyx(stdwin, y, x);
		DOCLOC dloc = doc.get_DOCLOC(y, x);

		// Alright, now let's implement deletion, and then new paragraphs.

		switch(input){
			case ESC:	goto kill;
					break;

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


			case UP:	Display::move_up(stdwin);
					break;

			case DOWN:	Display::move_down(stdwin);
					break;

			case RIGHT:	Display::move_right(stdwin);
					break;

			case LEFT:	Display::move_left(stdwin);
					break;
					
			default:	doc.insert_fchar(dloc, input);
					doc.assign_cursor_to_character(dloc, TextStyle::cursor_after);
					//print_all(debug, doc);
					// What does this do? re-Wraps all modified paragraphs.
					
					// so the freeze happens in here
					doc.wrap();

					/*
					debug << "\ec";
					debug << input << std::endl;
					print_all(debug, doc.get_paragraphs()[0]);
					debug << std::endl << std::endl;
					print_all(debug, doc.get_paragraphs()[0]);
					*/


					// OH its failing BECAUSE of the overflow copying, you dumbfuck

					werase(stdwin);
					Display::display_document(stdwin, doc);
					debug << std::endl << "Finished printing" << std::endl;
					refresh();
					break;
		}
	}

kill:
	endwin();
	debug.close();
}


// Functions to add lines: addchstr and addnstr
// What is chtype? How to use? All we know is it is something like an integer, so we can assign integer constants.

// Next step: let me delete a line, and then let me move the cursor.
// Moving the cursor will be much more tricky. Deleting a line is easy. Just a modification to delete_last_ch.
// Once I have the cursor movements in place, can phase out the delete_last_ch stuff.
// Then: only update new changes.
//
//
// Jk, first step is misleading. Now let's try letting me delete a character at a specific location.
// What do we need in order to do this? We av=
// What do we store? do we store the paragraph location, or the actual cursor location? Doesn't seem to make much sense to store cursor location, since that's really easy to get. In fact, it's a macro. Makes much moresense to store the paragraph location, and mayb occasionally make sure that the two are in sync As changes occur.
//
// How would that work? When we insert, depending on the results we shift the cursor and the paragraph location in the right ways
// But that's parallelism, and parallelism is dangerous.
//
// Is there a safer way to do this? Something that can be recalculated from scratch every time to always work? Or something whose recursive calculation is extremely simple and hard to fuck up?
//
//
// When I go to insert something, I HAVE to be calculating FROM the cursor position. That tells me where the user actualy things something will be inserted.
// Maybe I can optimize this later, but for now it's ok to recalculate ever time
// Moving the cursor itself is a different matter. How do I do that? Where do I work from? I would say that I work from the previous cursor position, always.
// Should I extract the cursor data? then pass it to updater? Otherwise, updater has to know about ncurses, right. Don't worry about that now.
// So either we make a function to get the parargaph location, or the thing using it does it iself.
// Make the function.
// What's the struct?

















// Interesting problems: when i move left, right, up or down, the resulting cursor movement depends on the paragraphs. Same with when I insert or anything.
// How do I do this right? Could decompose every command into sequences of movements, insertion of characters, deletions of characters, insertions of paragraphs, and deletions of paragraphs.
// Dealing with movements is relatively simple.
// How do I deal with insertions? with characters, we just move relative to our previous position in the paragraph, right? That's complicated logic, but it's a good way to think about it.
// And with insertions or deletions of paragraphs? Just move to the end or beginning of some paragraph.
//
// Ok, so with character insertion, how do I actually do that? I have the cursor at a particular spot. I delete the character and rebalance. Does what I do depend on the rebalance? I don't think so. The rebalance could end up being quite complicated, I think. I don't really want to have to analyze that. Here's what I do know. However this plays out, as long as that paragraph still exists, my position from the beginning of the paragraph goes down by 1.
// I could also phrase it in terms of PLOC; I go to the previous PLOC.
//
// So, here's what happens. I find the cursor position (constant time). Then I find the PLOC to do deletions (very low time.) Then, I go to the previous PLOC (very low time). From the previous PLOC, and the line_no of the paragraph, I find the new cursor position.
// And for insertions, we go to the next PLOC.
//
// Gets more tricky when the thing we are deleting is a newline.
//
// Also, how do I deal with insertion or deletion of blocks of text? that's tricky, isn't it. Could span paragraphs.
// With deletion, I just go before the first character involved.
// With insertion, I go after the last. Not so bad actually.
//
// But I note some things here. firstly, if updater controls all access to paragraphs, updater is gonna be really, really messy.
//
// So what do I do instead? Paragraphs is clearly something that has to be passed to like everything.
// But if I pass the vector, everything gets copied in all the time, which is really really shitty. Cuz this size could be really, really big.
//
// But think about it; I don't WANT to be passing in copies. I want to be passing in references to this paras, which is totally A-ok. So that's great.
//
// Another thing to think about: how do other copying operations work? a little tricky. I change vectors and lines and paragraphs, but all passing operations actually copy them.// Is that right? I think so, if I want them references I'll explicitly call them references, instead of copying them.
// Well, no. What I might be looking for is neither copying nor referencing, but shallow-copying. Creating a NEW OBJECT, whose sub-objects are references of each other.
// Would I ever want that? Almost certainly not.
//
// Great. So what's the next function to implement?
// I think deletions and insertions AT the cursor position. That sounds right. Build from there.
// That will require implementing a cursor-to-PLOC function.
//
// But let's not get ahead.







// How do I wrap up these methods? Namespace or class?
// Could make a Document class that holds Paragraphs. Could actually make sense.
// And the Document could have a get_PLOC function.
// And the get_cursor function? could also be there. Makes more sense than Updater.
// Yeah, let's do that.
// But what about when we need to print shit? Document definitely shouldn't do that.
// How does it get done? Well, we decided we only reprint the lines that need to be reprinted.
// So we could retreive that somehow from Document?
//
// FUCK WAIT. This coud be a problem down the line, but I want to address it here. How do we actually do the changes shit?
// I know lines know when they have been changed, and paragraphs know when at least one line is changed.
// But say a paragraph increases in size. How do I know about that? I would want to be inserting lines,
// but in order to insert lines I need to know by how much the paragraph change size.
// The alternative is reprinting everything after it. Except I'm not actually reprinting everything after it, am I, cuz the screen size is limited. So forget about printing speed, it's always constrained. That would actually be fine.
// 
// So a document could just output every changed paragraph, and we can reprint all the changed paragraphs.
// ACTUALLY, IM AN IDIOT. We don't even need all this changed shit necessarily, cuz PRINTING IS NOT A PROBLEM!
// WE CAN TOTALLY REPRINT AS MUCH AS WE LIKE! This just goes to show don't overdesign.
//
// OK, but; Document, or paras variable? That's the big question right now.
// If I use document, it's basically a giant singleton. which I actually don't want to use.
// Could I ever have more than one? only if I introduce tabs, I think. Which could actually happen.
// Alright fine, use Document.
