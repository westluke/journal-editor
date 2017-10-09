#ifndef READER_HPP
#define READER_HPP

#include <ncurses.h>
#include "printer.hpp"
#include "updater.hpp"
#include "paragraph.hpp"


struct CursorLoc{
	Paragraph::index_type x, y;
}

class Reader {
	public:
		Reader(WINDOW* w, Printer p, Updater up);
		void loop();
		CursorLoc get_position();


	private:
		WINDOW* win;
		Printer print;
		Updater updater;
};

#endif
