#include "Button.h"

namespace
{
    const int TEXT_PADDING_X = 2;
}


ascii::Button::Button(UnicodeString text, Rectangle bounds, Alignment alignment,
        Color textColor, Color textColorSelected, Color boxColor,
        Color boxColorSelected, string actionKey, bool keyboardEnabled) 
    : mHidden(false)
{
    // Initializer list not used for sake of clarity
    mText = text;
    mBounds = bounds;

    mTextColor = textColor;
    mTextColorSelected = textColorSelected;
    mBoxColor = boxColor;
    mBoxColorSelected = boxColorSelected;

    mActionKey = actionKey;
    mKeyboardEnabled = keyboardEnabled;

    mAlignment = alignment;

    // Adjust bounding box to reflect alignment
    switch (alignment)
    {
        case ALIGN_LEFT: break;

        case ALIGN_RIGHT:
            mBounds.x -= mBounds.width;
            break;

        case ALIGN_CENTER:
            mBounds.x -= mBounds.width / 2;
            mBounds.y -= mBounds.height / 2;
            break;
    }
}

bool ascii::Button::IsSelected(int cursorX, int cursorY)
{
    // The button is never selected if it is hidden
    if (mHidden) return false;

    // Check if the cell selected by the cursor is contained by the button's
    // bounding rectangle
    return mBounds.left() <= cursorX && mBounds.top() <= cursorY
        && mBounds.right() > cursorX && mBounds.bottom() > cursorY;
}

void ascii::Button::Draw(Graphics& graphics, bool selected) const
{
    // Don't draw the button if it's hidden
    if (mHidden) return;

    // Draw the box
    const Color boxColor = (selected ? mBoxColorSelected : mBoxColor);

    // For buttons whose text is more than one character (in order to exclude
    // "-" and "+" who deserve cute little tiny buttons) stretch button width
    // to fit text, if required to
    Rectangle bounds = mBounds;

    int extraWidth = ((TEXT_PADDING_X * 2) + mText.length() - mBounds.width - 1);

    if (mText.length() > 1 && extraWidth > 0)
    {
        bounds.width += extraWidth;
        if (mAlignment == ALIGN_RIGHT)
        {
            bounds.x -= extraWidth;
        }
    }

    graphics.fillRect(bounds, ' ', boxColor, Color::Black);

    // Position the button text
    int textX = mBounds.x;
    // Centered vertically on the box whether center aligned or no
    int textY = mBounds.y + mBounds.height / 2;

    // Position the text inside the bounding box with the proper alignment
    switch (mAlignment)
    {
        case ALIGN_LEFT:
            // Padded from the left side of the box
            textX += TEXT_PADDING_X;
            break;

        case ALIGN_CENTER:
            // Centered horizontally inside the box
            textX = (mBounds.x + mBounds.width / 2) - (mText.length() / 2);
            break;
            
        case ALIGN_RIGHT:
            // Padded from the right side of the box
            textX += mText.length();
            textX -= TEXT_PADDING_X;
            break;
    }

    const Color textColor = (selected ? mTextColorSelected : mTextColor);
    graphics.blitString(mText, textColor, textX, textY);
}
