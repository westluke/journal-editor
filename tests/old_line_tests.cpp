
#include "../src/line.hpp"
#include "../lib/catch.hpp"
#include <string>

// Didn't test equalize() here yet, since equalize() is a very simple combination of relieve_excess and accept_flowback.

SCENARIO ("Lines can be constructed in several ways."){
	SECTION("Let's test default Line construction."){
		Line l_default;

		REQUIRE(l_default.get_string() == std::string(""));
		REQUIRE(l_default.get_text() == text_type());
		REQUIRE(l_default.size() == 0);
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
			l = Line(string_to_text_type("This is a line."));
		}

		REQUIRE(l.get_string() == std::string("This is a line."));
		REQUIRE(l.get_text() == string_to_text_type("This is a line."));
		REQUIRE(l.size() == 15);
		REQUIRE(l.get_ch(3).character == 's');
	}
}

SCENARIO ("Characters can be added or removed from lines."){
	Line l = Line("This is a line.");

	SECTION ("Remove a character.") {
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

		//Line::index_type a = 30, b = 31;
		//WARN(a-b);

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
			flowback = Line("This is a flowback line and only the first 60 characters fit but no more.");
			Line flowback2 = Line("a");
			REQUIRE(acceptor.accept_flowback(flowback, 60) == true);
			//                               	      0         10        20        30        40        50        60        70        80
			REQUIRE(acceptor.get_string() == std::string("This is a flowback line and only the first 60 characters fit "));
			REQUIRE(acceptor.accept_flowback(flowback2, 60) == false);
		}
	}
}
