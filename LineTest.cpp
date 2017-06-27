#include <iostream>
#include "Line.hpp"

int main() {
//	/*
	Line l1 ("Thisisasolidchunkoftext");
	Line l2 ("Thisisanothersolidchunk");
	l1.relieve_excess(l2, 10);

	std::cout << l1.get_string() << std::endl;
	std::cout << l2.get_string() << std::endl;

	Line l3 ("                           ");
	Line l4 ("Accept space?");
	l3.relieve_excess(l4, 10);
	std::cout << l3.get_string() << std::endl;
	std::cout << l4.get_string() << std::endl;
//	*/

	/*
	Line l1 ("Accept flowback?");
	Line l2 ("Here are some words to flow back.");
	std::cout << l1.accept_flowback (l2, 20) << std::endl;

	std::cout << l1.get_string() << std::endl;
	std::cout << l2.get_string() << std::endl;

	Line l3 ("Accept flowback?");
	Line l4 ("Hereisonesolidlinetoflowback.");
	std::cout << l3.accept_flowback(l4, 20) << std::endl;
	
	std::cout << l3.get_string() << std::endl;
	std::cout << l4.get_string() << std::endl;

	Line l5 ("Accept flowback?");
	Line l6 ("  a     a               whitespace!");
	std::cout << l5.accept_flowback(l6, 20) << std::endl;

	std::cout << l5.get_string() << std::endl;
	std::cout << l6.get_string() << std::endl;
	*/
	
	// all these tests seem to work.
	


	// test relieve_excess with space and character at line_width, and also with all space / all character.
	// test error conditions.
	//
	// test accept_flowback with all space // all character, also with the first word breaking available space.
	// test with the accepted word being followed by all whitespace, and with whitespace first then solid chaacter.
	// test with a sequence that prohibits flowback (first character, word too long)
	return 0;
}
