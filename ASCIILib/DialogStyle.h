#pragma once

#include <string>
#include <map>
using namespace std;

#include "Surface.h"
#include "Color.h"
using namespace ascii;


// Represents a style of MessageDialog, including information such as text
// color, border and background style, 
struct DialogStyle
{
    public:
        // Free all of the style's ASCII surfaces
        ~DialogStyle();

        // Loads a DialogStyle from the given file.
        static DialogStyle* FromFile(string path);

        // Generates an ASCII surface from this style's frame style, of the
        // given dimensions
        Surface* MakeFrame(int width, int height);

        // Color of the text inside the dialog
        Color TextColor;

        // Padding against the edges of the dialog frame (in cells).
        unsigned int PaddingX;
        unsigned int PaddingY;

        // Whether this style utilizes a dynamically sized frame.
        bool IsFramed;

        // Whether this style supports line breaks
        bool LineBreaks;

        // Minimum dimensions of a dynamically sized frame from this style
        int MinBubbleWidth;
        int MinBubbleHeight;

        // Whether the text in this style must be flush with both edges
        bool IsTextFlush;

        // Whether the words of a paragraph all appear at the same time (but
        // not instantly)
        bool SimultaneousWords;

        // Key for the sound group that loops while a MessageDialog is
        // revealing text in this style
        map<string, string> RevealingSoundGroups;

        // Volume modifier for the sound group that is looped while revealing
        // text
        float RevealingSoundGroupVolume;

        // Key for the sound effect that plays when a MessageDialog using this
        // style is cleared
        string ClearSound;

        bool HasCursor;
        Color CursorColor;
        int CursorBlinkMS;

        string Filename;

    private:
        // Square dimensions of the multidimensional array storing this style's
        // frame's parts. Center + 4 edges + 4 corners = 9 parts
        static const unsigned int FRAME_SURFACES_DIM = 3;
        // Codes for the three sections a frame surface
        static const unsigned int FRAME_SURFACES_FIRST = 0;
        static const unsigned int FRAME_SURFACES_CENTER = 1;
        static const unsigned int FRAME_SURFACES_LAST = 2;

        // Helper method to avoid repeating the same code for width and height 
        static void AssignPartInfo(int* partCounter,
                unsigned int cornerDimension,
                unsigned int* partDimension, unsigned int* partSurfaceCoord);

        // Surfaces representing the dialog's dynamically sized frame.
        // [0][0] represents top-left, [2][0] represents top-right, [2][2]
        // represents bottom-right
        Surface* mFrameSurfaces[3][3];
};
