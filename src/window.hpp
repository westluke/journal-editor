#ifndef WINDOW_HPP
#define WINDOW_HPP

// And I can wrap the display functions into this class!
// How will the cursor work? How will it keep track of that?
// It will have to move through the Document in sorta weird ways. And that will slow things down, probably.
// Moving around won't be a problem, I imagine.
// But wait, now I'm "inserting" using the window? Or I have a direct reference to the document?
// Either one of those is bad.
// Or the window handles input, which is also sorta weird. But doable.
// So I could move most of the main functionality into this new Window class. That's basically what this would do.
// Or I could just have Documents keep track of cursor / window. Let's do the simple option for now.

class Window{
}
