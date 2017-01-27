#include "Graphics.h"

#include <algorithm>
#include <string>
#include <sstream>

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
        int bufferWidth, int bufferHeight)
	: Surface(bufferWidth, bufferHeight),
    mTitle(title),
    mFullscreen(false), mBackgroundColor(ascii::Color::Black),
    mpWindow(NULL), mpRenderer(NULL), mHidingImages(false),
    mCellFonts(bufferWidth, vector<string>(bufferHeight, "")),
    mCharWidth(charWidth), mCharHeight(charHeight)
{
    // Retrieve the native max resolution of the player's display
    SDL_DisplayMode mode;

    if (SDL_GetDesktopDisplayMode(0, &mode) != 0)
    {
        Log::Error("Failed to get display mode of the current display.");
        Log::SDLError();
    }

    mSystemFullscreenWidth = mode.w;
    mSystemFullscreenHeight = mode.h;

    // Initialize graphics at the default scale and everything
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

	mpWindow = SDL_CreateWindow(mTitle, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		width() * mCharWidth * mScale, height() * mCharHeight * mScale,
		flags);

    if (!mpWindow)
    {
        Log::Error("Failed to create a window for the game.");
        Log::SDLError();
    }

	checkSize();

	mpRenderer = SDL_CreateRenderer(mpWindow, -1, SDL_RENDERER_ACCELERATED);

    if (!mpRenderer)
    {
        Log::Error("Failed to create SDL_Renderer.");
        Log::SDLError();
    }

	mpCache = new ascii::ImageCache(mpRenderer,
            mCharWidth,
            mCharHeight);

    for (auto it = mFonts.begin(); it != mFonts.end(); ++it)
    {
        if (it->second->charHeight() == mCharHeight * mScale)
        {
            it->second->Initialize(mpRenderer);
        }
    }
    
    // Use linear scaling
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

void ascii::Graphics::SetScale(float scale)
{
//    if (scale != mScale)
//    {
        mScale = scale;
        
        Dispose();
        Initialize();
//    }


    // Switch the default font to the proper size
}

void ascii::Graphics::Dispose()
{
    SDL_DestroyRenderer(mpRenderer);
    SDL_DestroyWindow(mpWindow);

    // Dispose of fonts
    for (auto it = mFonts.begin(); it != mFonts.end(); ++it)
    {
        it->second->Dispose();
    }

    delete mpCache;
}

void ascii::Graphics::AddFont(string key, int size, string fontLayoutPath, string fontPath)
{
    stringstream sstream;
    sstream << key << size;
    
    float scale = (float) size / (float) mCharHeight;
    mFonts[sstream.str()] = new PixelFont(mCharWidth * scale, size, fontLayoutPath, fontPath);

    if (size == mCharHeight * mScale)
    {
        mFonts[sstream.str()]->Initialize(mpRenderer);
    }
}

void ascii::Graphics::UnloadFont(string key, int size)
{
    stringstream sstream;
    sstream << key << size;
    PixelFont* pFont = mFonts[sstream.str()];
    delete pFont;
    mFonts.erase(key);
}

void ascii::Graphics::UnloadAllFonts()
{
    for (auto it = mFonts.begin(); it != mFonts.end(); ++it)
    {
        delete it->second;
    }
    mFonts.clear();
}

void ascii::Graphics::SetDefaultFont(string key)
{
    mDefaultFont = key;
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

    if (SDL_SetWindowFullscreen(mpWindow, flags) != 0)
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
    return pixelX / (mCharWidth * mScale);
}

int ascii::Graphics::pixelToCellY(int pixelY)
{
    if (drawOrigin().y > 0)
    {
        pixelY -= drawOrigin().y;
    }
    return pixelY / (mCharHeight * mScale);
}

int ascii::Graphics::cellToPixelX(int cellX)
{
    return drawOrigin().x + (cellX * mCharWidth * mScale);
}

int ascii::Graphics::cellToPixelY(int cellY)
{
    return drawOrigin().y + (cellY * mCharHeight * mScale);
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
	SDL_SetRenderDrawColor(mpRenderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, ascii::Color::kAlpha);
	SDL_RenderFillRect(mpRenderer, NULL);
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
            dest.w *= mScale;
            dest.h *= mScale;

            SDL_RenderCopy(mpRenderer, it->second.first, NULL, &dest);
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
			colorRect.h = mCharHeight * mScale;

			Color backgroundColor = surface->getBackgroundColor(xSrc, ySrc);

			do
			{
				if (!surface->isCellOpaque(xSrc, ySrc))
                {
                    ++xSrc;
                    break;
                }

				colorRect.w += mCharWidth * mScale;
				++xSrc;
			} while (xSrc < surface->width() && surface->getBackgroundColor(xSrc, ySrc) == backgroundColor);

			SDL_SetRenderDrawColor(mpRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, Color::kAlpha);
			SDL_RenderFillRect(mpRenderer, &colorRect);
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

            if (!IsWhiteSpace(character) && surface->isCellOpaque(xSrc, ySrc))
            {
                Color color = surface->getCharacterColor(xSrc, ySrc);

                int destCellX = x + xSrc;
                int destCellY = y + ySrc;
                int destPixelX = cellToPixelX(destCellX);
                int destPixelY = cellToPixelY(destCellY);

                //Log::Print("Rendering character: " + UnicodeString(character));
                //Log::Print("x: ", false);
                //Log::Print(pixelX);
                //Log::Print("y: ", false);
                //Log::Print(pixelY);

                string cellFont = mCellFonts[destCellX][destCellY];

                PixelFont* font = GetFont(cellFont);

                if (font)
                    font->RenderCharacter(character, destPixelX, destPixelY, color);
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
    SDL_RenderPresent(mpRenderer);
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
	mBackgroundImages[key] = std::make_pair(mpCache->getTexture(textureKey), ascii::Point(x, y));
}

void ascii::Graphics::removeBackgroundImage(std::string key)
{
	mBackgroundImages.erase(key);
}

void ascii::Graphics::addForegroundImage(std::string key, std::string textureKey, int x, int y)
{
	mForegroundImages[key] = std::make_pair(mpCache->getTexture(textureKey), ascii::Point(x, y));
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
    return ascii::Point(mCharWidth * width() * mScale, mCharHeight * height() * mScale);
}

ascii::Point ascii::Graphics::actualResolution()
{
    int w, h;
    SDL_GetWindowSize(mpWindow, &w, &h);
    return ascii::Point(w, h);
}

void ascii::Graphics::checkSize()
{
    if (!mFullscreen)
    {
        int w, h;
        SDL_GetWindowSize(mpWindow, &w, &h);

        bool check = width() * mCharWidth * mScale == w && height() * mCharHeight * mScale == h;

        if (!check)
        {
            Log::Error("The size of the created window does not match expected dimensions! This could be because the width and height of the graphics buffer are too small.");
        }
    }
}

void ascii::Graphics::clearCellFonts()
{
    for (int x = 0; x < width(); ++x)
    {
        for (int y = 0; y < height(); ++y)
        {
            mCellFonts[x][y] = "";
        }
    }
}

void ascii::Graphics::setCellFont(Rectangle cells, string font)
{
    for (int x = cells.x; x < cells.right(); ++x)
    {
        for (int y = cells.y; y < cells.bottom(); ++y)
        {
            mCellFonts[x][y] = font;
        }
    }
}

PixelFont* ascii::Graphics::GetFont(string key)
{
    PixelFont* font = NULL;

    if (key.empty())
    {
        key = mDefaultFont;
    }

    stringstream sstream;
    sstream << key << (mCharHeight * mScale);
    key = sstream.str();

    if (mFonts.find(key) != mFonts.end())
    {
        font = mFonts[key];
    }
    else
    {
        string error = "Graphics tried to render a character in a nonexistent font: ";
        if (key.empty())
        {
            error += "[DEFAULT FONT]";
        }
        else
        {
            error += key;
        }

        Log::Error(error);
    }

    return font;
}
