#ifndef UPDATER_HPP
#define UPDATER_HPP

#include "paragraph.hpp"
#include "printer.hpp"

/* This is the one class that holds the Paragraphs AND the line numbers
 * 
 */

// Wait what was I thinking about marking lines changed? Why do I need that?
// Agh ok Updater is given an instruction. It acts on this instruction, changing lines of text
// to a new, internally valid state. Now it must update ncurses' understanding of the state.
// That means writing to the ncurses virtual display in several areas, before committing via refresh.
// How does it know where to write to the display?
//
// For instance, how does it know, after inserting a character, whether overflow occurred and other lines have changed?
// It could update the entire paragraph and every paragraph after. WELL NO I SOLVED THIS ISSUE, DIDN'T I?
// look at the line numbers. But that only works for paragraph-level updates.
//
// Here's the real question: how do I know which lines to update WITHIN the paragraph?
// I do think the lines themselves have to keep track. And remember, we delete on the text scale, not the line scale.
// Because otherwise, if paragraphs keep track of lines as they change those lines, thye have to know which lines were deleted, or risk doubling
// operations.
// SO YES, LINES KEEP TRACK OF BEING CHANGED.
//
// How do they keep track of being changed? Does the Paragraph tell them to be marked as changed?
// No, because it is cleanest ad clearly most effective for lines to do it themselves; any change to a Line MUST go through a changing function,
// since their text is private. In those functions, mark them as changed.
//
// There are public functions to check if a line is changed and clear changes.
// Actually, the second should probably be friended to Paragraph.
//
// Because how could I make sure that no one clears changes on a Line when they are not supposed to?
// friend vector<Line>? Paragraph::get_changed_lines();
// would require forward declaration of Paragraph.
//
// I'm ok with this. When else would someone need to clear a line? idk.
//
// Here's another thing. How does Paragraph return changed lines? 
// Should it return all the lines? And then I just look through for the changed ones? Could do that. AH but then who clears the changes?
// Updater would have to. And that's a no-no.
// So no, we have to return a list of lines that has already changed.
// And we need their line numbers. That means a new data structure!
//
// I should strongly consider the possibility that past lines and paragraphs, objects aren't needed, only functions.
// Although data does need to be passed effectively. So maybe just updater.
//
// How would I do this without the other classes?
//
// Have a loop in main to check input, then just make calls to the right functions to update internal model.
// The only data that must be consolidated is the vector of paragraphs, which can just be a simple pointer (it is a pointer anyways,
// passed by reference.)
// Can make it almost function oriented in how it deals with the data? No, because outside effects will be visible.
// We CAN'T pass this by value, so that would be a silly way to imitate functionality.
//
// Just put these functions in a namespace instead, since that is what they will REALLY be oding.
// Yeah, lol they don't have their own data, they aren't real objects.
// I'm silly.
//
// 

class Updater{
	public:
		void insert_ch(int ch);
		fchar replace_ch();
		fchar delete_ch();

		// These are the only tricky ones, because they can move across paragraph boundaries.
		// Conceptually tricky, but should be simple in practice. If the string starts in one paragraph and ends in another,
		// delete the rest of the first, every paragraph between, and the second up to the index.
		// Then join them. That's actually a unique operation that would be poorly executed with existing paragraph functions.
		// It would be most effective to make a paragraph function called merge, taking two indices and another paragraph.
		//
		// I'm not crazy, right? The best way to do this is an internal representation of text. I could just read out strings from the model,
		// but I still need to handle wrapping, which means putting them into some sort of Line class, which defeats the purpose.
		//
		// There is actually an argument to be made for the slightly delayed updates, in that updating entire lines at once is a little more easy
		// and sane than attempting to maintain consistency adding individual characters.
		void insert_str();
		std::string replace_str();
		std::string delete_str();

		void insert_ln();
		Line replace_ln();
		Line delete_ln();

		void insert_p();
		Paragraph replace_p();
		Paragraph delete_p();

		void update();
		Updater(Printer p);

	private:
		std::vector<Paragraph> paras;
		Printer print;
};

#endif
