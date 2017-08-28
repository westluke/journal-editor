#ifndef PARAGRAPH_HPP
#define PARAGRAPH_HPP

#include "line.hpp"
#include <initializer_list>

// Used to define font size of paragraph.
enum class HeaderLevel {none, h1, h2, h3};

struct p_index;


// The real fundamental unit of text. Composed of Lines, but those Lines may pass text between each other.
// Cannot contain zero lines, may contain a single empty line.
class Paragraph{
	public:
		typedef std::vector<Line>::size_type index_type;

		// Simple text modifications
		void insert_ch(p_index i, int ch);
		void insert_ch(p_index i, fchar ch);
		fchar replace_ch(p_index i, int ch);
		fchar replace_ch(p_index i, fchar ch);
		fchar delete_ch(p_index i);

		void insert_str(p_index i, std::string str);
		std::string delete_str(p_index i, Line::index_type length);
		std::string replace_str(p_index i, std::string str);

		void insert_text(p_index i, Line::text_type txt);
		Line::text_type delete_text(p_index i, Line::index_type length);
		Line::text_type replace_text(p_index i, Line::text_type txt);

		// Distribute
		bool distribute();
		bool valid();

		void set_header_level(HeaderLevel hl);
		bool apply_format(p_index start, p_index end, TextStyle f);

		void set_line_width(Line::index_type lw);

		fchar get_ch(p_index i);
		std::vector<Line> get_lines();

		// Constructor
		Paragraph(Line::index_type lw);
		Paragraph(std::initializer_list<Line> il, Line::index_type lw);
		Paragraph(std::initializer_list<char*>, Line::index_type lw);

	private:
		Line::index_type line_width;
		std::vector<Line> lines;

		HeaderLevel h_level;
		TextStyle initial_style;

		p_index previous_index(p_index i);
		p_index next_index(p_index i);

		// Records where on the page this paragraph begins.
		int line_no;
};

// hrmmmm shit gotta think about this more.
// What if a shift occurs and invalidates the changed_lines? Well it can only invalidate things AFTER the shift. All that means is that when the shift is set,
// we ONLY use the shift to update past the shift, because otherwise we could get into segfault territory.
// Question: the shift is all well and good when lines have been added. In that case, the shift_point is the index of the inserted line.
// But what about when the change is just that the last line has been deleted? In that case the shift_point points one past the end of the paragrpah.
// I suppose that makes some sense, in that its informative about how to deal with the next paragraphs.
// But how would this actually work? and is there a better altnerative?
//
// Something has a refresh function, perhaps the printer. It is fed paragraphs. WAAAIIIT a sec. is that a good idea? To shove all the content intp a function every refresh?
// It could be done by reference. Still, that seems silly.
//
// could use wnoutrefresh and sorta dispense with these stored updates.
// Could also use ncurses's capability to do character insertion, string insertion, and line insertion (insdelln, insertln) instead of rewriting lines
//
// There's something I want to be very careful about here. The object changing the text should also be the one controlling updates to the screen, and reader won't be smart enough to do both.
//
// TextManager is unsuited for that. What is this really? Some sort of Updater, perhaps also in control of refresh(), in which case textmanager is unnecessary.
// Sorta takes over the function of TextManager in a way that makes more sense.
//
//
//So what's the flow? Reader accepts a char, determines what to do with it. Frequently, it's a simple insertion, so that gets sent to Update, which internally contains all the paragraphs. Before it does that, Update will need to know where in the paragraphs to change, and reader only knows the cursor position.
//So reader asks Display for the meaning of that cursor position, and Display tells it the paragraph index.
//Now, Update can make the change, but it doesn't refresh immediately.
//When it's time to refresh, Update considers the changed and changes the display (through Display) as efficiently as possible.
//
//How does it consider the changes? Let's consider the changes that COULD be made. In general, we have replacement, insertion, and deletion.
//Those apply to characters, lines, strings within a line, strings across lines, and paragraphs. It must be able to handle an arbitrary number of these random changes,
//quickly.
//
//
//So, how do we do it? Firstly, we should probably move from the front to the back, yes. Consider why.
// Possibly just simpler to think about, but actually maybe less efficient?
// If I start from the back, and make replacements and such, and later encounter something that shifts, thats fine.
// It's the same amount of work as if I started from the front.
// The difference is, when I start from the back, future line numbers are UNCHANGED, for the most part. As long as I stay in one paragraph, higher paragrpahs are the same.
// And as long as I stay in one line, higher lines are the same. The same can't be said for moving forward.
// Any time I DO move into a higher paragraph, as with a long text replacement, I may need to recalculate.
// Ah but wait I see an issue. If I store updates as I intend, then we can really only update line-by-line, which is simpler but perhaps less efficient.
//
// There is another solution, to use that internal refresh before the display refresh, and do it every time. Probably costs the same.
// In that case, I don't need to record changes at all.
//
// Wait this is silly. the data structures of the window ARE the recorded changes. They are committed by refresh(). I'm an idiot. Don't record changes at all.
// Update() takes request, immediately makes changes to BOTH the paragraph and the internal model.

// Defines a location within a paragraph.
struct p_index {
	Paragraph::index_type line_no;
	Line::index_type ch_index;
};

#endif
