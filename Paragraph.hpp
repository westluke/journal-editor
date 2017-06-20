#ifndef PARAGRAPH_HPP
#define PARAGRAPH_HPP

#include <vector>
#include <string>
#include <cctype>

// Defines the types of formatting that a FormatModifier can cause.
enum class FormatEffect {bold, italic, underline, strikethrough, link, header1, header2, header3, none};

// Data object
class FormatModifier{
	public:
		// We can get and set the start and end,
		// but I want it to be extremely clear when that happens.
		int get_start();
		int get_end();

		int set_start();
		int set_end();

		// We cannot set the effect of an already-initialized modifier.
		FormatEffect get_effect();

		// Constructor
		FormatModifier(int start, int end, FormatEffect effect);

	private:
		// start is inclusive, end is exclusive.
		// If end is zero, that means the formatting continues for the entire paragraph.
		int start, end;
		FormatEffect effect;
};

// Describes the type of the last update done to a paragraph. Structural changes are changes in the textual content of the paragraph, 
// like adding a character. Formatting changes are what they sound like, and they don't affect the placement of characters in the paragraph.
enum class UpdateType {structural, formatting, none};

class Paragraph{
	public:
		// The replace and delete operations return what they remove from the text.
		void append_ch(char ch);
		void insert_ch(std::string::size_type index, char ch);
		char replace_ch(std::string::size_type index, char ch);
		char delete_ch(std::string::size_type index);

		void append_string(const std::string &str);
		void insert_string(std::string::size_type index, const std::string &str);
		std::string replace_string(std::string::size_type index, const std::string &str);
		std::string delete_string(std::string::size_type index, int length);

		// Must also be able to remove modifiers
		// Special behavior when same modifier applied twice.
		// Can clear formatting by applying "none" format.
		bool apply_modifier(FormatModifier modifier);

		// Without modifiers included, those are separate.
		std::vector<std::string> get_lines();

		// Returns a copy, to avoid unintentional modification.
		std::vector<FormatModifier> get_modifiers ();

		// Returns the type of the update done to this paragraph in the last char read.
		// Needs to be cleared before each cycle.
		UpdateType get_last_update();
		void clear_last_update();

		void set_line_width(std::string::size_type lw);

		void update_line_markers();

		// Constructor
		Paragraph();
		std::vector< std::pair<std::string::size_type, std::string::size_type> > line_markers;
		std::string text;
		std::string::size_type line_width;

	private:
		std::vector<FormatModifier> modifiers;

		// Gets set internally, when public methods are used to change the paragraph.
		UpdateType last_update;
};


// Should I keep this idea? I forgot to address what happens when the formatting spans multiple paragraphs. That was dumb.
// Either we have to split the formatting between paragraphs, or the formatting has to exist outside paragraphs.
// If it exists outside paragraphs, it would have to be delineated by character count from the start of the file.
// That would mean keeping track of all the characters, maybe in a giant string. That would be a nightmare, and I'm not doing it.
// Better to make multiple modifiers when formatting spans multiple paragraphs.
//
// I should make a paragraph display manager. Maybe cursor display should be part of meta.

#endif
