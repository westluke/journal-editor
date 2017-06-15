#include <ncurses.h>
#include <csignal>
#include <iostream>
#include <atomic>


void temp_handler(int sig) {
	raise(SIGUSR1);
}

// Yes, this worked.
// This seems to work fine so far, and it seems like most of the time functions like getch() are implemented so as to be thread-safe.
// So maybe I don't need any kind of lock.
// How would I lock it? There are a few things I have to guarantee. EVERY screen change must be detected and acted upon.
// At the same time, I definitely can't ungetch() more than one character at a time/
// Remember that almost all the time, the window resize will happen during a call to getch().
// DURING. That means ungetch() HAS to occur DURING the getch(), so locking is sorta pointless.
// What ISN'T pointless is ensuring that no more than one character is pushed back at a time.
// What would have to happen to push back more than one character? During a getch(), a signal is handled.
// BEFORE THE GETCH() COMPLETES, ANOTHER SIGNAL IS HANDLED. That would be pretty crazy. But still, is there any way to prevent it?
// Yes, I think so.
//
//
// The handler sets a global atomic to 1.
// After completing, getch() sets it to zero.
// After that, the program adjusts the window to the correct size. As long as getch() receives the right character, it doesn't matter how many signals were handled.
//
// Only if its zero can the handler ungetch().
void resize_handler(int sig) {
	std::cout << "WORKED" << std::endl;
}


// A lot of state required.
// size_changed needs a representation of window size (no atomicity necessary)
// Makes sense to use a static object inside the size_changed function.
// need to store mutex lock, must also be visible to both threads, so global with internal linkage.
// need to store flag atomically, must be visible to both threads, so global but internal linkage.


void thread_task(void) {
	while (true){
		if (flag){
			if (size_changed()){
				lock(mutex);
				ungetch(KEY_RELEASE);
				flag = FALSE;
				unlock(mutex);
			}
		}

		delay();
	}
}

bool size_changed() {
	static int prev_rows = 0, prev_cols = 0;
	int rows, cols;

	getmaxyx(stdscr, rows, cols);

	if ((prev_rows != rows) || (prev_cols != cols)){ 
		prev_rows = rows;
		prev_cols = cols;
		return true;
	}

	return false;
}

int main() {
	int y, x;
	
	// Use of sa_sigaction and sa_handler overlap, that's why. So definitely don't use braced-init.
	struct sigaction temp_handler_sigaction;
	temp_handler_sigaction.sa_handler = temp_handler;
	sigemptyset(&temp_handler_sigaction.sa_mask);
	temp_handler_sigaction.sa_flags = SA_RESTART;

	struct sigaction resize_handler_sigaction;
	resize_handler_sigaction.sa_handler = resize_handler;
	sigemptyset(&resize_handler_sigaction.sa_mask);
	resize_handler_sigaction.sa_flags = SA_RESTART;

	sigaction(SIGWINCH, &temp_handler_sigaction, nullptr);
	sigaction(SIGUSR1, &resize_handler_sigaction, nullptr);

	initscr();			/* Start curses mode 		  */
	cbreak();
	noecho();
	getmaxyx(stdscr, y, x);
	move(0, 1);
	vline('~', y);
	refresh();			/* Print it on to the real screen */

	while (!('a' == getch()))
		;
	// If you don't have this then things get crazy.
	endwin();

	return 0;
}
