#include "EditorState.hpp"

CursorPosition::CursorPosition (WINDOW *scr): screen(scr) {
}

void CursorPosition::update() {
	getmaxyx(screen, y, x);
}

void CursorPosition::get_x() {
	return x;
}

void CursorPositoin::get_y() {
	return y;
}



/*
// Just copy in the cursor position.
EditorState::EditorState(CursorPosition p): pos(p) {
}

// This will do copying with the default copy constructor, which should be fine.
// We will have many pointers to the same WINDOW, but that shouldn't be a problem.
CursorPosition EditorState::get_pos() {
	return pos;
}
*/
