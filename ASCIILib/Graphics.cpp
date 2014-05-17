#include "Graphics.h"

#include <sstream>

const int kFontSize = 12;
const char* kFontPath = "terminal.fon";

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;

//static
const unsigned int ascii::Graphics::kBufferHeight = 25;

ascii::Graphics::Graphics(const char* title)
	: Surface(kBufferWidth, kBufferHeight), mBackgroundColor(ascii::Color::Black)
{
	TTF_Init();

	mFont = TTF_OpenFont(kFontPath, kFontSize);
	TTF_SizeText(mFont, " ", &mCharWidth, &mCharHeight);

	mWindow = SDL_CreateWindow(title, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		kBufferWidth * mCharWidth, kBufferHeight * mCharHeight, 
		SDL_WINDOW_SHOWN);

	checkSize();

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

	mCache = new ascii::ImageCache(mRenderer, mCharWidth, mCharHeight);
}

ascii::Graphics::Graphics(const char* title, int bufferWidth, int bufferHeight)
	: Surface(bufferWidth, bufferHeight), mBackgroundColor(ascii::Color::Black)
{
	TTF_Init();

	mFont = TTF_OpenFont(kFontPath, kFontSize);
	TTF_SizeText(mFont, " ", &mCharWidth, &mCharHeight);

	mWindow = SDL_CreateWindow(title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		bufferWidth * mCharWidth, bufferHeight * mCharHeight,
		SDL_WINDOW_SHOWN);

	checkSize();

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

	mCache = new ascii::ImageCache(mRenderer, mCharWidth, mCharHeight);
}

ascii::Graphics::~Graphics(void)
{
	clearGlyphs();

	SDL_DestroyRenderer(mRenderer);
	
	SDL_DestroyWindow(mWindow);

	TTF_CloseFont(mFont);

	TTF_Quit();
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
		
		dest.x = it->second.second.x * mCharWidth;
		dest.y = it->second.second.y * mCharHeight;

		SDL_QueryTexture(it->second.first, NULL, NULL, &dest.w, &dest.h);

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
	for (auto it = mForegroundImages.begin(); it != mForegroundImages.end(); ++it)
	{
		SDL_Rect dest;
		
		dest.x = it->second.second.x * mCharWidth;
		dest.y = it->second.second.y * mCharHeight;
		SDL_QueryTexture(it->second.first, NULL, NULL, &dest.w, &dest.h);

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

	SDL_assert(width() * mCharWidth == w && height() * mCharHeight == h);
}