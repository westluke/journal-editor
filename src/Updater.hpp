#ifndef UPDATER_HPP
#define UPDATER_HPP

class Updater{
	public:
		void insert_ch();
		fchar replace_ch();
		fchar delete_ch();

		// These are the only tricky ones, because they can move across paragraph boundaries.
		// Conceptually tricky, but should be simple in practice. If the string starts in one paragraph and ends in another,
		// delete the rest of the first, every paragraph between, and the second up to the index.
		// Then join them. That's actually a unique operation that would be poorly executed with existing paragraph functions.
		// It would be most effective to make a paragraph function called merge, taking two indices and another paragraph.
		void insert_str();
		std::string replace_ch();
		std::string delete_str();

		void insert_ln();
		Line replace_ln();
		Line delete_ln();

		void insert_p();
		Paragraph replace_p();
		Paragraph delete_p();

	private:
		std::vector<Paragraph> paras;
};

#endif
