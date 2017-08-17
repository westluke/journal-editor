#include <map>
#include "Paragraph.hpp"

// This class should store all the paragraphs, along with the line number that each paragraph starts on.
// I'm not sure of the best data structure to use here.
// Could use a map, from line number to paragraph. That's probably best.
class TextManager {
	public:
		void insert_ch(int ch, CursorPosition pos);
		void begin_entry();
		void end_entry();

		TextManager(WINDOW *scr);
	
	private:
		std::map<int, Paragraph> pmap;
		WINDOW *screen;
};

TextManager::TextManager(WINDOW *scr): screen(scr) {}

// When is it permitted? Whenever the position is inside a paragraph or one past the end.
void TextManager::insert_ch(int ch, CursorPosition pos){

}

// Why again am I doing paragraphs and not lines?
// If I do lines, finding a cursor is REALLY easy.
// But, for instance, wrapping around might become harder. It will require interaction with neighboring lines.
// I might be able to do that well.
// Formatting could be done on a per-line basis, but that might be slightly slower.
// Display would be easy.
// But it would not be clear to me where paragraphs began and ended. Im sticking with paragraphs.
//
// What is bad about paragraphs?
// Finding positions is annoying. It requires that I get lines from them.
// But they are much more suited to representing the text, and to conversion to a web format.
// If each paragraph could maintain an internal representation of their lines, I wouldn't really have either of these issues, but I would have double the text.
// Except that I could simply store lines as pairs of indices into the paragraph's text, which would make everything easy.
// But then again, maybe paragraphs should exist independently of their representations as lines.

ParagraphPosition TextManager::find_paragraph_position(CursorPosition pos){
	if (pmap.empty()){
		if (pos.get_x() == 0 && pos.get_y() == 0){
			return ParagraphPosition {nullptr, 0};
		}

		return ParagraphPosition {nullptr, -1};
	}

	std::map::iterator begin = pmap.begin();
	std::map::iterator end = pmap.end();

	ParagraphPosition ret;

	for (std::map::iterator it = begin; it != end; ++it){
		if (it->first <= pos.get(y)){
			ret.p
		}
	}
}
