#include "DialogScene.h"

#include <sstream>

#include "Surface.h"
#include "Game.h"
using namespace ascii;


// Static redeclaration
Rectangle ascii::DialogScene::msLastFrame = Rectangle();


ascii::DialogScene::DialogScene(DialogStyle* style, Game* game)
    : mStyle(style), mCurrentFrame(0), mFilled(false), mSaveLineBreak(false),
    mpGame(game),
    // Dialog bubbles stretch downward by default
    mStretchDirection(STRETCH_DOWN)
{
}

void ascii::DialogScene::AddFrame(Rectangle frame, bool needsPadding)
{
    // Output the frame's coordinates for debugging
    //stringstream output;
    //output << "Adding dialog frame: (" << frame.x << ", " << frame.y << ", " << frame.width << ", " << frame.height << ")";
    //Log::Print(output.str());

    // Save the rectangle as msLastFrame so it can be used to add relative
    // frames after this
    msLastFrame = frame;

    Rectangle paddedFrame = frame;
    if (needsPadding)
    {
        // Apply text padding and create a DialogFrame object
        paddedFrame = Rectangle(
                frame.x + mStyle->PaddingX,
                frame.y + mStyle->PaddingY,
                frame.width - mStyle->PaddingX * 2,
                frame.height - mStyle->PaddingY * 2);
    }

    mFrames.push_back(DialogFrame(paddedFrame, mStyle, mpGame));

    // If the style of this DialogScene has a frame, generate the surface and
    // store it next to the unpadded location of the new frame, where it will
    // frame the padded rectangle
    if (mStyle->IsFramed)
    {
        mFrameSurfaces[Point(frame.x, frame.y)] =
            mStyle->MakeFrame(frame.width, frame.height);
    }
}

void ascii::DialogScene::AddRelativeFrame(int xOffset, int yOffset, int width, int height)
{
    if (abs(xOffset + yOffset) == 0)
    {
        Log::Error("Tried to add a relative frame without any offset");
    }
    else
    {
        //Log::Print("Adding a relative dialog frame.");
        //stringstream output;
        //output << "X Offset: " << xOffset << " Y Offset: " << yOffset;
        //output << " Width: " << width << " Height: " << height << endl;

        int xStart = msLastFrame.x;
        int yStart = msLastFrame.y;
        int lastWidth = msLastFrame.width;
        int lastHeight = msLastFrame.height;
        //output << "Last frame X: " << xStart << " Last Frame Y: " << yStart;
        //output << " Last frame Width: " << lastWidth << " Last frame Height: " << lastHeight;
        //output << endl;

        if (xOffset > 0) xStart = msLastFrame.right() - 1;
        if (yOffset > 0) yStart = msLastFrame.bottom() - 1;

        if (xOffset < 0) xStart -= width;
        if (yOffset < 0) yStart -= height;
        //output << "Frame start X: " << xStart << " Frame start Y: " << yStart;
        //Log::Print(output.str());

        int x = xStart + xOffset;
        int y = yStart + yOffset;

        //output.str("");
        //output.clear();
        //output << "X: " << x << " Y: " << y << " Width: " << width << " Height: " << height;
        //Log::Print(output.str());

        AddFrame(Rectangle(x, y, width, height));
    }
}

Rectangle ascii::DialogScene::MeasureBubbleFrame(UnicodeString paragraph)
{
    //Log::Print("Measuring a bubble frame for paragraph: " + paragraph);
    //Log::Print(paragraph.length());

    // Create an empty rectangle to store the dimensions as we calculate them
    Rectangle bubbleFrame;

    /* START BY CALCULATING WIDTH AND HEIGHT */

    // If we're stretching to the right or left 
    if (mStretchDirection == STRETCH_LEFT || mStretchDirection == STRETCH_RIGHT)
    {
        // width equals horizontal padding + length of the text line
        int width = mStyle->PaddingX * 2 + paragraph.length();
        bubbleFrame.width = max(width, mStyle->MinBubbleWidth);
        // height equals the third value or minimum height
        bubbleFrame.height = max(mBubbleValue3, mStyle->MinBubbleHeight);
    }
    // If we're stretching down or up
    else
    {
        // Width equals the third value or minimum width
        bubbleFrame.width = max(mBubbleValue3, mStyle->MinBubbleWidth);

        // Height equals vertical padding + length of the text line

        // Start by determining how many lines are needed
        Rectangle model(
                0, 0,
                bubbleFrame.width - mStyle->PaddingX * 2, 
                mpGame->graphics()->height());
        int lines = Surface::measureStringMultilineY(paragraph, model);
        int height = mStyle->PaddingY * 2 + lines;

        bubbleFrame.height = max(height, mStyle->MinBubbleHeight);
    }

    /* NOW POSITION THE BUBBLE */

    // X and Y start as the first and second given values
    bubbleFrame.x = mBubbleValue1;
    bubbleFrame.y = mBubbleValue2;

    // If we're stretching from the top, move the bubble upwards to simulate
    // this effect
    if (mStretchDirection == STRETCH_UP)
    {
        bubbleFrame.y -= (bubbleFrame.height - 1);
    }
    // If we're stretching from the left, move the bubble left to simulate this
    // effect
    if (mStretchDirection == STRETCH_LEFT)
    {
        bubbleFrame.x -= (bubbleFrame.width - 1);
    }

    //Log(bubbleFrame.x);
    //Log(bubbleFrame.y);
    //Log(bubbleFrame.width);
    //Log(bubbleFrame.height);
    return bubbleFrame;
}

void ascii::DialogScene::SetBubbleFrame(UnicodeString paragraph)
{
    // Delete every other frame so the new bubble frame will be the only one
    DeleteFrames();

    // Measure the bubble frame required
    Rectangle bubbleFrame = this->MeasureBubbleFrame(paragraph);

    // Add the bubble frame as the only frame in this scene
    AddFrame(bubbleFrame);
}

void ascii::DialogScene::AddWord(UnicodeString word)
{
    DialogFrame* nextFrame = FrameForWord(word);

    if (nextFrame)
    {
        // If a frame is available, use it
        nextFrame->AddWord(word);
    }
    else
    {
        // Otherwise wait and clear
        mFilled = true;
    }
}

void ascii::DialogScene::AddHeading(UnicodeString heading)
{
    DialogFrame* nextFrame = FrameForHeading();

    if (nextFrame)
    {
        // If a frame is available, use it
        nextFrame->AddHeading(heading);
    }
    else
    {
        // Otherwise wait and clear
        mFilled = true;
    }
}

void ascii::DialogScene::AddSurface(Surface* surface)
{
    DialogFrame* frame = &mFrames[mCurrentFrame];
    frame->AddSurface(surface);
}

UnicodeString ascii::DialogScene::AddParagraphFlush(UnicodeString paragraph)
{
    UnicodeString remainingParagraph = paragraph;
    for (; mCurrentFrame < mFrames.size(); ++mCurrentFrame)
    {
        DialogFrame* frame = &mFrames[mCurrentFrame];
        remainingParagraph = frame->AddParagraphFlush(remainingParagraph);

        if (remainingParagraph.length() == 0)
        {
            break;
        }
    }

    // Now line break before the next paragraph
    LineBreak();

    // Return the remainder so it can be handled after clearing
    return remainingParagraph;
}

void ascii::DialogScene::AddMockParagraph(UnicodeString paragraph, UChar mockLetter)
{
    // TODO  this will only add the mock paragraph until it fills the CURRENT
    // frame, it will not spread the remainder to remaining frames. This
    // clearly is not the expected behavior, but for the SPECIFIC purpose this
    // needs to fill so far, it is fine


    DialogFrame* frame = &mFrames[mCurrentFrame]; 
    frame->AddMockParagraph(paragraph, mockLetter);
}

void ascii::DialogScene::FillMockParagraphs(UChar mockLetter)
{
    for (; mCurrentFrame < mFrames.size(); ++mCurrentFrame)
    {
        DialogFrame* frame = &mFrames[mCurrentFrame]; 
        if (mStyle->IsTextFlush)
        {
            frame->FillMockParagraphsFlush(mockLetter);
        }
        else
        {
            frame->FillMockParagraphs(mockLetter);
        }
    }
}

void ascii::DialogScene::LineBreak()
{
    DialogFrame* nextFrame = FrameForLineBreak();

    if (nextFrame != NULL)
    {
        nextFrame->LineBreak();
    }
    else
    {
        mSaveLineBreak = true;
    }
}

bool ascii::DialogScene::AllWordsRevealed()
{
    for (auto it = mFrames.begin(); it != mFrames.end(); ++it)
    {
        // If any frame hasn't revealed all its words, then nope
        if (!it->AllWordsRevealed()) return false;
    }

    // Otherwise yes
    return true;
}

bool ascii::DialogScene::AllWordsHidden()
{
    for (auto it = mFrames.begin(); it != mFrames.end(); ++it)
    {
        // If the frame hasn't hidden all its words, then nope
        if (!it->AllWordsHidden()) return false;
    }

    // Otherwise yes sirree!
    return true;
}

int ascii::DialogScene::LettersToReveal()
{
    if (mFrames.empty())
    {
        Log::Error("Tried to count letters to reveal in a DialogScene without frames.");
        return 0;
    }

    return mFrames[mCurrentFrame].LettersToReveal();
}

int ascii::DialogScene::RevealedLetters()
{
    int sum = 0;
    for (int i = 0; i < mFrames.size(); ++i)
    {
        sum += mFrames[i].RevealedLetters();
    }
    return sum;
}

void ascii::DialogScene::RevealLetters(int amount)
{
    // Reveal letters from every text frame that has letters to reveal,
    // in case there are multiple revealing words simultaneously
    for (auto it = mFrames.begin(); it != mFrames.end(); ++it)
    {
        it->RevealLetters(amount);
    }
}

void ascii::DialogScene::RevealAllWords()
{
    for (auto it = mFrames.begin(); it != mFrames.end(); ++it)
    {
        it->RevealAllLetters();
    }
}

void ascii::DialogScene::HideLetters(int amount)
{
    // This code has to get pretty weird in order to make hiding multiple
    // centered frames look good. If one centered frame below another is
    // much shorter, we don't want it to start hiding at the same time because
    // that frame will then disappear first, appearing like a bug in the wipe

    // Also note that this code assumes our DialogScene is composed of centered
    // frames, because that is currently the only use-case for hiding letters
    // (during the ritual recording scene)

    // Calculate the greatest width of any dialog frame
    int maxWidth = 0;
    for (int i = 0; i < mFrames.size(); ++i)
    {
        if (mFrames[i].Width() > maxWidth)
        {
            maxWidth = mFrames[i].Width();
        }
    }

    // Hide letters from every text frame that has letters to hide,
    // in case there are multiple hiding words simultaneously
    for (int i = 0; i < mFrames.size(); ++i)
    {
        DialogFrame* pFrame = &(mFrames[i]);
        int cellsBeforeHide = (maxWidth - pFrame->Width()) / 2;
        pFrame->HideLetters(amount, cellsBeforeHide);
    }
}

void ascii::DialogScene::Draw(Graphics& graphics, Preferences* config)
{
    // Draw dynamically sized frames based on the style's frame style,
    // if it has one
    for (auto it = mFrameSurfaces.begin(); it != mFrameSurfaces.end(); ++it)
    {
        Point position = it->first;
        Surface* frameSurface = it->second;

        graphics.blitSurface(frameSurface, position.x, position.y);
    }

    // Simply draw all DialogFrames
    for (auto it = mFrames.begin(); it != mFrames.end(); ++it)
    {
        it->Draw(graphics, config);
    }
}

void ascii::DialogScene::DrawCursor(Graphics& graphics)
{
    DialogFrame* nextFrame = FrameForWord("a");
    if (nextFrame)
    {
        nextFrame->DrawCursor(graphics);
    }
    else
    {
        Log::Error("No frame in which to draw the cursor");
    }
}

ascii::DialogFrame* ascii::DialogScene::FrameForWord(UnicodeString word)
{
    //Log::Print("Searching for a frame for word:");
    //Log::Print(word);
    //Log::Print(mCurrentFrame);
    for (; mCurrentFrame < mFrames.size(); ++mCurrentFrame)
    {
        DialogFrame* nextFrame = &mFrames.at(mCurrentFrame);

        if (nextFrame->CanFitWord(word))
        {
            return nextFrame;
        }
        else
        {
            nextFrame->MarkFilled();
        }
    }

    return NULL;
}

ascii::DialogFrame* ascii::DialogScene::FrameForHeading()
{
    for (; mCurrentFrame < mFrames.size(); ++mCurrentFrame)
    {
        DialogFrame* nextFrame = &mFrames.at(mCurrentFrame);

        if (nextFrame->CanFitHeading())
        {
            return nextFrame;
        }
    }

    return NULL;
}

ascii::DialogFrame* ascii::DialogScene::FrameForLineBreak()
{
    // Iterate through every frame and check if they can fit it
    for (auto it = mFrames.begin(); it != mFrames.end(); ++it)
    {
        // Return the first frame which can
        if (it->CanLineBreak())
            return &(*it);
        else
            it->MarkFilled();
    }

    // If none can fit a line break,
    return NULL;
}

void ascii::DialogScene::ClearAllFrames()
{
    for (auto it = mFrames.begin(); it != mFrames.end(); ++it)
    {
        it->Clear();
    }

    if (mSaveLineBreak)
    {
        mFrames.begin()->HalfLineBreak();
        mSaveLineBreak = false;
    }

    // No frames are filled anymore
    mCurrentFrame = 0;
}

void ascii::DialogScene::ResetAllFrames()
{
    for (auto it = mFrames.begin(); it != mFrames.end(); ++it)
    {
        it->Clear();
    }

    if (mSaveLineBreak)
    {
        mSaveLineBreak = false;
    }

    // No frames are filled anymore
    mCurrentFrame = 0;
}

void ascii::DialogScene::DeleteFrames()
{
    // Get rid of all frames
    mFrames.clear();

    // Also get rid of any surface created for their background
    for (int i = 0; i < mFrameSurfaces.size(); ++i)
    {
        delete mFrameSurfaces[i];
    }
    mFrameSurfaces.clear();

    mCurrentFrame = 0;
}

void ascii::DialogScene::SetBubbleFixedValues(int v1, int v2, int v3)
{
    mBubbleValue1 = v1;
    mBubbleValue2 = v2;
    mBubbleValue3 = v3;
}

void ascii::DialogScene::SetStretchDirection(string direction)
{
    map<string, StretchDirection> stretchDirections;
    stretchDirections["Down"] = STRETCH_DOWN;
    stretchDirections["Up"] = STRETCH_UP;
    stretchDirections["Right"] = STRETCH_RIGHT;
    stretchDirections["Left"] = STRETCH_LEFT;

    mStretchDirection = stretchDirections[direction];
}

bool ascii::DialogScene::HasWords()
{
    for (auto it = mFrames.begin(); it != mFrames.end(); ++it)
    {
        if (it->HasWords()) return true;
    }

    return false;
}

void ascii::DialogScene::MarkPosition()
{
    DialogFrame* frame = &mFrames[mCurrentFrame];
    frame->MarkPosition();
}

void ascii::DialogScene::RewindPosition()
{
    DialogFrame* frame = &mFrames[mCurrentFrame];
    frame->RewindPosition();
}
