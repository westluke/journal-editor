#include "../src/line.hpp"
#include "../lib/catch.hpp"
#include <string>

// ACCEPT THAT MY TESTING IS NECESSARILY LIMITED SO I JUST HAVE TO DEBUG

std::vector<std::string> text_type_to_string(const std::vector<text_type> &txts){
	std::vector<std::string> ret;

	for (int i = 0; i < txts.size(); i++){
		ret.push_back(text_type_to_string(txts[i]));
	}

	return ret;
}

TEST_CASE("Can read size of a Line"){
	Line empty = Line();
	Line onech = Line("a");
	Line onespace = Line(" ");
	Line l0 = Line("  yo  ");

	REQUIRE(empty.size() == 0);
	REQUIRE(onech.size() == 1);
	REQUIRE(onespace.size() == 1);
	REQUIRE(l0.size() == 6);
}

TEST_CASE("Can read a character from a Line"){
	Line onech = Line("a");
	Line onespace = Line(" ");
	Line l0 = Line("hello there");

	REQUIRE(onech.get_ch(0) == fchar('a'));
	REQUIRE(onespace.get_ch(0) == fchar(' '));
	REQUIRE(l0.get_ch(0) == fchar('h'));
	REQUIRE(l0.get_ch(10) == fchar('e'));
}

TEST_CASE("Can retrieve all text, in one of two ways"){
	Line empty = Line();
	Line onech = Line("a");
	Line onespace = Line(" ");
	Line l0 = Line("hello there");

	REQUIRE(empty.get_text() == string_to_text_type(""));
	REQUIRE(onech.get_text() == string_to_text_type("a"));
	REQUIRE(onespace.get_text() == string_to_text_type(" "));
	REQUIRE(l0.get_text() == string_to_text_type("hello there"));

	REQUIRE(l0.get_text(0, 11) == string_to_text_type("hello there"));
	REQUIRE(l0.get_text(0, 0) == string_to_text_type(""));
	REQUIRE(l0.get_text(0, 1) == string_to_text_type("h"));
}

TEST_CASE("Can convert to string"){
	Line empty = Line();
	Line onech = Line("a");
	Line onespace = Line(" ");
	Line l0 = Line("hello there");

	REQUIRE(empty.get_string() == "");
	REQUIRE(onech.get_string() == "a");
	REQUIRE(onespace.get_string() == " ");
	REQUIRE(l0.get_string() == "hello there");
}

TEST_CASE("Can insert character"){
	Line empty = Line();
	Line onech = Line("a");
	Line onespace = Line(" ");
	Line l0 = Line("hello there");

	empty.insert_ch(0, fchar('a'));
	onech.insert_ch(0, fchar('4'));
	onespace.insert_ch(1, fchar('B'));
	l0.insert_ch(0, fchar('-'));

	REQUIRE(empty == Line("a"));
	REQUIRE(onech == Line("4a"));
	REQUIRE(onespace == Line(" B"));
	REQUIRE(l0 == Line("-hello there"));
}

TEST_CASE("Can delete character"){
	Line onech = Line("a");
	Line l0 = Line("hello there");

	onech.delete_ch(0);
	l0.delete_ch(10);

	REQUIRE(onech == Line(""));
	REQUIRE(l0 == Line("hello ther"));
}

TEST_CASE("Can append text"){
	Line empty = Line();
	Line onech = Line("a");
	Line l0 = Line("hello there");

	empty.append_text(string_to_text_type("1!"));
	onech.append_text(string_to_text_type("yooo"));
	l0.append_text(string_to_text_type("HI"));

	REQUIRE(empty == Line("1!"));
	REQUIRE(onech == Line("ayooo"));
	REQUIRE(l0 == Line("hello thereHI"));
}

TEST_CASE("Can prepend text"){
	Line empty = Line();
	Line onech = Line("a");
	Line l0 = Line("hello there");

	empty.prepend_text(string_to_text_type("y0"));
	onech.prepend_text(string_to_text_type("<>"));
	l0.prepend_text(string_to_text_type("HHH"));

	REQUIRE(empty == Line("y0"));
	REQUIRE(onech == Line("<>a"));
	REQUIRE(l0 == Line("HHHhello there"));
}

TEST_CASE("Can delete text"){
	Line onech = Line("a");
	Line onech2 = Line("T");
	Line l0 = Line("hello there");

	onech.delete_text(0, 0);
	onech2.delete_text(0, 1);
	l0.delete_text(2, 6);

	REQUIRE(onech == Line("a"));
	REQUIRE(onech2 == Line(""));
	REQUIRE(l0 == Line("hethere"));
}

TEST_CASE("Can detect whether lines can equalize"){
	Line empty = Line("");
	Line longline = Line("Long ");

	Line space = Line(" ");
	Line onech = Line("L");
	Line twoch = Line("LL");
	Line l0 = Line("1Space 2Spaces  3   ");
	Line l1 = Line("   3Space  2Spaces 1");
	Line l2 = Line(" Word0 Word1 Word2 ");

	SECTION("Flowback"){
		// Need more false cases
		// Need to check can_equalize as well.
		REQUIRE(empty.can_accept_flowback(space, 1) == true);
		REQUIRE(empty.can_accept_flowback(onech, 1) == true);
		// THIS ONE IS A PROBLEM
		REQUIRE(empty.can_accept_flowback(twoch, 1) == true);
		REQUIRE(empty.can_accept_flowback(twoch, 2) == true);

		REQUIRE(longline.can_accept_flowback(space, 1) == true);
		REQUIRE(longline.can_accept_flowback(onech, 1) == false);
		REQUIRE(longline.can_accept_flowback(onech, 6) == true);
		REQUIRE(longline.can_accept_flowback(onech, 2000) == true);
	}

	SECTION("Excess"){
		REQUIRE(empty.exceeds_width_non_whitespace(5) == false);
		REQUIRE(longline.exceeds_width_non_whitespace(4) == false);
		REQUIRE(longline.exceeds_width_non_whitespace(3) == true);
	}
}

/*
TEST_CASE("Can actually equalize lines"){
	Line empty = Line("");
	Line longline = Line("Long ");

	Line space = Line(" ");
	Line onech = Line("L");
	Line twoch = Line("LL"):

	Line empty = Line("");
	Line longline = Line("Long ");

	Line space = Line(" ");
	Line onech = Line("L");
	Line twoch = Line("LL"):
	Line l0 = Line("1Space 2Spaces  3   );
	Line l1 = Line("   3Space  2Spaces 1");
	Line l2 = Line(" Word0 Word1 Word2 ");

}

TEST_CASE("Can split text into vector of words and whitespaces"){
}
*/
