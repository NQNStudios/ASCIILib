#pragma once

#include "unicode/utypes.h"

#include "Color.h"
#include "Point.h"
#include "Graphics.h"
using namespace ascii;

#include "Alignment.h"


// Defines static, stationary text in a game menu
class Label
{
    public:
        // Create a UI label
        Label(UnicodeString text, Point position, Alignment alignment, Color textColor);
        // Create a multiline UI label
        Label(UnicodeString text, Rectangle bounds, Color textColor);

        // Draw the label
        void Draw(Graphics& graphics) const; 

        UnicodeString Text() { return mText; }
        Point Position() { return mPosition; }

    private:
        UnicodeString mText;

        Point mPosition;
        Alignment mAlignment;

        Color mTextColor;

        Rectangle mBounds;
};
