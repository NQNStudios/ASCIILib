#include "ScrollingWord.h"

#include <sstream>

#include "Log.h"
using namespace ascii;


ScrollingWord::ScrollingWord(UnicodeString word, Point position, DialogStyle* style)
    : mWord(word), mPosition(position), mRevealedChars(0), mRevealing(false),
    mpStyle(style)
{
}

int ScrollingWord::LettersToReveal()
{
    return mWord.length() - mRevealedChars;
}

void ScrollingWord::RevealLetters(unsigned int amount)
{
    mRevealedChars += amount;
    // Be careful not to reveal more than exists. That's undefined behavior
    if (mRevealedChars > mWord.length())
    {
        mRevealedChars = mWord.length();
    }
    // Mark that this word is currently being revealed
    mRevealing = true;
}

void ScrollingWord::RevealAllLetters()
{
    mRevealedChars = mWord.length();
}

void ScrollingWord::HideLetters(unsigned int amount)
{
    mRevealedChars -= amount;
    // Be careful not to go negative. That's DEFINITELY undefined behavior
    if (mRevealedChars < 0)
    {
        mRevealedChars = 0;
    }
    // Mark that this word is currently being hidden
    mRevealing = false;
}

void ScrollingWord::Draw(Graphics& graphics)
{
    int drawX = mPosition.x;
    int drawY = mPosition.y;

    int firstLetter = 0;

    // If we're hiding, skip letters from the start so as to wipe from the same
    // direction as words were revealed
    if (!mRevealing)
    {
        const int hiddenChars = mWord.length() - mRevealedChars;
        drawX += hiddenChars;
        firstLetter = hiddenChars;
    }

    for (int c = 0; c < mRevealedChars; ++c)
    {
        graphics.setCharacter(drawX, drawY, mWord[c + firstLetter]);
        graphics.setCharacterColor(drawX, drawY, mpStyle->TextColor);
        ++drawX;
    }
}

Point ScrollingWord::NextCell()
{
    Point next = mPosition;
    next.x += mRevealedChars;
    return next;
}
