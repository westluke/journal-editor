#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <string>
#include <ncurses.h>
#include "paragraph.hpp"
#include "line.hpp"

class Printer {
	public:
		Printer();
		~Printer();
		void print(std::string s, Paragraph::index_type line_no);

		void print(Line::text_type txt, Paragraph::index_type line_no);
		void print(Paragraph p, int line);

		Paragraph::p_index get_index_from_loc(CursorLoc cl);


	private:
		WINDOW* win;
};

void bottom_iter_print(std::string s);


		// I NEED TO SPLIT THESE UP MORE. BECAUSE IM CREATING LOOPS.
		// RIGHT NOW THERES A LOOP BETWEEN READER AND PRINTER, BECAUSE PRINTER NEEDS CURSORLOC
		// Ah wait but cursorloc depends on Paragraph
		// BUT I CAN SOLVE THAT EASILY BY PUTTING CURSORLOC AND OTHER SIMPLE STRUCTURES IN THEIR OWN FILE. WHAT ELSE? TEXTSTYLE DEFINITELY.
		// NOT P-INDEX DUE TO SIZE TYPES.
		// fchar definitely, maybe header
		//
		// Fuck it, make everything include Lines and Paragraphs too.
		// That actually makes sense.
		// 
		// So how would that work? Well p_index should stay with Paragraph, And so should headerlevel. CursorLoc requires Paragraph, so I could just drop it in the paragraph file as well. That means wherever CursorLoc is defined, Paragraph must be defined too. But really, it's connection to Pargraaph is tangential, so I'm gonna redefine it. getyx uses ints, so i will use ints. How about p_index? nah still gotta use Paragrpah stuff.


// How do we define borders here?
// How do we make that system cleanly?
// I don't want the text manager fucking about with borders. That's nuts. And the reader has nothing to do with it.
// The text manager should just tell the printer WHICH lines to reprint, not where. To the text manager, "WHERE" makes no sense. INDICES make sense.
// Print the third line of the fourth paragraph, or print this line as the 35th. That makes sense.
// So the printer should get some setup for the border, and must take care of border numbers itself.
// It must also take care of the periphery itself. These distinctions make sense, right? Yeah for now I can see no better way to subdivide the work.
// This also means that the printer is really on the only object capable of computing between a cursor location and a p_index, right?
//
// OH wait no. Because it doesn't know where paragraphs start. As a matter of fact, who DOES know where paragraphs start?
// I don't feel like TextManager should know that, because it's not about text. Printer could know that, but then it's storing paragraphs itself, which is weiiird.
// I propose bringing back EditorState. I think my instincts were right there. EditorState is the link between interface and text.
//
// It contains a vector of objects, each of which store Paragraphs, the lines they start on, and the lines most recently updated perhaps.
// This vector could have its own validity function. But to do it that way is a little indirect; It's no longer purely about passing objects to each other.
// Find a simpler solution? No matter what, a single class must possess both paragraphs and line numbers, and no other class should possess that.
//
// So what class should have it? It doesn't belong to printer, since printer won't modify shit. Certainly doesn't belong to reader.
//
// Things have to modify these paragraphs. But also, just realized, their line numbers are basically intrinsic to them, so textmanager could have them.
// I mean the line number for the first paragraph is zero, and then it just increases by the lengths of the paragraphs, yeah.
//
// And in a sense the line number is a part of the text representation, so this is fine. The textmanager stores them in its own objects.
//
//
//printer, reader, and updater cannot be unit-tested.


#endif
