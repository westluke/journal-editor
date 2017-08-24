#include <string>
#include <ncurses.h>

#ifndef PRINTER_HPP
#define PRINTER_HPP

class Printer {
	public:
		Printer();
		~Printer();
		void print(std::string s, int line);
		void print(Paragraph p, int line)

	private:
		WINDOW* win;
};

#endif
