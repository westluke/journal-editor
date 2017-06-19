#include <vector>
#include <string>

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
		FormatEffect effect();

		// Constructor
		FormatModifier(int start, int end, FormatEffect effect);

	private:
		int start, end;
		FormatEffect effect;
};

// Describes the type of the last update done to a paragraph. Structural changes are changes in the textual content of the paragraph, 
// like adding a character. Formatting changes are what they sound like, and they don't affect the placement of characters in the paragraph.
enum class UpdateType {structural, formatting, none};

class Paragraph{
	public:
		// Combine to replace.
		insert_ch(int index, char ch);
		delete_ch(int index);

		// Must also be able to remove modifiers
		// Special behavior when same modifier applied twice.
		// Can clear formatting by applying "none" format.
		apply_modifier(FormatModifier modifier);

		// Without modifiers included, those are separate.
		std::vector<std::string> get_lines(int max_width);

		// Returns a copy, to avoid unintentional modification.
		std::vector<FormatModifier> get_modifiers ();

		// Returns the type of the update done to this paragraph in the last char read.
		// Needs to be cleared before each cycle.
		UpdateType last_update();
		void clear_last_update();

	private:
		std::string text;
		std::vector<FormatModifier> modifiers;

		// Gets set internally, when public methods are used to change the paragraph.
		UpdateType last_update;
};

// should formatting really be separate from the text?
// Yes. If i make it part of the text, whether the outputting works as i expect is platform-dependent.
// Each formatting object defines a beginning and end (besides the header), as well as a type of formatting, with an enum.
//
