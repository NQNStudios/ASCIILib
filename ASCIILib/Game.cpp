#include "Game.h"

#include <stdio.h>

#include <SDL_image.h>

const int kFPS = 60;
const int kMaxFrameTime = 5 * 1000 / 60;

ascii::Game::Game(const char* title, const char* fontpath, const int bufferWidth, const int bufferHeight)
	: mBufferWidth(bufferWidth), mBufferHeight(bufferHeight), mWindowTitle(title), mFontpath(fontpath), mRunning(false)
{
	mSoundManager = new SoundManager();
}

ascii::Game::Game(const char* title, const char* fontpath)
	: mBufferWidth(0), mBufferHeight(0), mWindowTitle(title), mFontpath(fontpath), mRunning(false)
{
	mSoundManager = new SoundManager();
}

ascii::Game::~Game()
{
    delete mGraphics;
	delete mSoundManager;

	SDL_Quit();
}

void ascii::Game::Run()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP);

	if (mBufferWidth != 0 && mBufferHeight != 0)
	{
		mGraphics = new ascii::Graphics(mWindowTitle, mFontpath, mBufferWidth, mBufferHeight);
	}
	else
	{
		mGraphics = new ascii::Graphics(mWindowTitle, mFontpath);
	}

	LoadContent(imageCache(), mSoundManager);

	ascii::Input input;

	mRunning = true;

	int lastUpdateTime = SDL_GetTicks();

	while (mRunning)
	{
		const int initialTime = SDL_GetTicks();

		input.beginNewFrame();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					Quit();
					return;
				case SDL_KEYDOWN:
					input.keyDownEvent(event);
					break;
				case SDL_KEYUP:
					input.keyUpEvent(event);
					break;
				case SDL_MOUSEWHEEL:
					input.scrollEvent(event);
					break;
			}
		}

		HandleInput(input);

		const int currentTime = SDL_GetTicks();
		const int elapsedTime = currentTime - lastUpdateTime;

		Update(std::min(elapsedTime, kMaxFrameTime));
		lastUpdateTime = currentTime;

        mSoundManager->update();

		Draw(*mGraphics);

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
