#include "PixelFont.h"

#include <vector>

#include "SDL_image.h"

#include "FileReader.h"
#include "Log.h"
#include "Rectangle.h"


PixelFont::PixelFont(int charWidth, int charHeight,
        string fontLayoutFile, string textureSheet)
    : mCharWidth(charWidth), mCharHeight(charHeight),
    mInitialized(false)
{
    mFontLayoutPath = fontLayoutFile;
    mFontPath = textureSheet;
}

void PixelFont::Initialize(SDL_Renderer* pRenderer)
{
    Log::Print("Initializing pixel font: " + mFontPath);
    mpRenderer = pRenderer;
    
    // Extract the rows of characters from the layout file
    vector<UnicodeString> layoutRows;
    FileReader layoutFile(mFontLayoutPath);

    while (layoutFile.HasNextLine())
    {
        layoutRows.push_back(layoutFile.NextLineUnicode());
    }

    // Load the texture sheet
    SDL_Surface* tempSurface = IMG_Load(mFontPath.c_str());

    // Make sure the texture sheet loaded correctly
    if (!tempSurface)
    {
        Log::Error("Failed to load pixel font: " + mFontPath);
        Log::SDLError();
        return;
    }

	//Make sure the texture sheet aligns with the layout grid
    int rows = layoutRows.size();
    int cols = layoutRows[0].length();

    int expectedWidth = cols * mCharWidth;
    int expectedHeight = rows * mCharHeight;

    if (tempSurface->w != expectedWidth || tempSurface->h != expectedHeight)
    {
        Log::Error("Tried to load pixel font with invalid texture sheet dimensions to match font layout file: " + mFontPath);

        Log::Print("Layout file: " + mFontLayoutPath);
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

    // Font files are white letters on black background, making black
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
            sourceRect.x = c * mCharWidth;
            sourceRect.y = r * mCharHeight;
            sourceRect.w = mCharWidth;
            sourceRect.h = mCharHeight;

            mCharacterRectangles[character] = sourceRect;
        }
    }

    // Now the pixel font should be ready to render letters!
    mInitialized = true;
}

PixelFont::~PixelFont()
{
    Dispose();
}

void PixelFont::Dispose()
{
    if (mInitialized)
    {
        Log::Print("Disposing pixel font: " + mFontPath);
        SDL_DestroyTexture(mpTextureSheet);
    }

    mInitialized = false;
}

void PixelFont::RenderCharacter(UChar character, int x, int y, Color color)
{
    if (!mInitialized)
    {
        Log::Error("Tried to render characters with uninitialized font: " + mFontPath);
        return;
    }

    // Retrieve the character's source rectangle
    SDL_Rect src = mCharacterRectangles[character];
    SDL_Rect dest = Rectangle(x, y, mCharWidth, mCharHeight);

    SDL_SetTextureColorMod(mpTextureSheet,
            color.r,
            color.g,
            color.b);

    SDL_RenderCopy(mpRenderer, mpTextureSheet, &src, &dest);
}

