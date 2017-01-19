#include "PixelFont.h"

#include <vector>

#include "SDL_image.h"

#include "FileReader.h"
#include "Log.h"
#include "Rectangle.h"


PixelFont::PixelFont(SDL_Renderer* pRenderer, int charWidth, int charHeight,
        string fontLayoutFile, string textureSheet)
    : mpRenderer(pRenderer), mCharWidth(charWidth), mCharHeight(charHeight)
{
    // Extract the rows of characters from the layout file
    vector<UnicodeString> layoutRows;
    FileReader layoutFile(fontLayoutFile);

    while (layoutFile.HasNextLine())
    {
        layoutRows.push_back(layoutFile.NextLineUnicode());
    }

    // Load the texture sheet
    SDL_Surface* tempSurface = IMG_Load(textureSheet.c_str());

    // Make sure the texture sheet loaded correctly
    if (!tempSurface)
    {
        Log::Error("Failed to load pixel font: " + textureSheet);
        Log::SDLError();
        return;
    }

	//Make sure the texture sheet aligns with the layout grid
    int rows = layoutRows.size();
    int cols = layoutRows[0].length();

    int expectedWidth = cols * charWidth;
    int expectedHeight = rows * charHeight;

    if (tempSurface->w != expectedWidth || tempSurface->h != expectedHeight)
    {
        Log::Error("Tried to load pixel font with invalid texture sheet dimensions to match font layout file: " + textureSheet);

        Log::Print("Layout file: " + fontLayoutFile);
        Log::Print("Expected width: ", false);
        Log::Print(expectedWidth);
        Log::Print("Expected height: ", false);
        Log::Print(expectedHeight);
        Log::Print("Actual width: ", false);
        Log::Print(tempSurface->w);
        Log::Print("Actual height: ", false);
        Log::Print(tempSurface->h);
        return;
    }

    // Font files are white letters on black background, making white
    // transparent
    SDL_SetColorKey(tempSurface, SDL_ENABLE,
            Color::Black.ToUint32(tempSurface->format));

    // Now create a texture from the loaded surface, and free the surface
    mpTextureSheet = SDL_CreateTextureFromSurface(pRenderer, tempSurface);

    // Dispose of the surface
    SDL_FreeSurface(tempSurface);

    // Now go through all of the rows, constructing source rectangles for each
    // character
    for (int r = 0; r < rows; ++r)
    {
        UnicodeString row = layoutRows[r];

        for (int c = 0; c < cols; ++c)
        {
            UChar character = row[c];

            SDL_Rect sourceRect; 
            sourceRect.x = c * charWidth;
            sourceRect.y = r * charHeight;
            sourceRect.w = charWidth;
            sourceRect.h = charHeight;

            mCharacterRectangles[character] = sourceRect;
        }
    }

    // Now the pixel font should be ready to render letters!
}

PixelFont::~PixelFont()
{
    SDL_DestroyTexture(mpTextureSheet);
}

void PixelFont::RenderCharacter(UChar character, int x, int y, Color color)
{
    // Retrieve the character's source rectangle
    SDL_Rect src = mCharacterRectangles[character];
    SDL_Rect dest = Rectangle(x, y, mCharWidth, mCharHeight);

    SDL_SetTextureColorMod(mpTextureSheet,
            color.r,
            color.g,
            color.b);

    SDL_RenderCopy(mpRenderer, mpTextureSheet, &src, &dest);
}

