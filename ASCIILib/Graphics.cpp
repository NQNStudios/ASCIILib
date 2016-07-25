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

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;

//static
const unsigned int ascii::Graphics::kBufferHeight = 25;

ascii::Graphics::Graphics(const char* title, int charWidth, int charHeight,
        string fontpath, int bufferWidth, int bufferHeight)
	: Surface(bufferWidth, bufferHeight), mTitle(title),
    mFullscreen(false), mBackgroundColor(ascii::Color::Black),
    mWindow(NULL), mRenderer(NULL), mHidingImages(false), mFontPath(fontpath),
    mCharWidth(charWidth), mCharHeight(charHeight)
{
    Initialize();
}

ascii::Graphics::~Graphics(void)
{
    Dispose();
}

void ascii::Graphics::Initialize()
{
    mFullscreen = false;

    int flags = SDL_WINDOW_SHOWN;

	mWindow = SDL_CreateWindow(mTitle, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		width() * mCharWidth, height() * mCharHeight, 
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
        Log::Error("Failed to SDL_Renderer.");
        Log::SDLError();
    }

    mpFont = new PixelFont(mRenderer, mCharWidth, mCharHeight, "content/type/font-layout.txt", "content/type/test-font.png");

	mCache = new ascii::ImageCache(mRenderer,
            mCharWidth,
            mCharHeight);
}

void ascii::Graphics::Dispose()
{
    delete mpFont;

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
    return pixelY / mCharHeight;
}

int ascii::Graphics::cellToPixelX(int cellX)
{
    return drawOrigin().x + (cellX * mCharWidth);
}

int ascii::Graphics::cellToPixelY(int cellY)
{
    return drawOrigin().y + (cellY * mCharHeight);
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
        // Draw every image in the given map otherwise, using their specified
        // positions
        for (auto it = images->begin(); it != images->end(); ++it)
        {
            SDL_Rect dest;
            
            dest.x = cellToPixelX(it->second.second.x);
            dest.y = cellToPixelY(it->second.second.y);

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

			colorRect.x = cellToPixelX(x + xSrc);
			colorRect.y = cellToPixelY(y + ySrc);
			colorRect.w = 0;
			colorRect.h = mCharHeight;

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
	//draw all characters
	for (int ySrc = 0; ySrc < surface->height(); ++ySrc)
	{
        for (int xSrc = 0; xSrc < surface->width(); ++xSrc)
        {
            UChar character = surface->getCharacter(xSrc, ySrc);

            if (!IsWhiteSpace(character))
            {
                Color color = surface->getCharacterColor(xSrc, ySrc);

                int pixelX = cellToPixelX(x + xSrc);
                int pixelY = cellToPixelY(y + ySrc);

                //Log::Print("Rendering character: " + UnicodeString(character));
                //Log::Print("x: ", false);
                //Log::Print(pixelX);
                //Log::Print("y: ", false);
                //Log::Print(pixelY);
                mpFont->RenderCharacter(character, pixelX, pixelY, color);
            }
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

ascii::Point ascii::Graphics::drawResolution()
{
    return ascii::Point(mCharWidth * width(), mCharHeight * height());
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

        SDL_assert(width() * mCharWidth == w && height() * mCharHeight == h);
    }
}
