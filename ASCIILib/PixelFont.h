#pragma once

#include <string>
#include <map>
using namespace std;

#include "unicode/utypes.h"

#include "Color.h"
using namespace ascii;

#include "SDL.h"

namespace ascii
{

    // Represents a font defined inside a uniformly grid-based texture sheet.
    class PixelFont
    {
        public:
            // Create and load a pixel font
            PixelFont(SDL_Renderer* pRenderer, int charWidth, int charHeight,
                    string fontLayoutFile, string textureSheet);
            ~PixelFont();

            // Draw the given character at the point given in pixels using the
            // given renderer
            void RenderCharacter(UChar character, int x, int y, Color color);
        private:
            SDL_Renderer* mpRenderer;
            SDL_Texture* mpTextureSheet;
            map<UChar, SDL_Rect> mCharacterRectangles;

            int mCharWidth;
            int mCharHeight;
    };

}

