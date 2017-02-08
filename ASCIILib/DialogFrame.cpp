#include "DialogFrame.h"

#include <string>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <sstream>

#include "Log.h"
#include "StringTokenizer.h"

#include "Game.h"
using namespace ascii;

namespace
{
    const unsigned int LINE_BREAK_AMOUNT = 2;
}


ascii::DialogFrame::DialogFrame(Rectangle frame, DialogStyle* style, ascii::Game* game)
    : mFrame(frame), mTextColor(style->TextColor), mpGame(game),
    mLastCharX(mFrame.x), mLastCharY(mFrame.y),
    mMarkedCharX(0), mMarkedCharY(0),
    // FIXME this code does not account for right-to-left dialog!
    frameStartX(frame.left()), frameStartY(frame.top()),
    frameFinishX(frame.right() - 1), frameFinishY(frame.bottom() - 1),
    mpStyle(style), mDummyCellsPassed(0)
{
}

int ascii::DialogFrame::Width()
{
    return frameFinishX - frameStartX + 1;
}

int ascii::DialogFrame::Height()
{
    return frameFinishY - frameStartY + 1;
}

void ascii::DialogFrame::AddWord(UnicodeString word)
{
    // Count the length of the word, including trailing whitespace
    int lengthWithSpace = word.length();
    // Copy and trim the word of whitespace
    UnicodeString trimmed = word;
    trimmed.trim();
    // Count the length without trailing whitespace
    int length = trimmed.length();

    if (lengthWithSpace == 0 || length == 0)
    {
        // TODO something goes wrong here and I don't know why, but this return
        // statement avoids the bug
        Log::Error("Tried to add an empty word");
        return;
    }

    // Calculate where the word needs to be placed
    unsigned int drawX = mLastCharX;
    unsigned int drawY = mLastCharY;

    // Calculate where the word will terminate if inserted on the current line
    const unsigned int finishX = drawX + length - 1;

    if (finishX > frameFinishX)
    {
        //Log("Wrapping to the next line because of word");
        //ULog(word);
        // Wrap to the next line if necessary
        drawX = frameStartX;
        ++drawY;
    }

    // Add the word (including trailing white-space) as a token mapped with
    // its screen position.
    mWords.push_back(ScrollingWord(word, Point(drawX, drawY), mpStyle));

    // Store the coordinates where the word terminated, so we can place the
    // next word
    mLastCharX = drawX + lengthWithSpace;
    mLastCharY = drawY;
}

void ascii::DialogFrame::AddHeading(UnicodeString heading)
{
    // Make sure the heading will fit on a line
    if (heading.length() > Width())
    {
        Log::Error("Tried to add a heading which was too long to fit on a line: " + heading);
        return;
    }

    // Calculate where the heading needs to be placed
    unsigned int drawX = (frameStartX + frameFinishX) / 2 - heading.length() / 2;
    unsigned int drawY = mLastCharY;

    // Add the word as a token mapped with its screen position.
    mWords.push_back(ScrollingWord(heading, Point(drawX, drawY), mpStyle));
    mLastCharX = drawX + heading.length() - 1;
}

UnicodeString ascii::DialogFrame::AddParagraphFlush(UnicodeString paragraph)
{
    if (mLastCharY > frameFinishY) return paragraph;

    vector<int> wordCounts;
    vector<vector<UnicodeString> > lines;
    vector<int> lineLengths;

    int words = 0;
    int lineLength = 0;

    StringTokenizer tokenizer(paragraph);
    UnicodeString remainder;

    vector<UnicodeString> line;
    while (tokenizer.HasNextToken())
    {
        // Retrieve the next token untrimmed
        UnicodeString word = tokenizer.NextToken(false);

        // Count the token's length trimmed
        UnicodeString trimmed(word);
        trimmed.trim();
        lineLength += trimmed.length();

        if (lineLength <= Width())
        {
            //Log("Adding justified word");
            //ULog(word);
            // Add the token untrimmed
            line.push_back(word);
            // Increase line length to account for untrimmed version before
            // counting the next token
            lineLength += word.length() - trimmed.length();

            //Log("Line length after adding word: ");
            //Log(lineLength);
        }
        else
        {
            lineLength -= trimmed.length();
            if (mLastCharY + lines.size() >= frameFinishY)
            {
                remainder = word + tokenizer.BufferRemainder();
                break;
            }
            else
            {
                wordCounts.push_back(line.size());
                lines.push_back(line);
                if (IsWhiteSpace(line.back()[line.back().length() - 1]))
                {
                    lineLength -= 1;
                }
                lineLengths.push_back(lineLength);

                //Log("-------------------------------");

                //Log("Adding justified word");
                //ULog(word);
                line.clear();
                line.push_back(word);
                lineLength = word.length();
                //Log("Line length after adding word: ");
                //Log(lineLength);
            }
        }
    }

    if (!line.empty())
    {
        lines.push_back(line);
        wordCounts.push_back(line.size());
        lineLengths.push_back(lineLength);
        lineLength = 0;
    }

    for (int i = 0; i < lines.size(); ++i)
    {
        vector<UnicodeString> line = lines[i];
        int wordCount = wordCounts[i];
        int lineLength = lineLengths[i];

        int extraSpaces = Width() - lineLength;

        //Log("Extra spaces: ");
        //Log(extraSpaces);

        vector<int> spaces;
        if (wordCount > 1)
        {
            for (int j = 0; j < wordCount - 1; ++j)
            {
                spaces.push_back(0);
            }

            for (int j = extraSpaces - 1; j >= 0; --j)
            {
                int whichGap = j % spaces.size();
                spaces[whichGap] += 1;
            }
        }
        else
        {
            spaces.push_back(0);
        }

        // The last word has no trailing spaces
        spaces.push_back(0);

        int k = 0;
        
        for (int i = 0; i < line.size(); ++i)
        {
            UnicodeString uword = line[i];
            if (CanFitWord(uword))
            {
                AddWord(uword);
                RevealLetters(uword.length());

                if (lineLength > 3 * Width() / 4)
                {
                    mLastCharX += spaces[k++];
                }
            }
        }
    }

    //Log::Print("Returning a remainder");
    //Log::Print(remainder);
    return remainder;
}

void ascii::DialogFrame::AddSurface(Surface* surface)
{
    // Make sure the surface will fit on a line
    if (surface->width() > Width())
    {
        Log::Error("Tried to add a surface which was too wide to fit in this DialogFrame.");
        return;
    }

    // Make sure the surface will fit vertically in the remaining space
    if (surface->height() > (frameFinishY + 1 - mLastCharY))
    {
        Log::Error("Tried to add a surface which was too tall to fit in this DialogFrame.");
        return;
    }

    // Place the surface in the frame
    Point position(mLastCharX, mLastCharY);
    mSurfaces[position] = surface;

    // Continue filling the DialogFrame one line below the bottom of the surface
    mLastCharY += surface->height();
    HalfLineBreak();
}

UnicodeString ascii::DialogFrame::MockWord(UnicodeString word, UChar mockLetter)
{
    UnicodeString mockWord;
    for (int i = 0; i < word.length(); ++i)
    {
        mockWord += mockLetter;
    }
    return mockWord;
}

bool ascii::DialogFrame::AddMockParagraph(UnicodeString paragraph, UChar mockLetter)
{
    StringTokenizer tokenizer(paragraph);

    while (tokenizer.HasNextToken())
    {
        UnicodeString word = tokenizer.NextToken();
        UnicodeString mockWord = MockWord(word, mockLetter) + " ";

        if (CanFitWord(mockWord))
        {
            AddWord(mockWord);
            RevealLetters(mockWord.length());
        }
        else
        {
            return false;
        }
    }

    if (CanLineBreak())
    {
        LineBreak();
        return true;
    }
    
    return false;
}

void ascii::DialogFrame::FillMockParagraphs(UChar mockLetter)
{
    bool fitLastParagraph = true;
    while (fitLastParagraph)
    {
        // Add a random currently loaded paragraph, converted into the mock
        // character
        UnicodeString paragraph = mpGame->textManager()->GetRandomText(Width()*2);
        fitLastParagraph = AddMockParagraph(paragraph, mockLetter);
    }
}

void ascii::DialogFrame::FillMockParagraphsFlush(UChar mockLetter)
{
    int maxLettersLeft = (frameFinishY - mLastCharY + 1) * Width();
    int letters = 0;

    while (letters < maxLettersLeft)
    {
        UnicodeString paragraph = mpGame->textManager()->GetRandomText(Width()*2);
        StringTokenizer tokenizer(paragraph);

        UnicodeString mockParagraph;
        while (tokenizer.HasNextToken())
        {
            UnicodeString word = tokenizer.NextToken();
            UnicodeString mockWord = MockWord(word, mockLetter);
            mockParagraph += mockWord + " ";
            letters += mockWord.length() + 1;
        }

        AddParagraphFlush(mockParagraph);
        if (CanLineBreak())
        {
            LineBreak();
        }
        else
        {
            ++mLastCharY;
        }
    }
}

void ascii::DialogFrame::LineBreak()
{
    // Don't line break ever if the style doesn't allow it
    if (!mpStyle->LineBreaks)
        return;

    // Only break for a single line if this frame is empty. We'll just assume a
    // parent object wrapped into this frame after filling an old one
    if (mLastCharX == frameStartX && mLastCharY == frameStartY)
    {
        HalfLineBreak();
        return;
    }

    mLastCharX = frameStartX;
    mLastCharY += LINE_BREAK_AMOUNT;
}

void ascii::DialogFrame::HalfLineBreak()
{
    if (mpStyle->LineBreaks)
    {
        mLastCharX = frameStartX;
        mLastCharY += LINE_BREAK_AMOUNT / 2;
    }
}

bool ascii::DialogFrame::CanFitWord(UnicodeString word)
{
    // If we're already past the end of the frame because of a line break,
    // no go.
    if (mLastCharY > frameFinishY) return false;
    // If the word is wider than the frame, no go
    if (word.length() > mFrame.width)
    {
        Log::Error("Tried to add word '" + word + "' to a dialog frame for which it was too wide");
        return false;
    }

    // Calculate the end of the word if placed immediately after the previous
    // one
    word.trim();

    if (word.length() == 0)
    {
        Log::Print("Warning! Trying to add a word composed only of white-space");
    }

    int finishX = mLastCharX + word.length() - 1;

    // If it can't fit on this line, can it fit on the next?
    if (finishX > frameFinishX)
    {
        //Log("Token doesn't fit in this frame:");
        //ULog(word);
        //Log(frameFinishX - finishX);
        //Log(word.length());
        // Fits as long as we can wrap to another line
        return mLastCharY < frameFinishY;
    }

    return true; // It fits on the current line
}

bool ascii::DialogFrame::CanFitHeading()
{
    // 3 lines must remain in order for any text to be fit after the line
    // breaking following the heading
    int linesRequired = LINE_BREAK_AMOUNT + 1;

    return (frameFinishY - mLastCharY + 1 >= linesRequired);
}

bool ascii::DialogFrame::CanLineBreak()
{
    // This doesn't make sense but trust me on it
    if (!mpStyle->LineBreaks) return true;
    // This frame can fit a line break as long as it's above its last line.
    return mLastCharY < frameFinishY;
}

int ascii::DialogFrame::RevealedLetters()
{
    int sum = 0;
    for (int i = 0; i < mWords.size(); ++i)
    {
        sum += mWords[i].RevealedLetters();
    }
    return sum;
}

int ascii::DialogFrame::LettersToReveal()
{
    // Return the greatest number of letters that any child ScrollingWord is
    // missing. The sum is not returned because every ScrollingWord currently
    // in the frame reveals itself simultaneously
    int max = 0;

    for (int i = 0; i < mWords.size(); ++i)
    {
        ScrollingWord word = mWords[i];
        if (word.LettersToReveal() > max)
        {
            max = word.LettersToReveal();
        }
    }

    return max;
}

void ascii::DialogFrame::RevealLetters(int amount)
{
    // Reveal letters on every word that's currently scrolling
    // (This can be used to reveal multiple words at the same time in
    // different places!)
    for (auto it = mWords.begin(); it != mWords.end(); ++it)
    {
        if (!it->Revealed())
        {
            it->RevealLetters(amount);
        }
    }
}

void ascii::DialogFrame::RevealAllLetters()
{
    // Reveal all letters on every word in the message
    for (auto it = mWords.begin(); it != mWords.end(); ++it)
    {
        if (!it->Revealed())
        {
            it->RevealAllLetters();
        }
    }
}

void ascii::DialogFrame::HideLetters(int amount, int dummyCells)
{
    if (mDummyCellsPassed < dummyCells)
    {
        ++mDummyCellsPassed;
        return;
    }

    // Hide letters on every word that is currently being hidden
    // (This can be used to hide multiple words at the same time in different
    // places!)
    for (auto it = mWords.begin(); it != mWords.end(); ++it)
    {
        if (!it->Hidden())
        {
            it->HideLetters(amount);
            // TODO handle hiding all words at once for special dialog
            break;
        }
    }
}

bool ascii::DialogFrame::AllWordsRevealed()
{
    // If no words have been added, then all words are revealed
    if (mWords.empty())
    {
        return true;
    }

    // Otherwise check all of them, until finding one that isn't fully revealed
    for (auto it = mWords.begin(); it != mWords.end(); ++it)
    {
        if (!it->Revealed())
        {
            return false;
        }
    }
    
    // If none is unfinished, then all words are revealed
    return true;
}

bool ascii::DialogFrame::AllWordsHidden()
{
    // If no words have been added, then all words are hidden
    if (mWords.empty())
    {
        return true;
    }

    // Otherwise check all of them, until finding one that isn't fully hidden
    for (auto it = mWords.begin(); it != mWords.end(); ++it)
    {
        if (!it->Hidden())
        {
            return false;
        }
    }

    // If none aren't fully hidden, all are fully hidden
    return true;
}

void ascii::DialogFrame::Clear()
{
    // clear all scrolling words
    mWords.clear();
    // clear all surface
    mSurfaces.clear();

    // move the "cursor" back to the beginning
    mLastCharX = frameStartX;
    mLastCharY = frameStartY;
}

void ascii::DialogFrame::Draw(Graphics& graphics, Preferences* config)
{
    // Draw every scrolling word
    for (auto it = mWords.begin(); it != mWords.end(); ++it)
    {
        it->Draw(graphics);
    }

    // Draw every surface
    for (auto it = mSurfaces.begin(); it != mSurfaces.end(); ++it)
    {
        graphics.blitSurface(it->second, it->first.x, it->first.y);
    }
   
    // If debug view is enabled, display the text frame's bounding
    // rectangle visually:
    if (config->GetBool("debug-view"))
    {
        for (int x = frameStartX; x <= frameFinishX; ++x)
        {
            for (int y = frameStartY; y <= frameFinishY; ++y)
            {
                graphics.setBackgroundColor(x, y, Color::Red);
            }
        }
    }
}

void ascii::DialogFrame::DrawCursor(Graphics& graphics)
{
    // If this frame's DialogStyle requires a cursor, draw it following all
    // text that has been revealed
    if (mpStyle->HasCursor)
    {
        Point cursorPosition(mLastCharX, mLastCharY);
        if (!mWords.empty())
        {
            cursorPosition = mWords.back().NextCell(); 

            // Draw on the next line if it would appear past the end of this
            // one
            if (cursorPosition.x > frameFinishX)
            {
                cursorPosition.x = frameStartX;
                cursorPosition.y += 1;
            }
        }

        graphics.setBackgroundColor(cursorPosition.x, cursorPosition.y,
                mpStyle->CursorColor);
    }
    
}

bool ascii::DialogFrame::HasWords()
{
    // This might be some hackish trickery
    return !mWords.empty() || !mSurfaces.empty();
}

void ascii::DialogFrame::MarkPosition()
{
    mMarkedCharX = mLastCharX;
    mMarkedCharY = mLastCharY;
}

void ascii::DialogFrame::RewindPosition()
{
    mLastCharX = mMarkedCharX;
    mLastCharY = mMarkedCharY;
}
