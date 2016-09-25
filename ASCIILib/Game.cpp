#include "Game.h"

#include <stdio.h>
#include <algorithm>
#include <SDL_image.h>

#include "Log.h"
using namespace ascii;

const int kFPS = 60;
const int kMaxFrameTime = 5 * 1000 / 60;

ascii::Game::Game(const char* title, const char* fontpath, const int bufferWidth, const int bufferHeight)
	: mBufferWidth(bufferWidth), mBufferHeight(bufferHeight), mWindowTitle(title), mFontpath(fontpath), mRunning(false)
{
	mpSoundManager = new SoundManager();
}

ascii::Game::Game(const char* title, const char* fontpath)
	: mBufferWidth(0), mBufferHeight(0), mWindowTitle(title), mFontpath(fontpath), mRunning(false)
{
	mpSoundManager = new SoundManager();
}

ascii::Game::~Game()
{
    delete mpGraphics;
	delete mpSoundManager;
    delete mpInput;

	SDL_Quit();
}

void ascii::Game::ShowMouseCursor()
{
    SDL_ShowCursor(SDL_ENABLE);
}

void ascii::Game::HideMouseCursor()
{
    SDL_ShowCursor(SDL_DISABLE);
}

void ascii::Game::Run()
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        Log::Error("SDL_Init failed!");
        Log::SDLError();
    }
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        Log::Error("SDL_image failed to initialize with PNG and JPG support");
        Log::SDLError();
    }

	if (mBufferWidth != 0 && mBufferHeight != 0)
	{
		mpGraphics = new ascii::Graphics(mWindowTitle, mFontpath, mBufferWidth, mBufferHeight);
	}
	else
	{
		mpGraphics = new ascii::Graphics(mWindowTitle, mFontpath);
	}

    mpInput = new Input();

	LoadContent(imageCache(), mpSoundManager);

	mRunning = true;

	int lastUpdateTime = SDL_GetTicks();

	while (mRunning)
	{
		const int initialTime = SDL_GetTicks();

		mpInput->beginNewFrame();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					Quit();
					return;
				case SDL_KEYDOWN:
					mpInput->keyDownEvent(event);
					break;
				case SDL_KEYUP:
					mpInput->keyUpEvent(event);
					break;
				case SDL_MOUSEWHEEL:
					mpInput->scrollEvent(event);
					break;
                case SDL_WINDOWEVENT:
                    HandleWindowEvent(event);
                    break;
			}
		}

		HandleInput(*mpInput);

		const int currentTime = SDL_GetTicks();
		const int elapsedTime = currentTime - lastUpdateTime;

		Update(std::min(elapsedTime, kMaxFrameTime));
		lastUpdateTime = currentTime;

        mpSoundManager->update(elapsedTime);

		Draw(*mpGraphics);

		const int msPerFrame = 1000 / kFPS;
		const int elapsedTimeMS = SDL_GetTicks() - initialTime;

		if (elapsedTimeMS < msPerFrame)
		{
			SDL_Delay(msPerFrame - elapsedTimeMS);
		}
	}
}

void ascii::Game::Quit()
{
    mRunning = false;
}
