#pragma once

#include <vector>
#include <map>
using namespace std;

#include "unicode/utypes.h"
#include "unicode/unistr.h"

#include "Rectangle.h"
#include "Color.h"
#include "Graphics.h"
#include "Surface.h"
#include "Point.h"
using namespace ascii;

#include "Preferences.h"

#include "ScrollingWord.h" 

namespace ascii
{
    class Game;

    // This class stores and renders text and other dialog objects inside a frame,
    // exposing useful information such as whether the frame is filled and also allowing
    // the text to be revealed piece by piece.
    class DialogFrame
    {
        public:
            // Construct an empty DialogFrame
            DialogFrame(ascii::Rectangle frame, DialogStyle* style, Game* game);

            // Append a word to the text frame's message. Remember to first check
            // whether the word will fit using CanFitWord(). Also note that this
            // function reveals the previously added word, even if it hasn't yet
            // been fully revealed, so make sure to also check AllWordsRevealed()
            void AddWord(UnicodeString word);
            // Add a centered heading to the text frame's message
            void AddHeading(UnicodeString heading);

            // Add a paragraph of words so that both sides of the text are flush
            // with the frame's edges. Return the portion of the paragraph which
            // does not fit in this frame
            UnicodeString AddParagraphFlush(UnicodeString paragraph);

            // Add a surface at the current position, wrapping lines to continue
            // text beneath it
            void AddSurface(Surface* surface);

            // Mask a paragraph's text by replacing its letters with mockLetter,
            // then add the paragraph instantly
            bool AddMockParagraph(UnicodeString paragraph, UChar mockLetter);

            // Fill the rest of this frame with mock paragraphs formed by repeating
            // the given character
            void FillMockParagraphs(UChar mockLetter);

            // Fill the rest of this frame with mock paragraphs formed by repeating
            // the given character while keeping text justified with both sides
            void FillMockParagraphsFlush(UChar mockLetter);

            // Insert a line break in the text frame's message.
            void LineBreak();
            // Insert half a line break in the text frame's message. Used for when
            // a line break falls at the top of the frame.
            void HalfLineBreak();

            // Check whether this text frame can fit a given word
            bool CanFitWord(UnicodeString word);
            // Check whether this text frame can fit a heading centered on one line
            bool CanFitHeading();
            // Check whether this text frame can fit a line break
            bool CanLineBreak();
            // Check whether this text frame can fit half a line break
            bool CanHalfLineBreak();

            // Check whether all words in this text frame have been fully revealed
            bool AllWordsRevealed();
            // Check whether all words in this text frame are fully hidden
            bool AllWordsHidden();

            // How many letters this DialogFrame has left to reveal.
            int LettersToReveal();

            // Reveal a number of letters at the end of the TextFrame's visible
            // message
            void RevealLetters(int amount);
            // Reveal every hidden letter
            void RevealAllLetters();
            // Hide a number of letters at the beginning of the TextFrame's revealed
            // message. If dummyCells is provided, force the frame to call
            // HideLetters(1) dummyCells amount of times before actually hiding
            // anything
            void HideLetters(int amount, int dummyCells=0);

            void MarkFilled() { mLastCharY = frameFinishY + 1; }

            // Clear all words from this frame
            void Clear();
            // Draw the text and surfaces in this frame to the graphics buffer
            void Draw(Graphics& graphics, Preferences* config);
            // Draw a cursor at the position where characters will next appear
            void DrawCursor(Graphics& graphics);

            int Width();
            int Height();

            Rectangle Bounds() { return mFrame; }

            bool HasWords();

            int RevealedLetters();

            void MarkPosition();
            void RewindPosition();

        private:
            Game* mpGame;
            UnicodeString MockWord(UnicodeString word, UChar letter);
            Rectangle mFrame;
            Color mTextColor;

            int frameStartX;
            int frameStartY;
            int frameFinishX;
            int frameFinishY;

            int mLastCharX;
            int mLastCharY;

            int mMarkedCharX;
            int mMarkedCharY;


            vector<ScrollingWord> mWords;
            map<Point, Surface*> mSurfaces;
            DialogStyle* mpStyle;

            int mDummyCellsPassed;
    };

}
