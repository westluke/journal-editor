#include "printer.hpp"
#include "reader.hpp"

// #define NDEBUG
#include <cassert>

//const char ESC = 27;

// What should be my sanity check program?  oh shit I need a cursor position for any interaction. well, not really.
// Simple: any character inputted gets added to a paragraph through Reader and then Update. then Update prints updated paragraph.
int main() {
	Printer p = Printer();
	Updater up = Updater(p);
	Reader r = Reader(stdscr, p, up);
	r.loop();
	return 0;
}
