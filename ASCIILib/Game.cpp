#include "Game.h"

#include <stdio.h>

#include <SDL_image.h>

const int kFPS = 60;
const int kMaxFrameTime = 5 * 1000 / 60;

ascii::Game::Game(const char* title, const int bufferWidth, const int bufferHeight, void (*loadContent)(ImageCache*, SoundManager*), void (*update)(Game*, int), void (*handleInput)(Game*, Input&), void (*draw)(Graphics&))
	: mLoadContent(loadContent), mUpdate(update), mHandleInput(handleInput), mDraw(draw), mBufferWidth(bufferWidth), mBufferHeight(bufferHeight), mWindowTitle(title), mCache(NULL), mRunning(false)
{
	mSoundManager = new SoundManager();
}

ascii::Game::Game(const char* title, void (*loadContent)(ImageCache*, SoundManager*), void (*update)(Game*, int), void (*handleInput)(Game*, Input&), void (*draw)(Graphics&))
	: mLoadContent(loadContent), mUpdate(update), mHandleInput(handleInput), mDraw(draw), mBufferWidth(0), mBufferHeight(0), mWindowTitle(title), mCache(NULL), mRunning(false)
{
	mSoundManager = new SoundManager();
}

void ascii::Game::Run()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP);

	if (mBufferWidth != 0 && mBufferHeight != 0)
	{
		mGraphics = new ascii::Graphics(mWindowTitle, mBufferWidth, mBufferHeight);
	}
	else
	{
		mGraphics = new ascii::Graphics(mWindowTitle);
	}

	mCache = mGraphics->imageCache();
	mLoadContent(mCache, mSoundManager);

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

		mHandleInput(this, input);

		const int currentTime = SDL_GetTicks();
		const int elapsedTime = currentTime - lastUpdateTime;

		mSoundManager->update();
		mUpdate(this, std::min(elapsedTime, kMaxFrameTime));
		lastUpdateTime = currentTime;

		mDraw(*mGraphics);

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
	delete mCache;
	delete mSoundManager;

	SDL_Quit();
}