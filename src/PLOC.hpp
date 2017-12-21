#ifndef PLOC_HPP
#define PLOC_HPP

#include "line.hpp"
#include "text.hpp"


// Defines a location within a paragraph; a line, and a character within that line.
// What if PLOC were a full description? Not just line_number, but line number within that paragraph, and also the length of that paragraph,
// and also the length of the particular line in question.
// WAIT, but PLOC doesn't belong in paragraph, so this is FINE!
// Ok, we need a structure for indexing into paragraphs, and a structure for indexing into documents.
// text_index indexes into lines. PLOC indexes into paragraphs. DOCLOC indexes into documents?


#endif
