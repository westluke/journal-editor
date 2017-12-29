#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "paragraph.hpp"

// How many paragraphs should be initialized? zero?
// Does it even make sense to have an empty paragraph? Yes, they can be used for spacing, just as newlines.
// But I could also just record that in the positions of the parargaphs, their line numbes. which is better?
// I prefer paragraphs in between, it makes it easier to reconstruct what's going on.
// So, do I start with no paragraphs, or 1? If I start with zero, it means that we can always have a cursor after the end of the last paragraph. But that wouldn't make any sense, cuz when do we jump to that next paragraph? Unclear. So no, we have to start with 1 paragraph.
//
// Of what line width? How do we decide? is Updater told? Maybe it gets one paragraph to clone.
// Should it be told a line width at all? What makes the most sense? To me, the Updater shouldn't have to know about line widths at all.
// The cloning idea makes the most sense to me. That, or abandoning the Updater entirely in favor of a different sort of model.
// Couldj ust have that pointer to a vector of lines.

struct DOCLOC {
	DOCLOC(document_size p, paragraph_size l, line_size ch): paragraph(p), ploc({l, ch}){}
	document_size paragraph;
	PLOC ploc;
};

class Document{
	public:
		Document(Paragraph b);
		void append_para();
		// Determines what paragraph gets cloned whenever a newline is added.
		void set_base_paragraph(Paragraph p);
		void insert_ch(DOCLOC docloc, int input);
		void insert_fchar(DOCLOC docloc, fchar fch);
		void replace_fchar(DOCLOC docloc, fchar fch);
		DOCLOC get_DOCLOC(int y, int x);

		Paragraph get_paragraph(document_size pi);
		void get_cursor(DOCLOC dloc, y, x);
		//fchar replace_ch();
		//fchar delete_ch();

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
		//void insert_str();
		//std::string replace_str();
		//std::string delete_str();

		//void insert_ln();
		//Line replace_ln();
		//Line delete_ln();

		//void insert_p();
		//Paragraph replace_p();
		//Paragraph delete_p();

		//void update();
		//Updater(Printer p);
		std::vector<Paragraph> get_paragraphs() const;

	private:
		std::vector<Paragraph> paragraphs;
		Paragraph base;
		//Printer print;
};

#endif
