#ifndef READER_HPP
#define READER_HPP

#include <ncurses.h>
#include "printer.hpp"



class Reader {
	public:
		Reader(WINDOW* w, Printer p);
		void loop();

	private:
		WINDOW* win;
		Printer prnt;
};

#endif
