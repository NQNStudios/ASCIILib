#include "Label.h"

#include "Log.h"
using namespace ascii;


Label::Label(UnicodeString text, Point position, Alignment alignment, Color textColor)
    : mText(text), mPosition(position), mAlignment(alignment),
    mTextColor(textColor)
{
}

Label::Label(UnicodeString text, Rectangle bounds, Color textColor)
    : mText(text), mBounds(bounds), mTextColor(textColor)
{
}

void Label::Draw(Graphics& graphics) const
{
    // Draw text across multiple lines bounded by mBounds
    if (mBounds.width > 0 && mBounds.height > 0)
    {
        graphics.blitStringMultiline(mText, mTextColor, mBounds);
    }
    // Draw text on one line
    else
    {
        Point position = mPosition;

        // Adjust the position to reflect text alignment
        switch (mAlignment)
        {
            case ALIGN_LEFT: break;

            case ALIGN_RIGHT:
                position.x -= mText.length();
                break;

            case ALIGN_CENTER:
                position.x -= mText.length() / 2;
                break;
        }

        // Draw the text in the given color at the set position
        graphics.blitString(mText, mTextColor, position.x, position.y);
    }
}
