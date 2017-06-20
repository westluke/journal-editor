#ifndef CURSOR_POSITION_HPP
#define CURSOR_POSITION_HPP

#include <ncurses.h>
#include <vector>

#include "Paragraph.hpp"

enum class EditorMode{insert, normal};

class CursorPosition {
	public:
		// These are getter functions to prevent unintentional modification of x and y
		// by user functions, which would be terrible.
		int get_x();
		int get_y();

		// x and y can ONLY be modified through the update function.
		void update();

		// Constructor
		CursorPosition(WINDOW *scr);

	private:
		WINDOW *screen;	// Generally stdscr
		int x, y;
};

/*
// Yes, I like this, this encapsulates EVERYTHING we know about the editor at a moment in time, when a command is not running.
class EditorState{
	public:
		// The mode can be altered by many different actions, so I'm keeping this public.
		EditorMode mode;

		// The cursor position should NEVER be modified by anything except the object itself, so this uses a getter.
		CursorPosition get_pos();
		
		// Constructor
		EditorState(CursorPosition p);

	private:
		CursorPosition pos;
};
*/

#endif
