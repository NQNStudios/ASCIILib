#include "Graphics.h"

const int kFontSize = 12;
const char* kFontPath = "terminal.fon";

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;
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
}


ascii::Graphics::~Graphics(void)
{
	SDL_DestroyRenderer(mRenderer);
	
	SDL_DestroyWindow(mWindow);

	TTF_CloseFont(mFont);

	TTF_Quit();
}
