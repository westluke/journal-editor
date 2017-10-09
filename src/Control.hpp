
//			UNUSED
//
class Control {
	public:
		void loop();
		Control (WINDOW* scr);
	
	private:
		EditorMode mode;
		TextManager manager;
		CursorPosition pos;

		void handle_normal_mode_input();
		void handle_input_mode_input();
}

Control::Control (WINDOW* scr):
	mode(EditorMode::normal),
	manager(scr),
	pos(scr)
{
	// Initialize this window as an editing window.
	
}

void Control::loop() {
	int input;

	while (true){
		pos.update();
		
		input = getch();

		if (input == KEY_RESIZE){
		//	resize();
		}

		else if (mode == EditorMode::insert){
			handle_insert_mode_input(input);

		}

		else if (mode == EditorMode::normal){
			handle_normal_mode_input(input);
		}
	}
}

// No matter what, keyboard shortcuts using option and command will cut through the program and affect the terminal, 
// it happens in vim too, and there's nothing I can do about it.
void Control::handle_insert_mode_input(int input) {
	if (input == ESC){
		nodelay(stdscr, true);

		// If the escape character came alone, then it was user input and we should go to normal mode.
		if (getch() == ERR){
			mode = EditorMode::normal;
		}

		// Otherwise, it came from some nefarious source and we must clear the input buffer.
		else {
			int possible_resize;
			while ((possible_resize = getch()) != ERR) {
				if (possible_resize == KEY_RESIZE) {
					resize();
				}
			}
		}
		nodelay(stdscr, false);
	}
	
	// If it wasn't the escape character, then we should print it.
	// We don't actually need a fancy display class. The text manager can probably do it all.
	// Just insert a character where we tell you.
	else{
		manager.insert_ch(pos);
	}
}

// What objects does this function need to have access to?
// If the user pushed an arrow key, it just needs to move the cursor.
// That means access to Metadata object.
// It needs to call all display functions.
// And it doesn't know where in a paragraph to add text. It might request that information, but that's too much responsibility.
// I think it makes more sense for it to make calls to a text manager which knows, based on cursor position, where to put text.
// And of course it must make calls to display managers of some sort.
// Should I write unit tests?
void Control::handle_normal_mode_input (int input){
	if (input == '='){
		endwin();
		return;
	}

	if (input == 'i'){
		mode = EditorMode::insert;
	}
}
