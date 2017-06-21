#include "Paragraph.hpp"

// Bitwise OR
inline Format Format::operator|(Format a, Format b){
	return static_cast<char>(a) | static_cast<char>(b);
}

// Bitwise AND
inline Format Format::operator&(Format a, Format b){
	return static_cast<char>(a) & static_cast<char(b);
}

// Bitwise XOR
inline Format Format::operator^(Format a, Format b){
	return static_cast<char>(a) ^ static_cast<char(b);
}

Paragraph::Paragraph(): line_width(lw){
	;
}

void Paragraph::insert_ch(std::string::size_type index, char ch){
	text.insert(index, 1, ch);
	update_line_markers();
}

char Paragraph::delete_ch(std::string::size_type index){
	char ch = text[index];
	text.erase(index, 1);
	update_line_markers();
	return ch;
}

// Without modifiers included, those are separate.
std::vector<std::string> get_lines();

// Returns a copy, to avoid unintentional modification.
std::vector<FormatModifier> get_modifiers ();

// Returns the type of the update done to this paragraph in the last char read.
// Needs to be cleared before each cycle.
UpdateType get_last_update();
void clear_last_update();

void set_line_width(std::string::size_type lw);

void update_line_markers();





void Paragraph::set_line_width(std::string::size_type lw){
	line_width = lw;
	update_line_markers();
}	

// How should this work? I have to find the last bit of whitespace before the character limit, and break there. If there is no whitespace, I break the word.l
// But it should be the first whitespace character in that bit of whitespace. That makes it harder.
// The index of that bit of whitespace is the end marker.
// Where does the next marker start? on the whitespace? nah that wouldn't make sense. Starts on the next non-whitespace character, that makes the most sense.
// So actually this shouldn't be a vector of ints, it should be a vector of pairs.
// Yea, thats what google drive does, just breaks the word. that makes sense. Doesn't even break on punctation, just the word.
// May at some point need to change over to wstring or something like it.
//
// TEST THIS
// AND CONSIDER WHETHER I SHOULD REALLY JUST DO STRINGS INSTEAD OF MARKERS, IT MIGHT BE MORE EFFICIENT TO SHUFFLE AMONG STRINGS WHEN ONE STRING OVERFLOWS.
// PROBABLY IS MORE EFFICIENT.
// TRY THIS FOR NOW.
// Because here, I have to update the line markers every single time I add text.
// If I were using strings, I would only have to shuffle when one string exceeded its boundaries, and then the effects could be limited.
// Yeah, this would actually be quite simple if I were using strings.
//
// In the debate between the two, I think the question comes down to display. Which will make it easier to  display hte lines?
// to dispaly the lines, it will be easiest if the formatting is available per line.
// That implies that i should use strings, and store formatting per line.
// But how would I use the formatmodifiers? i would have to pass all of them with each string, and it would be hard for a display manager
// to know which modifier applied to what. I suppose it could prepare a line to print, then examine every single modifier and insert control characters.
// But it might be better for these strings i use to use my own custom character, which stores its formatting.
// the formatting would be as an enum, binary style, using and and or to retrieve and store values.
// and the paragraph itself stores header style. 
// Actually they would have to be vectors of my own type, because I don't know how to implement string with my own type.
//
// But before I do that, I should rule out the control character idea.
// I would use attr_on. Which one does that conform to more closely?
// honestly it seems to conform to the modifiers more closely.
// Lets to modifiers with strings, modifiers are for each string, not for the whole paragraph.
//
// Actually I should probably wait to change this until I actually implement the display.
//
// In any case, each paragraph's lines are always kept internally consistent.
// Using markers, an external agent can determine the correct index for a given cursor position. You simply find the right set of markers, then increment from the start.
//
// idea: 
// 
void Paragraph::update_line_markers() {
	std::vector<std::pair<std::string::size_type, std::string::size_type>> temp_markers;

	// With no text, we have an empty set of markers.
	if (text.size() == 0){
		line_markers.swap(temp_markers);
		return;
	}

	std::string::size_type start = 0;
	std::string::size_type end;

	// This function needs a lot of guards, like if the text is totally whitespace or zero whitespace.
	// Put THAT IN.
	// What if the text is empty?
	// What if its a single character?
	// If its all whitespace, we also use an empty set of markers.
	while (true){

		// start is inclusive, so this doesn't fly.
		if (start == text.size()){
			line_markers.swap(temp_markers);
			return;
		}

		while (isspace(text[start])){
			start++;
		 	
		 	// This means the remainder of the text was whitespace.
			if (start == text.size()) {
				line_markers.swap(temp_markers);
				return;
			}
		}

		// At this point we know that start points to a non-whitespace character and we have at least one marker yet to push in.

		end = start + line_width;
		
		if (end >= text.size()){
			end = text.size();
			temp_markers.push_back(std::pair<std::string::size_type, std::string::size_type> (start, end));
			line_markers.swap(temp_markers);
			return;
		}

		// We have to end the line right before a whitespace character.
		while (!isspace(text[end])){
			end--;

			// Maybe there is no whitespace in this entire line.
			if (end == start){
				end = start + line_width;
				break;
			}
		}

		// Only do this if this was a line with whitespace in it.
		if (isspace(text[end])){

			// But if there are several whitespace characters in a row, we must end on the first of those.
			while(isspace(text[end]))
				end--;

			// We overshoot by one.
			end++;
		}

		temp_markers.push_back(std::pair<std::string::size_type, std::string::size_type> (start, end));
		start = end;
	}
}

std::vector<std::string> Paragraph::get_lines() {
	std::vector<std::string> ret;

	for (std::pair<std::string::size_type, std::string::size_type> pr : line_markers){
		ret.push_back(text.substr(pr.first, pr.second - pr.first));
	}

	return ret;
}

std::string::size_type get_index(int line, std::string::size_type x){
	if (line >= line_markers.size()){
		return -1;
	}

	std::pair<std::string::size_type, std::string::size_type> pr = line_markers[line];

	if (x >= pr.second - pr.first){
		return -1;
	}

	else {
		return pr.first + x;
	}
}


// dilemma: if I don't store lines in a paragraph, then everytime I need to interpret a new cursor position I will have to extract ALL the lines from a paragraph,
// or at least all lines until the cursor position.
// That feels so very wrong.
// So lets store lines, and it is possible to reinterpret them to a new line length.
// Or maybe I should still store all the text, and just hold marker to beginnings and ends of lines?o
// That sorta makes more sense. Then, when I add new characters, I don't actually have to shuffle around characters between strings, which is wasteful and awkward.
// Instead, I simply adjust the positions of the markers.
