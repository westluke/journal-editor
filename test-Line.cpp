#include "Line.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <sstream>
#include <string>

/*
// keep this commented out, sometimes comment out catch.hpp include.
// Use to test the bitwise XOR which is failing.
int main() {
	TextStyle t_bold = TextStyle::bold;
	TextStyle t_italic = TextStyle::italic;
	TextStyle t_bold_italic = t_bold | t_italic;

	print(t_bold);
	print(t_italic);
	print(t_bold_italic);
	print(t_bold_italic ^ t_bold);
}
*/

SCENARIO ("TextStyles can be operated on with bitwise operators") {
	GIVEN ("Some TextStyle values") {
		TextStyle t_none = TextStyle::none; 
		TextStyle t_bold = TextStyle::bold;
		TextStyle t_italic = TextStyle::italic;

		WHEN ("Bold is combined with italic") {
			
			TextStyle t_bold_italic = t_bold | t_italic;

			THEN ("Both values can be extracted from the combination with bitwise AND") {
				REQUIRE ((t_bold_italic & t_bold) != TextStyle::none);
				REQUIRE ((t_bold_italic & t_italic) != TextStyle::none);
			}

			THEN ("Other values cannot be extracted with bitwise AND"){
				REQUIRE ((t_bold_italic & TextStyle::underline) == TextStyle::none);
			}

			THEN ("We can remove values with bitwise NOT and bitwise AND"){
				REQUIRE ((t_bold_italic & (~t_bold)) == t_italic);
			}

			THEN ("We can flip values with bitwise XOR"){
				REQUIRE ((t_bold_italic ^ t_bold) == t_italic);
				REQUIRE ((t_bold_italic ^ TextStyle::underline) == (t_bold_italic | TextStyle::underline));
			}
		}
	}
}

SCENARIO ("The Line class provides its own text representation, as a vector of fchars. fchars store a character and a TextStyle."){
	SECTION("fchar.isspace() should be able to recognize any sort of whitespace."){
		REQUIRE((fchar{' ', TextStyle::none}).isspace());
		REQUIRE((fchar{'\n', TextStyle::none}).isspace());
		REQUIRE((fchar{'\t', TextStyle::none}).isspace());
	}

	GIVEN("An std::string \"HELLO\" and the Line::text_type representation of that string"){
		std::string s("HELLO");
		std::vector<fchar> fs = { {'H', TextStyle::none},
					  {'E', TextStyle::none},
					  {'L', TextStyle::none},
					  {'L', TextStyle::none},
					  {'O', TextStyle::none}};

		THEN ("We should be able to convert between them using static functions from Line."){
			REQUIRE(Line::text_type_to_string(fs) == s);
			REQUIRE(Line::string_to_text_type(s) == fs);
			REQUIRE(Line::string_to_text_type("HELLO") == fs);
		}
	}
}

SCENARIO ("Lines can be constructed in several ways."){
	SECTION("Let's test default Line construction."){
		Line l_default;

		REQUIRE(l_default.get_string() == std::string(""));
		REQUIRE(l_default.get_text() == Line::text_type());
		REQUIRE(l_default.line_length() == 0);
	}

	SECTION("Let's test Line construction with initializers.") {
		Line l;

		SECTION("Line created with string literal constructor"){
			l = Line("This is a line.");
		}

		SECTION("Line created with std::string constructor"){
			l = Line(std::string("This is a line."));
		}

		SECTION("Line created with text_type constructor"){
			l = Line(Line::string_to_text_type("This is a line."));
		}

		REQUIRE(l.get_string() == std::string("This is a line."));
		REQUIRE(l.get_text() == Line::string_to_text_type("This is a line."));
		REQUIRE(l.line_length() == 15);
		REQUIRE(l.get_ch(3).character == 's');
	}
}

SCENARIO ("Characters can be added or removed from lines."){
	Line l = Line("This is a line.");

	SECTION ("Remove a character."){
		REQUIRE(l.delete_ch(3).character == 's');
		REQUIRE(l.get_string() == std::string("Thi is a line."));
	}

	SECTION ("Add a character."){
		l.insert_ch(3, fchar('i'));
		REQUIRE(l.get_string() == std::string("Thiis is a line."));
	}
}

SCENARIO ("Line objects store lines of text as they might appear in a text window, and so provide functions for text wrapping."){
	SECTION("Testing relieve_excess"){
		// We always restrict this line to 60 characters.
		Line excess;
		Line acceptor = Line("");

		SECTION("Cutting off some text, but the point of separation is whitespace (whitespace at index line_width)"){
			//	       0         10        20        30        40        50        60        70        80
			excess = Line("This Line is too long, so the end must be cut off and placed in the accepting Line.");
			REQUIRE(excess.relieve_excess(acceptor, 60) == true);
			REQUIRE(acceptor.get_string() == std::string("in the accepting Line."));
			REQUIRE(excess.get_string() == std::string("This Line is too long, so the end must be cut off and placed "));
		}

		SECTION("Try it when we're just attempting to cut off whitespace."){
			//	       0         10        20        30        40        50        60        70        80
			excess = Line("This Line is too long, so the end must be cut off and placed                       ");
			REQUIRE(excess.relieve_excess(acceptor, 60) == false);
			REQUIRE(acceptor.get_string() == "");
			REQUIRE(excess.get_string() == std::string("This Line is too long, so the end must be cut off and placed                       "));
		}

		SECTION("Non-whitespace at line_width, so must search backwards"){
			//	       0         10        20        30        40        50        60        70        80
			excess = Line("This Line is too long, so the end must be cut off and placedXin the accepting Line.");
			REQUIRE(excess.relieve_excess(acceptor, 60) == true);
			REQUIRE(acceptor.get_string() == std::string("placedXin the accepting Line."));
			REQUIRE(excess.get_string() == std::string("This Line is too long, so the end must be cut off and "));
		}

		SECTION("Non-whitespace from the beginning to line_width, so must still cut at line_width."){
			//	       0         10        20        30        40        50        60        70        80
			excess = Line("ThisXLineXisXtooXlong,XsoXtheXendXmustXbeXcutXoffXandXplacedXin the accepting Line.");
			REQUIRE(excess.relieve_excess(acceptor, 60) == true);
			REQUIRE(acceptor.get_string() == std::string("Xin the accepting Line."));
			REQUIRE(excess.get_string() == std::string("ThisXLineXisXtooXlong,XsoXtheXendXmustXbeXcutXoffXandXplaced"));
		}

		SECTION("The inputted line_width is too great."){
			//	       0         10        20        30        40        50        60        70        80
			excess = Line("This Line is too long, so the end must be cut off and placed in the accepting Line.");
			REQUIRE(excess.relieve_excess(acceptor, 100) == false);
			REQUIRE(acceptor.get_string() == std::string(""));
			REQUIRE(excess.get_string() == std::string("This Line is too long, so the end must be cut off and placed in the accepting Line."));
		}

		SECTION("Try it when the acceptor isn't empty."){
			acceptor = Line("Now, do we have pieces of both sentences in this Line?");
			//	       0         10        20        30        40        50        60        70        80
			excess = Line("This Line is too long, so the end must be cut off and placed in the accepting Line.");
			REQUIRE(excess.relieve_excess(acceptor, 60) == true);
			REQUIRE(acceptor.get_string() == std::string("in the accepting Line.Now, do we have pieces of both sentences in this Line?"));
			REQUIRE(excess.get_string() == std::string("This Line is too long, so the end must be cut off and placed "));
		}
	}

	SECTION ("Testing accept_flowback"){
		Line acceptor = Line("");
		Line flowback;

		SECTION("The flowback Line is empty."){
			flowback = Line("");
			REQUIRE(acceptor.accept_flowback(flowback, 60) == false);
		}

		SECTION("The flowback Line is all whitespace."){
			flowback = Line(" ");
			Line flowback2 = Line("                                                                                 	\n");
			REQUIRE(acceptor.accept_flowback(flowback, 60) == true);
			REQUIRE(acceptor.get_string() == std::string(" "));
			REQUIRE(flowback.get_string() == std::string(""));

			REQUIRE(acceptor.accept_flowback(flowback2, 60) == true);
			REQUIRE(acceptor.get_string() == std::string("                                                                                  	\n"));
			REQUIRE(flowback.get_string() == std::string(""));
		}

		SECTION("The flowback Line doesn't contain whitespace."){
			flowback = Line("Thisisaflowbackline");
			REQUIRE(acceptor.accept_flowback(flowback, 60) == true);
			REQUIRE(acceptor.get_string() == std::string("Thisisaflowbackline"));
			REQUIRE(flowback.get_string() == std::string(""));
		}

		SECTION("The flowback Line's first word fits in acceptor."){
			//	         0         10        20        30        40        50        60        70        80
			flowback = Line("Thisisaflowbacklineandonlythefirstwordshouldfit\nbutnoneoftheothers so here they are.");
			REQUIRE(acceptor.accept_flowback(flowback, 60) == true);
			REQUIRE(acceptor.get_string() == std::string("Thisisaflowbacklineandonlythefirstwordshouldfit\n"));
			REQUIRE(flowback.get_string() == std::string("butnoneoftheothers so here they are."));
		}

		SECTION("The flowback Line's first word doesn't fit in the acceptor."){
			//	         0         10        20        30        40        50        60        70        80
			flowback = Line("Thisisaflowbacklineandthefirstwordshouldn'tfitmeaningthatnothingwill flow and shit.");
			REQUIRE(acceptor.accept_flowback(flowback, 60) == false);
			REQUIRE(acceptor.get_string() == std::string(""));
			REQUIRE(flowback.get_string() == std::string("Thisisaflowbacklineandthefirstwordshouldn'tfitmeaningthatnothingwill flow and shit."));
		}

		SECTION("More complicated example where the flowback Line starts with whitespace and only the first few words fit in the acceptor."){
			//	         0         10        20        30        40        50        60        70        80
			flowback = Line(" This is a flowback line and only the first few words shouldfitbutnomore.");
			REQUIRE(acceptor.accept_flowback(flowback, 60) == true);
			REQUIRE(acceptor.get_string() == std::string(" This is a flowback line and only the first few words "));
			REQUIRE(flowback.get_string() == std::string("shouldfitbutnomore."));
		}

		SECTION("60 characters should fit."){
			//	         0         10        20        30        40        50        60        70        80
			flowback = Line("This is a flowback line and only the first 60 characters fit few words should fitbutnomore.");
			Line flowback2 = Line("a");
			REQUIRE(acceptor.accept_flowback(flowback, 60) == true);
			REQUIRE(acceptor.get_string() == std::string("This is a flowback line and only the first 60 characters fit "));
			REQUIRE(acceptor.accept_flowback(flowback2, 60) == false);
		}
	}
}
