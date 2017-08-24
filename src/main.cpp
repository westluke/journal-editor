#include "printer.hpp"
#include "reader.hpp"

// #define NDEBUG
#include <cassert>

//const char ESC = 27;

int main() {
	Printer p = Printer();
	Reader r = Reader(stdscr, p);
	r.loop();
	return 0;
}
