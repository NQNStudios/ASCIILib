#pragma once

#include "unicode/utypes.h"

#include "Graphics.h"
#include "Color.h"
using namespace ascii;

#include "Alignment.h"


// A button in a game menu.
class Button
{
    public:
        // Create a UI button.
        Button(UnicodeString text, Rectangle bounds, Alignment alignment,
                Color textColor, Color textColorSelected, Color boxColor,
                Color boxColorSelected, string actionKey, bool keyboardEnabled);

        // Check if the button is selected by the mouse cursor.
        bool IsSelected(int cursorX, int cursorY);

        // Draw the button box and text
        void Draw(Graphics& graphics, bool selected) const;

        // Key of the UIAction to perform when the button is triggered
        string ActionKey() { return mActionKey; }

        // Whether the button can be selected with the keyboard
        bool KeyboardEnabled() { return mKeyboardEnabled; }

        // Hide the button
        void Hide() { mHidden = true; }
        // Show the button if it's hidden
        void Show() { mHidden = false; }

        // Return the text of the button
        UnicodeString GetText() { return mText; }
        // Set the button's text
        void SetText(UnicodeString value) { mText = value; }

    private:
        UnicodeString mText;
        Rectangle mBounds;
        Alignment mAlignment;

        Color mTextColor;
        Color mTextColorSelected;
        Color mBoxColor;
        Color mBoxColorSelected;

        string mActionKey;

        bool mKeyboardEnabled;
        bool mHidden;
};
