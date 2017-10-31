#include "../src/text.hpp"
#include "../lib/catch.hpp"
#include <string>

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
