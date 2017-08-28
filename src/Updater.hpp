#ifndef UPDATER_HPP
#define UPDATER_HPP

#include "paragraph.hpp"
#include "printer.hpp"

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
