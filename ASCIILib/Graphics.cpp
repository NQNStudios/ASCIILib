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
	SDL_Rect charRect;

	for (int x = 0; x < kBufferWidth; ++x)
	{
		for (int y = 0; y < kBufferHeight; ++y)
		{
			charRect.x = x * mCharWidth;
			charRect.y = y * mCharHeight;
			charRect.w = mCharWidth;
			charRect.h = mCharHeight;

			if (mBuffer->getCharacter(x, y) == 'x')
			{
				mBuffer->getBackgroundColor(x, y);
			}

			std::stringstream charstream;
			charstream << mBuffer->getCharacter(x, y);
			std::string character;
			charstream >> character;

			Color backgroundColor = mBuffer->getBackgroundColor(x, y);
			Color characterColor = mBuffer->getCharacterColor(x, y);

			SDL_SetRenderDrawColor(mRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, Color::kAlpha);
			SDL_RenderFillRect(mRenderer, &charRect);

			if (strcmp(character.c_str(), " "))
			{
				SDL_Surface* characterSurface = TTF_RenderText_Solid(mFont, character.c_str(), characterColor);
				SDL_Texture* characterTexture = SDL_CreateTextureFromSurface(mRenderer, characterSurface);

				SDL_RenderCopy(mRenderer, characterTexture, NULL, &charRect);

				SDL_DestroyTexture(characterTexture);
				SDL_FreeSurface(characterSurface);
			}
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