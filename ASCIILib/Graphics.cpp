#include "Graphics.h"

#include <algorithm>
#include <string>

#include "unicode/uchar.h"
#include "unicode/locid.h"
#include "unicode/ustream.h"
#include "unicode/ustdio.h"

#include <SDL_image.h>

#include "Log.h"
#include "StringTokenizer.h"
#include "FileReader.h"
using namespace ascii;

const int kFontSize = 12;

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;

//static
const unsigned int ascii::Graphics::kBufferHeight = 25;

// Note: The font used by Graphics might for some reason yield a pixel height
// different than kFontSize. In this case, mCharHeight will be invalid for
// calculation of window size and other values. kFontSize is the actual
// measurement of the height of a cell, and mCharHeight is the height of the
// character the font created


ascii::Graphics::Graphics(const char* title, string fontpath,
        int bufferWidth, int bufferHeight)
	: Surface(bufferWidth, bufferHeight), mTitle(title),
    mFullscreen(false), mBackgroundColor(ascii::Color::Black),
    mWindow(NULL), mRenderer(NULL), mHidingImages(false),
    mCharHeightCorrection(0)
{
	if(TTF_Init() == -1)
    {
        Log::Error("SDL_ttf failed to initialize");
        Log::SDLError();
    }

	mFont = TTF_OpenFont(fontpath.c_str(), kFontSize);
    if (!mFont)
    {
        Log::Error("Failed to open the font file: " + fontpath);
        Log::SDLError();
    }

    Initialize();
}

ascii::Graphics::~Graphics(void)
{
    Dispose();

	TTF_CloseFont(mFont);

	TTF_Quit();
}

void ascii::Graphics::Initialize()
{
    mFullscreen = false;

    UpdateCharSize();

    int flags = SDL_WINDOW_SHOWN;

	mWindow = SDL_CreateWindow(mTitle, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		width() * mCharWidth, height() * kFontSize, 
		flags);

    if (!mWindow)
    {
        Log::Error("Failed to create a window for the game.");
        Log::SDLError();
    }

	checkSize();

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

    if (!mRenderer)
    {
        Log::Error("Failed to create SDL_Renderer.");
        Log::SDLError();
    }

	mCache = new ascii::ImageCache(mRenderer,
            mCharWidth,
            kFontSize);
}

void ascii::Graphics::Dispose()
{
    // Free textures we've created for rendering tokens
    clearGlyphs();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    delete mCache;
}

void ascii::Graphics::SetFullscreen(bool fullscreen)
{
    // Don't go into any of these operations if unnecessary
    if (fullscreen == mFullscreen) return;

    mFullscreen = fullscreen;

    Uint32 flags = 0; 
    if (fullscreen)
    {
        flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    else
    {
        Dispose();
        Initialize();
    }

    if (SDL_SetWindowFullscreen(mWindow, flags) != 0)
    {
        Log::SDLError();
    }
}

void ascii::Graphics::ToggleFullscreen()
{
    SetFullscreen(!mFullscreen);
}

int ascii::Graphics::pixelToCellX(int pixelX)
{
    if (drawOrigin().x > 0)
    {
        pixelX -= drawOrigin().x;
    }
    return pixelX / mCharWidth;
}

int ascii::Graphics::pixelToCellY(int pixelY)
{
    if (drawOrigin().y > 0)
    {
        pixelY -= drawOrigin().y;
    }
    return pixelY / kFontSize;
}

ascii::Point ascii::Graphics::drawOrigin()
{
    // Calculate the top-left corner of the rendering region
    int drawX = 0;
    int drawY = 0;

    Point actualRes = actualResolution();
    Point drawRes = drawResolution();

    if (actualRes.x > drawRes.x || actualRes.y > drawRes.y)
    {
        // Dealing with a fullscreen window
        drawX = actualRes.x / 2 - drawRes.x / 2;
        drawY = actualRes.y / 2 - drawRes.y / 2;
    }

    return Point(drawX, drawY);
}

void ascii::Graphics::clearScreen()
{
	// Draw background color
	SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, ascii::Color::kAlpha);
	SDL_RenderFillRect(mRenderer, NULL);
}

void ascii::Graphics::drawImages(std::map<std::string, Image>* images)
{
    // Don't draw any images if they're currently being hidden
    if (!mHidingImages)
    {
        int drawX = drawOrigin().x;
        int drawY = drawOrigin().y;

        // Draw every image in the given map otherwise, using their specified
        // positions
        for (auto it = images->begin(); it != images->end(); ++it)
        {
            SDL_Rect dest;
            
            dest.x = drawX + it->second.second.x * mCharWidth;
            dest.y = drawY + it->second.second.y * kFontSize;

            SDL_QueryTexture(it->second.first, NULL, NULL, &dest.w, &dest.h);

            SDL_RenderCopy(mRenderer, it->second.first, NULL, &dest);
        }
    }
}

void ascii::Graphics::drawBackgroundColors(ascii::Surface* surface, int x, int y)
{
    int drawX = drawOrigin().x;
    int drawY = drawOrigin().y;

	for (int ySrc = 0; ySrc < surface->height(); ++ySrc) 
    {
		int xSrc = 0;

		while (xSrc < surface->width())
		{
			//chain all adjacent background colors in a row for more efficient rendering
			SDL_Rect colorRect;

			colorRect.x = drawX + (x + xSrc) * mCharWidth;
			colorRect.y = drawY + (y + ySrc) * kFontSize;
			colorRect.w = 0;
			colorRect.h = kFontSize;

			Color backgroundColor = surface->getBackgroundColor(xSrc, ySrc);

			do
			{
				if (!surface->isCellOpaque(xSrc, ySrc))
                {
                    ++xSrc;
                    break;
                }

				colorRect.w += mCharWidth;
				++xSrc;
			} while (xSrc < surface->width() && surface->getBackgroundColor(xSrc, ySrc) == backgroundColor);

			SDL_SetRenderDrawColor(mRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, Color::kAlpha);
			SDL_RenderFillRect(mRenderer, &colorRect);
		}
	}
}

void ascii::Graphics::drawCharacters(ascii::Surface* surface, int x, int y)
{
    int drawX = drawOrigin().x;
    int drawY = drawOrigin().y;

	//draw all characters
	for (int ySrc = 0; ySrc < surface->height(); ++ySrc)
	{
		int xSrc = 0;

		while (xSrc < surface->width())
		{
			// Chain all adjacent characters with the same color into strings
            // for more efficient rendering

            // Don't bother chaining spaces together
			UChar uch = surface->getCharacter(xSrc, ySrc);

			if (IsWhiteSpace(uch))
			{
				++xSrc;
				continue;
			}

            // If the character is not a space, start chaining with its
            // neighbors
            UnicodeString charChain;
			SDL_Rect textRect;

			textRect.x = drawX + (x + xSrc) * mCharWidth;
			textRect.y = drawY + (y + ySrc) * kFontSize;
			textRect.w = 0;
			textRect.h = kFontSize;
			Color characterColor = surface->getCharacterColor(xSrc, ySrc);

			do
			{
				if (!surface->isCellOpaque(xSrc, ySrc))
				{
					++xSrc;
					break;
				}

                uch = surface->getCharacter(xSrc, ySrc);

                // Don't chain empty space in a word. Empty space can exist
                // here if it is used as the base for a non-space character
                // combo
                if (!IsWhiteSpace(uch))
                {
                    charChain += uch;
                    textRect.w += mCharWidth;
                }
                else
                {
                    ++xSrc;
                    break;
                }

				++xSrc;
			} while (
                // Stop when we reach the end of the row
                xSrc < surface->width()
                // Stop if the next character has a different color
                && surface->getCharacterColor(xSrc, ySrc) == characterColor
                // Stop if the next character is another space
                && !IsWhiteSpace(surface->getCharacter(xSrc, ySrc)));

            // Convert the unicode into an appropriate string encoding for
            // TTF_RenderText()
            string temp;
            string str = charChain.toUTF8String(temp);

			Glyph glyph = std::make_pair(str, characterColor);

			SDL_Texture* texture = NULL;

			if (mGlyphTextures[glyph])
			{
				texture = mGlyphTextures[glyph];
			}
			else
			{
				SDL_Surface* surface = TTF_RenderUTF8_Solid(mFont, str.c_str(), characterColor);
				texture = SDL_CreateTextureFromSurface(mRenderer, surface);

				mGlyphTextures[glyph] = texture;
				
				SDL_FreeSurface(surface);
			}

            // If for some reason the font dimensions are off, clip the glyph
            // so the texture fits where it should
            SDL_Rect adjustedGlyphSrc;
            adjustedGlyphSrc.x = 0;
            adjustedGlyphSrc.y = 0;
            adjustedGlyphSrc.w = textRect.w;
            adjustedGlyphSrc.h = textRect.h;
            SDL_RenderCopy(mRenderer, texture, &adjustedGlyphSrc, &textRect);
		}
	}
}

void ascii::Graphics::drawSurface(ascii::Surface* surface, int x, int y)
{
	// Draw all background colors
    drawBackgroundColors(surface, x, y);

    // Draw all characters
    drawCharacters(surface, x, y);
}

void ascii::Graphics::refresh()
{
    SDL_RenderPresent(mRenderer);
}

void ascii::Graphics::update()
{
    // Clear the screen for drawing
    clearScreen();

	// Draw background images
    drawImages(&mBackgroundImages);

    // Draw the buffer surface in between
    drawSurface(this, 0, 0);

	// Draw foreground images
    drawImages(&mForegroundImages);

    // Draw foreground surfaces
    for (int i = 0; i < mForegroundSurfaces.size(); ++i)
    {
        Surface* surface = mForegroundSurfaces[i].first;
        Point position = mForegroundSurfaces[i].second;

        drawSurface(surface, position.x, position.y);
    }

    // Clear any surfaces from the foreground
    mForegroundSurfaces.clear();

    // Refresh the window to show all changes
    refresh();
}

void ascii::Graphics::drawForegroundSurface(ascii::Surface* surface, int x, int y)
{
    mForegroundSurfaces.push_back(make_pair(surface, Point(x, y)));
}

void ascii::Graphics::addBackgroundImage(std::string key, std::string textureKey, int x, int y)
{
	mBackgroundImages[key] = std::make_pair(mCache->getTexture(textureKey), ascii::Point(x, y));
}

void ascii::Graphics::removeBackgroundImage(std::string key)
{
	mBackgroundImages.erase(key);
}

void ascii::Graphics::addForegroundImage(std::string key, std::string textureKey, int x, int y)
{
	mForegroundImages[key] = std::make_pair(mCache->getTexture(textureKey), ascii::Point(x, y));
}

void ascii::Graphics::removeForegroundImage(std::string key)
{
	mForegroundImages.erase(key);
}

void ascii::Graphics::clearImages()
{
	mBackgroundImages.clear();
	mForegroundImages.clear();
}

void ascii::Graphics::hideImages()
{
    mHidingImages = true;
}

void ascii::Graphics::showImages()
{
    mHidingImages = false;
}

void ascii::Graphics::clearGlyphs()
{
	for (std::map<Glyph, SDL_Texture*>::iterator it = mGlyphTextures.begin(); it != mGlyphTextures.end(); ++it)
	{
		SDL_DestroyTexture(it->second); //destroy all stored string textures
	}

	mGlyphTextures.clear();
}

ascii::Point ascii::Graphics::drawResolution()
{
    return ascii::Point(mCharWidth * width(), kFontSize * height());
}

ascii::Point ascii::Graphics::actualResolution()
{
    int w, h;
    SDL_GetWindowSize(mWindow, &w, &h);
    return ascii::Point(w, h);
}

void ascii::Graphics::checkSize()
{
    if (!mFullscreen)
    {
        int w, h;
        SDL_GetWindowSize(mWindow, &w, &h);

        bool check = width() * mCharWidth == w && height() * kFontSize == h;

        if (!check)
        {
            Log::Error("The size of the created window does not match expected dimensions! This could be because the width and height of the graphics buffer are too small.");
        }
    }
}

void ascii::Graphics::UpdateCharSize()
{
	TTF_SizeText(mFont, "A", &mCharWidth, &mCharHeight);

    if (mCharHeight != kFontSize)
    {
        Log::Print("Warning! Char height measured from the font loaded does not match expected font size:", false);
        Log::Print(mCharHeight, false);
        Log::Print(" != ", false);
        Log::Print(kFontSize);

        mCharHeightCorrection = abs(mCharHeight - kFontSize);
    }

    Log::Print("Char dimensions: (", false);
    Log::Print(mCharWidth, false);
    Log::Print(" ", false);
    Log::Print(mCharHeight, false);
    Log::Print(")");
}
