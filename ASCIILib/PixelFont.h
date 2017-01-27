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
            PixelFont(int charWidth, int charHeight,
                    string fontLayoutFile, string textureSheet);
            
            void Initialize(SDL_Renderer* pRenderer);
            void Dispose();


            ~PixelFont();

            // Draw the given character at the point given in pixels using the
            // given renderer
            void RenderCharacter(UChar character, int x, int y, Color color);

            int charHeight() { return mCharHeight; }

        private:
            SDL_Renderer* mpRenderer;
            SDL_Texture* mpTextureSheet;
            map<UChar, SDL_Rect> mCharacterRectangles;

            int mCharWidth;
            int mCharHeight;

            string mFontLayoutPath;
            string mFontPath;

            bool mInitialized;
    };

}

