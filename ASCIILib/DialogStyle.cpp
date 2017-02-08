#include "DialogStyle.h"

#include <sstream>

#include "Rectangle.h"

#include "json.h"
#include "FilePaths.h"
#include "content.h"
#include "parsing.h"


// Static redeclarations
const unsigned int ascii::DialogStyle::FRAME_SURFACES_DIM;
const unsigned int ascii::DialogStyle::FRAME_SURFACES_FIRST;
const unsigned int ascii::DialogStyle::FRAME_SURFACES_CENTER;
const unsigned int ascii::DialogStyle::FRAME_SURFACES_LAST;

ascii::DialogStyle::~DialogStyle()
{
    if (IsFramed)
    {
        for (int i = 0; i < FRAME_SURFACES_DIM; ++i)
        {
            for (int j = 0; j < FRAME_SURFACES_DIM; ++j)
            {
                delete mFrameSurfaces[i][j];
            }
        }
    }
}

ascii::DialogStyle* ascii::DialogStyle::FromFile(string path)
{
    /* Load a DialogStyle from the given file path. DialogStyle files have the
     * following JSON format:
     *
     * {
     *     "frame-surface": "frame-surface-handle",         // default: none
     *     "bubble": [ bool ],                              // default: false,
     *                                                         ignored without frame 
     *                                                         surface
     *     "text-color": [ r, g, b ],                       // required
     *     "text-padding": [ x, y ],                        // default: none
     *     "clear-sound": "clear-sound-name",               // default: none
     *     "revealing-sound-groups": {
     *         "default": "default-revealing-sound-group-name", // required
     *         "Normal": "Normal-speed-revealing-sound-group",
     *         "Fast": "Fast-speed-revealing-sound-group"
     *     }
     * }
     *     
     * The frame surface defines what the message dialog's frame will look
     * like. If no surface is provided, the message dialog has no frame. RGB
     * values are from 0 to 255, and text padding is given in cells. */

    // Make an empty style to store all the info we are about to parse
    DialogStyle* style = new DialogStyle();

    style->Filename = path;

    // Parse the JSON file
    Json::Value* jsonPtr = Json::Load(path);
    Json::Value json = *jsonPtr;


    // Parse the text color (the only required attribute)
    style->TextColor = ParseColor(json["text-color"]);

    // Parse the padding info as a point (if included in the JSON)
    Point padding = Point(0, 0);
    if (ElementExists(json, "text-padding"))
        padding = ParsePoint(json["text-padding"], map<string, int>());
    style->PaddingX = padding.x;
    style->PaddingY = padding.y;

    // Parse the name of the frame clearing sound (if included in the JSON)
    if (ElementExists(json, "clear-sound"))
    {
        style->ClearSound = GetString(json, "clear-sound");
    }

    // Parse the text-revealing cursor if this style defines one
    style->HasCursor = false;
    if (ElementExists(json, "revealing-cursor"))
    {
        Json::Value dictionaryJson = json["revealing-cursor"];

        style->HasCursor = true;
        style->CursorColor = ParseColor(dictionaryJson["color"]);
        style->CursorBlinkMS = GetInt(dictionaryJson, "blink-interval");
    }

    // Parse the dictionary of text revealing sound groups (if included)
    if (ElementExists(json, "revealing-sound-groups"))
    {
        Json::Value dictionaryJson = json["revealing-sound-groups"];

        Json::Value::Members keys = dictionaryJson.getMemberNames();

        for (auto it = keys.begin(); it != keys.end(); ++it)
        {
            style->RevealingSoundGroups[*it] = GetString(dictionaryJson, *it);
        }
    }

    // The DialogStyle can configure a modifier for the sound volume of the
    // revealing group
    style->RevealingSoundGroupVolume = 1.0f;

    if (ElementExists(json, "revealing-group-volume"))
    {
        style->RevealingSoundGroupVolume = GetFloat(json, "revealing-group-volume");
    }

    // Parse whether this style uses flush text (defaulting to non-flush)
    style->IsTextFlush = false;
    if (ElementExists(json, "flush-text"))
    {
        style->IsTextFlush = GetBool(json, "flush-text");
    }

    // Load the frame surface if a handle is defined, and break it into the
    // parts we need
    style->IsFramed = false;
    style->MinBubbleWidth = 0;
    style->MinBubbleHeight = 0;
    if (ElementExists(json, "frame-surface"))
    {
        // This style has a frame
        style->IsFramed = true;

        // Retrieve the handle of the frame template surface file
        Handle frameHandle = GetString(json, "frame-surface");

        // By default, frames have minimum dimensions large enough to
        // accomodate padding + one cell
        style->MinBubbleWidth = padding.x * 2 + 1;
        style->MinBubbleHeight = padding.y * 2 + 1;
        if (ElementExists(json, "min-bubble-width"))
        {
            style->MinBubbleWidth =
                max(style->MinBubbleWidth, GetInt(json, "min-bubble-width"));
        }
        if (ElementExists(json, "min-bubble-height"))
        {
            style->MinBubbleHeight =
                max(style->MinBubbleHeight, GetInt(json, "min-bubble-height"));
        }

        // Load the frame surface
        string framePath = "content/surfaces/" + frameHandle;
        Surface* frameSurface = Surface::FromFile(FileAccessPath(framePath).c_str());

        /* The frame surface must have odd-numbered dimensions,
         * because the edges and center of the frame are defined in the surface
         * file as a vertically and horizontally symmetrical cross with constant
         * thickness of 1 cell, and the corners of the frame share the same
         * dimensions up and down, and side to side.
         *
         * (2 x any number) + odd number = odd number
         *
         * To illustrate:
         *
         * aabcc    aaaabcccc    abc
         * aabcc    ddddeffff    abc
         * ddeff    gggghiiii    def
         * gghii                 ghi
         * gghii                 ghi
         *
         * In the frame surfaces represented above, the
         * letters denote the following:
         *
         * a - top left corner of the frame (will not be scaled)
         * b - top edge of the frame (will be stretched horizontally to size)
         * c - top right corner of the frame (will not be scaled)
         * d - left edge of the frame (will be stretched vertically to size)
         * e - inside of the frame (will be stretched vertically and
         * horizontally to size)
         * f - right edge of the frame (will be stretched vertically to size)
         * g - bottom left corner of the frame (will not be scaled)
         * h - bottom edge of the frame (will be stretched horizontally to
         * size)
         * i - bottom right corner of the frame (will not be scaled)
         * */

        if (frameSurface->width() % 2 == 0
                || frameSurface->height() % 2 == 0)
        {
            // TODO handle this error! An invalidly sized frame surface was loaded
            return NULL;
        }

        // Calculate the width and height of the frame corners
        const unsigned int cornerWidth = (frameSurface->width() - 1) / 2;
        const unsigned int cornerHeight = (frameSurface->height() - 1) / 2;

        // Cut the frame surface into 9 pieces with a loop
        for (int i = 0; i < FRAME_SURFACES_DIM; ++i)
        {
            for (int j = 0; j < FRAME_SURFACES_DIM; ++j)
            {
                // This is one of the 9 pieces. We need its location and size on
                // the frame surface
                unsigned int frameSurfaceX, frameSurfaceY;
                unsigned int partWidth, partHeight;

                // Calculate its width and x position on the frame surface
                AssignPartInfo(&i, cornerWidth, &partWidth, &frameSurfaceX);
                // Calculate its height and y position on the frame surface
                AssignPartInfo(&j, cornerHeight, &partHeight, &frameSurfaceY);

                // Create an empty surface of the necessary size
                style->mFrameSurfaces[i][j] = new Surface(partWidth, partHeight);
                // Copy onto it the part of the frame surface we want for this
                // piece
                style->mFrameSurfaces[i][j]->copySurface(frameSurface,
                        Rectangle(frameSurfaceX, frameSurfaceY,
                            partWidth, partHeight), 0, 0);
            }
        }

        // Clean up the original now that we've extracted our resizable pieces.
        delete frameSurface;
    }

    // Styles default to use line breaks
    style->LineBreaks = true;
    if (ElementExists(json, "line-breaks"))
    {
        style->LineBreaks = GetBool(json, "line-breaks");
    }

    // Styles default to not use simultaneous words
    style->SimultaneousWords = false;
    if (ElementExists(json, "simultaneous-words"))
    {
        style->SimultaneousWords = GetBool(json, "simultaneous-words");
    }

    // Clean up the JSON we parsed
    delete jsonPtr;

    // Return the result
    return style;
}

void ascii::DialogStyle::AssignPartInfo(int* partCounter,
        unsigned int cornerDimension, unsigned int* partDimension,
        unsigned int* partSurfaceCoord)
{
    switch (*partCounter)
    {
        // It's the first corner in whichever dimension we're checking
        case FRAME_SURFACES_FIRST:
            *partDimension = cornerDimension;
            // Which means it's located at the axis of whichever dimension we're
            // checking
            *partSurfaceCoord = 0;

            break;

        // It's the center in whichever dimension we're checking
        case FRAME_SURFACES_CENTER:
            *partDimension = 1;
            // Which means it's located a corner length away from the axis 
            *partSurfaceCoord = cornerDimension;
            break;

        // It's the last corner in whichever dimension we're checking
        case FRAME_SURFACES_LAST:
            *partDimension = cornerDimension;
            // Which mean's it's located a single unit further than the center
            *partSurfaceCoord = cornerDimension + 1;
            break;
    }
}

// Sorry about this function. I really can't find a better way to do it.
ascii::Surface* ascii::DialogStyle::MakeFrame(int width, int height)
{
    Surface* frameSurface = new Surface(width, height);

    Surface* topLeftCorner = mFrameSurfaces
        [FRAME_SURFACES_FIRST][FRAME_SURFACES_FIRST];
    Surface* leftBorder = mFrameSurfaces
        [FRAME_SURFACES_FIRST][FRAME_SURFACES_CENTER];
    Surface* bottomLeftCorner = mFrameSurfaces
        [FRAME_SURFACES_FIRST][FRAME_SURFACES_LAST];
    Surface* topBorder = mFrameSurfaces
        [FRAME_SURFACES_CENTER][FRAME_SURFACES_FIRST];
    Surface* frameFill = mFrameSurfaces
        [FRAME_SURFACES_CENTER][FRAME_SURFACES_CENTER];
    Surface* bottomBorder = mFrameSurfaces
        [FRAME_SURFACES_CENTER][FRAME_SURFACES_LAST];
    Surface* topRightCorner = mFrameSurfaces
        [FRAME_SURFACES_LAST][FRAME_SURFACES_FIRST];
    Surface* rightBorder = mFrameSurfaces
        [FRAME_SURFACES_LAST][FRAME_SURFACES_CENTER];
    Surface* bottomRightCorner = mFrameSurfaces
        [FRAME_SURFACES_LAST][FRAME_SURFACES_LAST];

    // Every corner has the same dimensions and we will reuse these numbers
    const int cornerWidth = topLeftCorner->width();
    const int cornerHeight = topLeftCorner->height();

    // Assemble a frame from this style by stretching out its 9 parts to the
    // specified size.

    // NOTE: We copy, not blit, the frame surfaces because they may contain
    // transparency which we wish to preserve in the frame we return

    // Copy the top-left corner in the top-left corner of the new
    // surface
    frameSurface->copySurface(topLeftCorner, 0, 0);

    // Copy the left border repeatedly to form the left border of the
    // frame
    for (int i = 0; i < height - cornerHeight * 2; ++i)
    {
        const int sy = cornerHeight + i;

        frameSurface->copySurface(leftBorder, 0, sy);
    }

    // Copy the bottom-left corner in the bottom-left corner of the new
    // surface
    frameSurface->copySurface(bottomLeftCorner, 0, height - cornerHeight);

    // Copy the top border repeatedly to form the top border of the
    // frame
    for (int i = 0; i < width - cornerWidth * 2; ++i)
    {
        const int x = cornerWidth + i;

        frameSurface->copySurface(topBorder, x, 0);
    }

    // Copy the frame fill repeatedly to form the inside of the frame
    for (int i = 0; i < width - cornerWidth * 2; ++i)
    {
        for (int j = 0; j < height - cornerHeight * 2; ++j)
        {
            const int x = cornerWidth + i;
            const int y = cornerHeight + j;

            frameSurface->copySurface(frameFill, x, y);
        }
    }

    // Copy the bottom border repeatedly to form the bottom border of
    // the frame
    for (int i = 0; i < width - cornerWidth * 2; ++i)
    {
        const int x = cornerWidth + i;

        frameSurface->copySurface(bottomBorder, x,
                height - cornerHeight);
        // cornerHeight is the same as horizontal border height
    }

    // Copy the top-right corner in the top-right corner of the new
    // surface
    frameSurface->copySurface(topRightCorner, width - cornerWidth, 0);

    // Copy the right border repeatedly to form the right border of the new
    // surface
    for (int i = 0; i < height - cornerHeight * 2; ++i)
    {
        const int y = cornerHeight + i;

        frameSurface->copySurface(rightBorder, width - cornerWidth, y);
        // cornerWidth is the same as horizontal border width
    }

    // Copy the bottom-right border in the bottom-right corner of the new
    // surface
    frameSurface->copySurface(bottomRightCorner, width - cornerWidth,
            height - cornerHeight);

    // Return our Frankenstein construction
    return frameSurface;
}
