#include "../src/paragraph.hpp"
#include "../src/Line.hpp"

#include "../lib/catch.hpp"
#include <string>

#include <iostream>
// Far and away the most important functions to test, really the only functions to test right now, are valid, distribute, insert, and delete.
// Test valid first, then distribute, then delete, then insert

SCENARIO("Paragraphs have two constructors, one to specify only line_width and one to initialize with Lines."){
	Paragraph p1 = Paragraph({Line("Hello there,"), Line("this is a"), Line("paragraph.")}, 20);
	std::vector<Line> test1 = {Line("Hello there,"), Line("this is a"), Line("paragraph.")};
	REQUIRE(p1.get_lines() == test1);

	Paragraph p2 = Paragraph({"Hello there,", "this is a", "paragraph."}, 20);
	REQUIRE(p2.get_lines() == test1);

	Paragraph p3 = Paragraph(20);
}

SCENARIO("A paragraph tests its own internal consistency using the valid() function."){
	SECTION("Starting whitespace is ok."){
		Paragraph p({"    Here is some starting whitespace."}, 100);
		REQUIRE(p.valid());
	}

	SECTION("Starting whitespace in a line that isn't the first line is not ok."){
		Paragraph p({"Can't have whitespace", "    in the second line."}, 25);
		REQUIRE_FALSE(p.valid());
	}

	SECTION("A paragraph composed of a single empty line is ok."){
		Paragraph p({""}, 100);
		REQUIRE(p.valid());
	}

	SECTION("A multi-line paragraph with an empty line is not ok."){
		Paragraph p({"The second line being empty is not ok.", "", "asdfjh"}, 100);
		REQUIRE_FALSE(p.valid());
	}

	SECTION("A line in the paragraph is permitted to exceed its line_width if it does so with whitespace."){
		Paragraph p({"Here is space:                    ", "normal line"}, 20);
		REQUIRE(p.valid());
	}

	SECTION("Lines may not exceed their line_width with non-space characters."){
		Paragraph p({"Exceeds its permitted width.", "Doesn't."}, 15);
		REQUIRE_FALSE(p.valid());
	}

	SECTION("Lines may not have the potential to flow back to a previous line."){
		Paragraph p({"Here's a line.", "Here's another line, longer, that can flow into the previous."}, 100);
		REQUIRE_FALSE(p.valid());
	}
}

SCENARIO (	"The real heart of Paragraph is the distribute() function, which ensures that a paragraph is valid"
		"by removing empty lines and spreading long lines to shorter adjacent lines."){

	SECTION("Make sure that it deletes empty lines."){
		Paragraph p({"Here's a line.", "", "Another line."}, 17);
		std::vector<Line> test = {"Here's a line.", "Another line."};
		p.distribute();
		REQUIRE(p.get_lines() == test);
	}

	SECTION("Make sure that it doesn't delete a paragraph composed of a single empty line."){
		Paragraph p({""}, 10);
		std::vector<Line> test = {""};
		p.distribute();
		REQUIRE(p.get_lines() == test);
	}

	SECTION("distribute() should allow lines to flow back."){
		Paragraph p({"Here's a line. ", "The next ", "line should flow."}, 16);
		std::vector<Line> test = {"Here's a line. ", "The next line ", "should flow."};
		p.distribute();
		REQUIRE(p.get_lines() == test);
	}

	SECTION("distribute() should relieve excess in lines."){
		Paragraph p({"Here's a line that's too long. ", "It should flow."}, 25);
		std::vector<Line> test = {"Here's a line that's too ", "long. It should flow."};
		p.distribute();
		REQUIRE(p.get_lines() == test);
	}

	SECTION("If a line is super long, and requires that more lines be added, distribute should relieve the excess onto lines added to the end."){
		Paragraph p({"This line is super long and should really be broken up. Longwordlongwordlongword here."}, 15);
		std::vector<Line> test = {"This line is ", "super long and ", "should really ", "be broken up. ", "Longwordlongwor", "dlongword here."};
		p.distribute();
		REQUIRE(p.get_lines() == test);
	}
}
