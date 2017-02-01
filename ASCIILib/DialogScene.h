#pragma once

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "unicode/unistr.h"
using namespace icu;

#include "Graphics.h"
#include "Rectangle.h"
#include "Color.h"
#include "Surface.h"
#include "Point.h"
using namespace ascii;

#include "Preferences.h"

#include "DialogFrame.h"
#include "DialogStyle.h"


namespace ascii
{
class Game;


// Represents the direction in which bubble frames for this DialogScene will
// stretch to accomodate their text
enum StretchDirection
{
    STRETCH_DOWN,
    STRETCH_UP,
    STRETCH_RIGHT,
    STRETCH_LEFT
};

// This class manages multiple DialogFrames. It can reveal dialog piece by piece,
// placing new words inside the first frame with enough empty space to fit
// them.
class DialogScene
{
    public:
        // Create a dialog scene with no frames and no text
        DialogScene(DialogStyle* style, Game* game);

        // Add a frame to the scene
        void AddFrame(Rectangle frame, bool needsPadding=true);
        // Add a frame at a position relative to the last one's boundaries
        void AddRelativeFrame(int xOffset, int yOffset, int width, int height);

        // Measure the bubble frame required to fit the given paragraph
        Rectangle MeasureBubbleFrame(UnicodeString paragraph);

        // Set the scene with only a DialogFrame sized to fit the given
        // paragraph in a framed bubble of the current style
        void SetBubbleFrame(UnicodeString paragraph);

        // Add a word to the first suitable frame in the scene, or clear all
        // frames and start over
        void AddWord(UnicodeString word);
        // Add a centered heading to the first suitable frame in the scene, or
        // clear all frames and start over
        void AddHeading(UnicodeString heading);
        // Add a surface to the current dialog frame (TODO this is not intended to
        // be used in the middle of dynamic dialog flow)
        void AddSurface(Surface* surface);

        // Add an paragraph instantly, making sure the edges of the text are
        // flush with both edges of the frame
        UnicodeString AddParagraphFlush(UnicodeString paragraph);

        void AddMockParagraph(UnicodeString paragraph, UChar mockLetter);

        // Fill the rest of the scene's frames with mock paragraphs formed by
        // repeating the given letter in fake words
        void FillMockParagraphs(UChar mockLetter);

        // Whether the dialog scene has any frames
        bool HasFrames() { return !mFrames.empty(); }

        // Whether this dialog scene has room to fit a given word, or
        // will need to be cleared first.
        bool CanFitWord(UnicodeString word) { return FrameForWord(word) != NULL; }
        // Whether this dialog scene has room to fit a given heading, or
        // will need to be cleared first.
        bool CanFitHeading() { return FrameForHeading() != NULL; }
        // Whether this dialog scene has room to fit a line break, or
        // will need to be cleared first.
        bool CanFitLineBreak() { return FrameForLineBreak() != NULL; }

        // Whether the scene is filled and must be cleared
        bool Filled() { return mFilled; }

        // Clear every frame of its text
        void ClearAllFrames();

        // Reset every frame
        void ResetAllFrames();

        // Remove all text frames
        void DeleteFrames();

        // Insert a line break after the most recently added word in the text
        // frame. This will become half a line break if the line break comes
        // immediately at the top of a new frame.
        void LineBreak();

        // Check whether all words in this dialog scene are fully revealed
        bool AllWordsRevealed();
        // Check whether all DialogFrames have hidden all of their words
        bool AllWordsHidden();

        // How many letters this DialogScene still needs to reveal
        int LettersToReveal();

        // How many letters this DialogScene has already revealed
        int RevealedLetters();

        // Reveal a number of letters from every word currently being revealed
        // in the message
        void RevealLetters(int amount);

        // Reveal every letter that is still hidden
        void RevealAllWords();

        // Hide a number of letters from every word currently being hidden in
        // the message
        void HideLetters(int amount);

        // Draw every text frame of this dialog scene.
        void Draw(Graphics& graphics, Preferences* config);
        // Draw a cursor at the next position where a letter will appear
        void DrawCursor(Graphics& graphics);

        void SetBubbleFixedValues(int v1, int v2, int v3);
        void SetStretchDirection(string direction);

        bool HasWords();

        void MarkPosition();
        void RewindPosition();

    private:
        // Helper method to determine the first frame which can fit a word
        DialogFrame* FrameForWord(UnicodeString word);
        // Helper method to determine the first frame which can fit a heading
        DialogFrame* FrameForHeading();
        // Helper method to determine the first frame which can fit a line
        // break
        DialogFrame* FrameForLineBreak();
        // Helper method to determine the first frame which can fit half of
        // a line break
        DialogFrame* FrameForHalfLineBreak();

        DialogStyle* mStyle;
        vector<DialogFrame> mFrames;
        map<Point, Surface*> mFrameSurfaces;

        int mCurrentFrame;
        // Flag to save a line break for after the scene is cleared.
        bool mSaveLineBreak;
        // Flag to wait and clear this scene before adding more words
        bool mFilled;

        StretchDirection mStretchDirection;

        int mBubbleValue1;
        int mBubbleValue2;
        int mBubbleValue3;

        static Rectangle msLastFrame;
        Game* mpGame;
};
    
}
