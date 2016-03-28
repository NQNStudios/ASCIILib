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
        ToggleFullscreen();
    }
    else
    {
        Dispose();
    }
    Initialize(scale);
}

void ascii::Graphics::SetFullscreen(bool fullscreen)
{
    mFullscreen = fullscreen;

    // Delete the old window if its scale is not 1, so fullscreen can scale
    // automatically
    if (mScale != 1.0f)
    {
        SetScale(1.0f);
    }

    Uint32 flags = 0; 
    if (fullscreen)
    {
        flags = flags | SDL_WINDOW_FULLSCREEN;
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
    return pixelX / mCharWidth;
}

int ascii::Graphics::pixelToCellY(int pixelY)
{
    return pixelY / mCharHeight;
}

void ascii::Graphics::update()
{
	//draw background color
	SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, ascii::Color::kAlpha);
	SDL_RenderFillRect(mRenderer, NULL);
	
	//draw background images
    if (!mHidingImages)
    {
        for (auto it = mBackgroundImages.begin(); it != mBackgroundImages.end(); ++it)
        {
            SDL_Rect dest;
            
            dest.x = it->second.second.x * mCharWidth;
            dest.y = it->second.second.y * mCharHeight;

            SDL_QueryTexture(it->second.first, NULL, NULL, &dest.w, &dest.h);

            SDL_RenderCopy(mRenderer, it->second.first, NULL, &dest);
        }
    }

	//draw all buffer background colors
	for (int y = 0; y < height(); ++y)
	{
		int x = 0;

		while (x < width())
		{
			//chain all adjacent background colors in a row for more efficient rendering
			SDL_Rect colorRect;

			colorRect.x = x * mCharWidth;
			colorRect.y = y * mCharHeight;
			colorRect.w = 0;
			colorRect.h = mCharHeight;

			Color backgroundColor = getBackgroundColor(x, y);

			do
			{
				if (!isCellOpaque(x, y))
                {
                    ++x;
                    break;
                }

				colorRect.w += mCharWidth;
				++x;
			} while (x < width() && getBackgroundColor(x, y) == backgroundColor);

			SDL_SetRenderDrawColor(mRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, Color::kAlpha);
			SDL_RenderFillRect(mRenderer, &colorRect);
		}
	}

	//draw all characters
	for (int y = 0; y < height(); ++y)
	{
		int x = 0;

		while (x < width())
		{
			//chain all adjacent characters with the same color into strings for more efficient rendering

			char ch = getCharacter(x, y);
			if (ch == ' ')
			{
				++x;
				continue;
			}

			std::stringstream charstream;
			SDL_Rect textRect;

			textRect.x = x * mCharWidth;
			textRect.y = y * mCharHeight;
			textRect.w = 0;
			textRect.h = mCharHeight;
			Color characterColor = getCharacterColor(x, y);

			do
			{
				if (!isCellOpaque(x, y))
				{
					++x;
					break;
				}

				char ch = getCharacter(x, y);
				charstream << ch;
				textRect.w += mCharWidth;
				++x;
			} while (x < width() && getCharacterColor(x, y) == characterColor && getCharacter(x, y) != ' ');

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

	//draw foreground images
    if (!mHidingImages)
    {
        for (auto it = mForegroundImages.begin(); it != mForegroundImages.end(); ++it)
        {
            SDL_Rect dest;
            
            dest.x = it->second.second.x * mCharWidth;
            dest.y = it->second.second.y * mCharHeight;

            SDL_QueryTexture(it->second.first, NULL, NULL, &dest.w, &dest.h);
            dest.w *= mScale;
            dest.h *= mScale;


            SDL_RenderCopy(mRenderer, it->second.first, NULL, &dest);
        }
    }

	SDL_RenderPresent(mRenderer);
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
