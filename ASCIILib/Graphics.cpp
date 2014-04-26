#include "Graphics.h"

#include <sstream>

const int kFontSize = 12;
const char* kFontPath = "terminal.fon";

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;

//static
const unsigned int ascii::Graphics::kBufferHeight = 25;

ascii::Graphics::Graphics(const char* title)
	: Surface(kBufferWidth, kBufferHeight)
{
	TTF_Init();

	mFont = TTF_OpenFont(kFontPath, kFontSize);
	TTF_SizeText(mFont, " ", &mCharWidth, &mCharHeight);

	mWindow = SDL_CreateWindow(title, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		kBufferWidth * mCharWidth, kBufferHeight * mCharHeight, 
		SDL_WINDOW_SHOWN);

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
}

ascii::Graphics::Graphics(const char* title, int bufferWidth, int bufferHeight)
	: Surface(bufferWidth, bufferHeight)
{
	TTF_Init();

	mFont = TTF_OpenFont(kFontPath, kFontSize);
	TTF_SizeText(mFont, " ", &mCharWidth, &mCharHeight);

	mWindow = SDL_CreateWindow(title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		bufferWidth * mCharWidth, bufferHeight * mCharHeight,
		SDL_WINDOW_SHOWN);

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
}

ascii::Graphics::~Graphics(void)
{
	for (std::map<Glyph, SDL_Texture*>::iterator it = mStringTextures.begin(); it != mStringTextures.end(); ++it)
	{
		SDL_DestroyTexture(it->second); //destroy all stored string textures
	}

	SDL_DestroyRenderer(mRenderer);
	
	SDL_DestroyWindow(mWindow);

	TTF_CloseFont(mFont);

	TTF_Quit();
}

ascii::ImageCache* ascii::Graphics::createImageCache()
{
	return new ascii::ImageCache(mRenderer, mCharWidth, mCharHeight);
}

void ascii::Graphics::update()
{
	//draw all background colors
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
				colorRect.w += mCharWidth;
				++x;
			} while (x < width() && getBackgroundColor(x, y) == backgroundColor);

			SDL_SetRenderDrawColor(mRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, Color::kAlpha);
			SDL_RenderFillRect(mRenderer, &colorRect);
		}
	}

	//draw all images
	for (auto it = mImages.begin(); it != mImages.end(); ++it)
	{
		ascii::Point pos(it->first);

		SDL_Texture* texture = it->second;

		int w, h;

		SDL_QueryTexture(texture, NULL, NULL, &w, &h);

		SDL_Rect dest;
		dest.x = pos.x * mCharWidth;
		dest.y = pos.y * mCharHeight;
		dest.w = w;
		dest.h = h;

		SDL_RenderCopy(mRenderer, texture, NULL, &dest);
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
				char ch = getCharacter(x, y);
				charstream << ch;
				textRect.w += mCharWidth;
				++x;
			} while (x < width() && getCharacterColor(x, y) == characterColor && getCharacter(x, y) != ' ');

			std::string str;
			charstream >> str;

			Glyph glyph = std::make_pair(str, characterColor);

			SDL_Texture* texture = NULL;

			if (mStringTextures[glyph])
			{
				texture = mStringTextures[glyph];
			}
			else
			{
				SDL_Surface* surface = TTF_RenderText_Solid(mFont, str.c_str(), characterColor);
				texture = SDL_CreateTextureFromSurface(mRenderer, surface);

				mStringTextures[glyph] = texture;
				
				SDL_FreeSurface(surface);
			}

			SDL_RenderCopy(mRenderer, texture, NULL, &textRect);
		}
	}

	SDL_RenderPresent(mRenderer);
}