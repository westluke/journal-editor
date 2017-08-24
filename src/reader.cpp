#include "reader.hpp"

Reader::Reader(WINDOW* w, Printer p): win(w), prnt(p) {
	;
}

// updates lines normally, paragraphs when distribute() returns true.
// As I see it, the only fundamental question that remains is how we divide up this remaining work. Everything else is handled really.
// It seems right for the reader to delegate, not to do.
// So, who does it delegate to? Well the priority is updating text, right? I enter a character, and the program's internal representation of that line
// changes to match reality. And that doesn't belong here. That belongs in some sort of text manager, holding a vector of paragraphs.
// and every time the text manager updates, it builds a vector of updated lines. Those get passed to the printer. And then we're done really.
// That does mean I have to test paragraph before all this though.

// Yes, this reader should handle all actions for characters. I could hand off responsibility to another function, but that function would work almost exactly the same
// and lose flexibility. That would be abstraction inversion. Fundamentally, the work has to happen here.
// Actually, once we start to introduce modifiers and such, like command modes, this might change, with buffers and shit. But for now this works.
void Reader::loop(){
	int input;

	while (true){
		input = getch();
		//erase();
		prnt.print(std::to_string(input), 0);

		if (input == KEY_RESIZE){
		}
	}
}
