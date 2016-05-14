#include "Graphics.h"

#include <sstream>
#include <iostream>
using std::cout;
using std::endl;

const int kFontSize = 12;

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;

//static
const unsigned int ascii::Graphics::kBufferHeight = 25;

ascii::Graphics::Graphics(const char* title, const char* fontpath)
	: Surface(kBufferWidth, kBufferHeight),
    mTitle(title), mScale(1.0f), mFullscreen(false),
    mBackgroundColor(ascii::Color::Black), mWindow(NULL), mRenderer(NULL),
    mHidingImages(false)
{
	TTF_Init();

	mFont = TTF_OpenFont(fontpath, kFontSize);
    
    Initialize(mScale);
}

ascii::Graphics::Graphics(const char* title, const char* fontpath,
        int bufferWidth, int bufferHeight)
	: Surface(bufferWidth, bufferHeight), mTitle(title), mScale(1.0f),
    mFullscreen(false), mBackgroundColor(ascii::Color::Black),
    mWindow(NULL), mRenderer(NULL), mHidingImages(false)
{
	TTF_Init();

	mFont = TTF_OpenFont(fontpath, kFontSize);

    Initialize(mScale);
}

ascii::Graphics::~Graphics(void)
{
    Dispose();

	TTF_CloseFont(mFont);

	TTF_Quit();
}

void ascii::Graphics::Initialize(float scale)
{
    mScale = scale;
    mFullscreen = false;

    UpdateCharSize();

    int flags = SDL_WINDOW_SHOWN;

	mWindow = SDL_CreateWindow(mTitle, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		width() * mCharWidth, height() * mCharHeight, 
		flags);

	checkSize();

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

	mCache = new ascii::ImageCache(mRenderer,
            mCharWidth / mScale,
            mCharHeight / mScale);
}

void ascii::Graphics::Dispose()
{
    clearGlyphs();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    delete mCache;
}

void ascii::Graphics::SetScale(float scale)
{
    if (mFullscreen)
    {
        mScale = scale;
        UpdateCharSize();
    }
    else
    {
        Dispose();
        Initialize(scale);
    }
}

void ascii::Graphics::SetFullscreen(bool fullscreen)
{
    mFullscreen = fullscreen;

    // Delete the old window if its scale is not 1, so fullscreen can scale
    // automatically
    if (fullscreen && mScale != 1.0f)
    {
        SetScale(1.0f);
    }

    Uint32 flags = 0; 
    if (fullscreen)
    {
        flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    else
    {
        Dispose();
        Initialize(1.0f);
    }

    if (SDL_SetWindowFullscreen(mWindow, flags) != 0)
    {
        cout << SDL_GetError() << endl;
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
	//draw background color
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
            dest.y = drawY + it->second.second.y * mCharHeight;

            SDL_QueryTexture(it->second.first, NULL, NULL, &dest.w, &dest.h);

            dest.w *= mScale;
            dest.h *= mScale;

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
			colorRect.y = drawY + (y + ySrc) * mCharHeight;
			colorRect.w = 0;
			colorRect.h = mCharHeight;

			Color backgroundColor = surface->getBackgroundColor(xSrc, ySrc);

			do
			{
				if (!isCellOpaque(xSrc, ySrc))
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
			//chain all adjacent characters with the same color into strings for more efficient rendering

			char ch = surface->getCharacter(xSrc, ySrc);
			if (ch == ' ')
			{
				++xSrc;
				continue;
			}

			std::stringstream charstream;
			SDL_Rect textRect;

			textRect.x = drawX + (x + xSrc) * mCharWidth;
			textRect.y = drawY + (y + ySrc) * mCharHeight;
			textRect.w = 0;
			textRect.h = mCharHeight;
			Color characterColor = surface->getCharacterColor(xSrc, ySrc);

			do
			{
				if (!surface->isCellOpaque(xSrc, ySrc))
				{
					++xSrc;
					break;
				}

				char ch = surface->getCharacter(xSrc, ySrc);
				charstream << ch;
				textRect.w += mCharWidth;
				++xSrc;
			} while (xSrc < surface->width() && surface->getCharacterColor(xSrc, ySrc) == characterColor && surface->getCharacter(xSrc, ySrc) != ' ');

			std::string str;
			charstream >> str;

			Glyph glyph = std::make_pair(str, characterColor);

			SDL_Texture* texture = NULL;

			if (mGlyphTextures[glyph])
			{
				texture = mGlyphTextures[glyph];
			}
			else
			{
				SDL_Surface* surface = TTF_RenderText_Solid(mFont, str.c_str(), characterColor);
				texture = SDL_CreateTextureFromSurface(mRenderer, surface);

				mGlyphTextures[glyph] = texture;
				
				SDL_FreeSurface(surface);
			}

			SDL_RenderCopy(mRenderer, texture, NULL, &textRect);
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

	//draw foreground images
    drawImages(&mForegroundImages);

    // Refresh the window to show all changes
    refresh();
}

void ascii::Graphics::directRenderSurface(ascii::Surface* surface, int x, int y)
{
    clearScreen();
    drawImages(&mBackgroundImages);
    drawSurface(this, 0, 0);
    drawImages(&mForegroundImages);
    drawSurface(surface, x, y);
    refresh();
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

void ascii::Graphics::UpdateCharSize()
{
	TTF_SizeText(mFont, " ", &mCharWidth, &mCharHeight);
    mCharWidth *= mScale;
    mCharHeight *= mScale;
}
