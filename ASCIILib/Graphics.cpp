#include "Graphics.h"

#include <sstream>
#include <string>

const int kFontSize = 12;
const char* kFontPath = "terminal.fon";

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;

//static
const unsigned int ascii::Graphics::kBufferHeight = 25;

ascii::Graphics::Graphics(const char* title)
{
	TTF_Init();

	mFont = TTF_OpenFont(kFontPath, kFontSize);
	TTF_SizeText(mFont, " ", &mCharWidth, &mCharHeight);

	mWindow = SDL_CreateWindow(title, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		kBufferWidth * mCharWidth, kBufferHeight * mCharHeight, 
		SDL_WINDOW_SHOWN);

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

	mBuffer = new Surface(kBufferWidth, kBufferHeight);
}


ascii::Graphics::~Graphics(void)
{
	delete mBuffer;

	SDL_DestroyRenderer(mRenderer);
	
	SDL_DestroyWindow(mWindow);

	TTF_CloseFont(mFont);

	TTF_Quit();
}

void ascii::Graphics::clear()
{
	mBuffer->clear();
}

void ascii::Graphics::update()
{
	//draw all background colors
	for (int y = 0; y < kBufferHeight; ++y)
	{
		int x = 0;

		while (x < kBufferWidth)
		{
			//chain all adjacent background colors in a row for more efficient rendering
			SDL_Rect colorRect;

			colorRect.x = x * mCharWidth;
			colorRect.y = y * mCharHeight;
			colorRect.w = 0;
			colorRect.h = mCharHeight;

			Color backgroundColor = mBuffer->getBackgroundColor(x, y);

			do
			{
				colorRect.w += mCharWidth;
				++x;
			} while (x < kBufferWidth && mBuffer->getBackgroundColor(x, y) == backgroundColor);

			SDL_SetRenderDrawColor(mRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, Color::kAlpha);
			SDL_RenderFillRect(mRenderer, &colorRect);
		}
	}

	//draw all characters
	for (int y = 0; y < kBufferHeight; ++y)
	{
		int x = 0;

		while (x < kBufferWidth)
		{
			//chain all adjacent characters with the same color into strings for more efficient rendering

			char ch = mBuffer->getCharacter(x, y);
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
			Color characterColor = mBuffer->getCharacterColor(x, y);

			do
			{
				char ch = mBuffer->getCharacter(x, y);
				charstream << ch;
				textRect.w += mCharWidth;
				++x;
			} while (x < kBufferWidth && mBuffer->getCharacterColor(x, y) == characterColor && mBuffer->getCharacter(x, y) != ' ');

			std::string str;
			charstream >> str;

			SDL_Surface* surface = TTF_RenderText_Solid(mFont, str.c_str(), characterColor);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);

			SDL_RenderCopy(mRenderer, texture, NULL, &textRect);

			SDL_DestroyTexture(texture);
			SDL_FreeSurface(surface);
		}
	}

	SDL_RenderPresent(mRenderer);
}

void ascii::Graphics::blitSurface(Surface* surface, int x, int y)
{
	mBuffer->blitSurface(surface, x, y);
}

void ascii::Graphics::blitSurface(Surface* surface, Rectangle source, int x, int y)
{
	mBuffer->blitSurface(surface, source, x, y);
}