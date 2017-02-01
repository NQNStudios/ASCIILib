#pragma once

#include "unicode/utypes.h"
#include "unicode/unistr.h"
using namespace icu;

#include "Graphics.h"
#include "Point.h"
using namespace ascii;

#include "DialogStyle.h"


// Contains a word which is being revealed or hidden through
// scrolling characters. Words are hidden from the same side as they scroll
class ScrollingWord
{
    public:
        // Construct a ScrollingWord.
        ScrollingWord(UnicodeString word, Point position, DialogStyle* style);

        // Reveal characters. Changes the state of the word to "revealing"
        // which will affect its appearance if done while hiding.
        void RevealLetters(unsigned int amount);
        // Reveal all hidden letters.
        void RevealAllLetters();
        // How many letters are still hidden
        int LettersToReveal();
        // How many letters are already revealed
        int RevealedLetters() { return mRevealedChars; }
        // Hide characters. Changes the state of the word to "hiding"
        // which will affect its appearance if called while revealing.
        void HideLetters(unsigned int amount);

        // Whether the word is totally revealed
        bool Revealed() { return mRevealedChars == mWord.length(); }
        // Whether the word is totally hidden
        bool Hidden() { return mRevealedChars == 0; }

        // Draw as much of the word as must be revealed.
        void Draw(Graphics& graphics);

        // Return the cell immediately following the final cell where this
        // ScrollingWord will draw a character
        Point NextCell();

    private:
        UnicodeString mWord;
        int mRevealedChars;

        DialogStyle* mpStyle;
        Point mPosition;

        bool mRevealing;
};
