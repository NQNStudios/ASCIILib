#include "Graphics.h"

#include <sstream>

const int kFontSize = 12;

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;

//static
const unsigned int ascii::Graphics::kBufferHeight = 25;

ascii::Graphics::Graphics(const char* title, const char* fontpath)
	: Surface(kBufferWidth, kBufferHeight), mTitle(title), mScale(1.0f), mFullscreen(false), mBackgroundColor(ascii::Color::Black)
{
	TTF_Init();

	mFont = TTF_OpenFont(fontpath, kFontSize);
	TTF_SizeText(mFont, " ", &mCharWidth, &mCharHeight);
    
    SetVideoMode(mScale, mFullscreen);
}

ascii::Graphics::Graphics(const char* title, const char* fontpath, int bufferWidth, int bufferHeight)
	: Surface(bufferWidth, bufferHeight), mTitle(title), mScale(1.0f), mFullscreen(false), mBackgroundColor(ascii::Color::Black)
{
	TTF_Init();

	mFont = TTF_OpenFont(fontpath, kFontSize);
	TTF_SizeText(mFont, " ", &mCharWidth, &mCharHeight);

    SetVideoMode(mScale, mFullscreen);
}

ascii::Graphics::~Graphics(void)
{
	clearGlyphs();

	SDL_DestroyRenderer(mRenderer);
	
	SDL_DestroyWindow(mWindow);

	TTF_CloseFont(mFont);

	TTF_Quit();
}

void ascii::Graphics::SetVideoMode(float scale, bool fullscreen)
{
    // If Graphics has already been initialized, delete everything
    if (mWindow != NULL)
    {
        clearGlyphs();

        SDL_DestroyRenderer(mRenderer);

        SDL_DestroyWindow(mWindow);

        delete mCache;
    }

    mScale = scale;
    mFullscreen = fullscreen;

    int flags = SDL_WINDOW_SHOWN;

    if (fullscreen)
    {
        flags = flags | SDL_WINDOW_FULLSCREEN;
    }

	mWindow = SDL_CreateWindow(mTitle, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		width() * mCharWidth * mScale, height() * mCharHeight * mScale, 
		flags);

	checkSize();

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

	mCache = new ascii::ImageCache(mRenderer, mCharWidth, mCharHeight);
}

int ascii::Graphics::pixelToCellX(int pixelX)
{
    return pixelX / mCharWidth / mScale;
}

int ascii::Graphics::pixelToCellY(int pixelY)
{
    return pixelY / mCharHeight / mScale;
}

void ascii::Graphics::update()
{
	//draw background color
	SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, ascii::Color::kAlpha);
	SDL_RenderFillRect(mRenderer, NULL);
	
	//draw background images
	for (auto it = mBackgroundImages.begin(); it != mBackgroundImages.end(); ++it)
	{
		SDL_Rect dest;
		
		dest.x = it->second.second.x * mCharWidth * mScale;
		dest.y = it->second.second.y * mCharHeight * mScale;

		SDL_QueryTexture(it->second.first, NULL, NULL, &dest.w, &dest.h);
        dest.w *= mScale;
        dest.h *= mScale;

		SDL_RenderCopy(mRenderer, it->second.first, NULL, &dest);
	}

	//draw all buffer background colors
	for (int y = 0; y < height(); ++y)
	{
		int x = 0;

		while (x < width())
		{
			//chain all adjacent background colors in a row for more efficient rendering
			SDL_Rect colorRect;

			colorRect.x = x * mCharWidth * mScale;
			colorRect.y = y * mCharHeight * mScale;
			colorRect.w = 0;
			colorRect.h = mCharHeight * mScale;

			Color backgroundColor = getBackgroundColor(x, y);

			do
			{
				if (!isCellOpaque(x, y))
                {
                    ++x;
                    break;
                }

				colorRect.w += mCharWidth * mScale;
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

			textRect.x = x * mCharWidth * mScale;
			textRect.y = y * mCharHeight * mScale;
			textRect.w = 0;
			textRect.h = mCharHeight * mScale;
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
				textRect.w += mCharWidth * mScale;
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
	for (auto it = mForegroundImages.begin(); it != mForegroundImages.end(); ++it)
	{
		SDL_Rect dest;
		
		dest.x = it->second.second.x * mCharWidth * mScale;
		dest.y = it->second.second.y * mCharHeight * mScale;

		SDL_QueryTexture(it->second.first, NULL, NULL, &dest.w, &dest.h);
        dest.w *= mScale;
        dest.h *= mScale;


		SDL_RenderCopy(mRenderer, it->second.first, NULL, &dest);
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
	int w, h;
	SDL_GetWindowSize(mWindow, &w, &h);

	SDL_assert(width() * mCharWidth * mScale == w && height() * mCharHeight * mScale == h);
}
