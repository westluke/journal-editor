#ifndef READER_HPP
#define READER_HPP

#include <ncurses.h>
#include "printer.hpp"
#include "updater.hpp"


class Reader {
	public:
		Reader(WINDOW* w, Printer p, Updater up);
		void loop();

	private:
		WINDOW* win;
		Printer print;
		Updater updater;
};

#endif
