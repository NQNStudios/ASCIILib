#include "Game.h"

#include <stdio.h>
#include <algorithm>
#include <SDL_image.h>

#include "Log.h"
using namespace ascii;

const int kFPS = 60;
const int kMaxFrameTime = 5 * 1000 / 60;

ascii::Game::Game(const char* title, const int bufferWidth, const int bufferHeight,
        int charWidth, int charHeight)
	: mBufferWidth(bufferWidth), mBufferHeight(bufferHeight), mWindowTitle(title), mRunning(false),
    mTextManager(&mLanguageManager), mFirstInputFrame(true)
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

	mpSoundManager = new SoundManager();

	mpGraphics = new ascii::Graphics(mWindowTitle, charWidth, charHeight, mBufferWidth, mBufferHeight);

	mpInput = new Input();
}

ascii::Game::~Game()
{
	Log::Print("Game deconstructor called");
	delete mpGraphics;
	delete mpSoundManager;
    delete mpInput;

    Log::Print("Calling IMG_Quit()");
	IMG_Quit();

	Log::Print("Calling SDL_Quit()");
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
    mpContentManager = new ContentManager(this);

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
					Log::Print("Handling SDL_QUIT event.");
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
        mFirstInputFrame = false;

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

	UnloadContent(mpGraphics->imageCache(), mpSoundManager);
}

void ascii::Game::Quit()
{
    mRunning = false;
}

void ascii::Game::SetLanguage(int index)
{
    if (index != mLanguageManager.CurrentPackIndex())
    {
        // Set the language of the game
        mLanguageManager.SetPack(index);

        // Reload text files in the new language
        mTextManager.ReloadFiles();
    }

    // Save the selected language in JSON
    config()->SetInt("language", index);
    config()->WriteValues();
}

void ascii::Game::ActivateCondition(string condition)
{
    mActiveConditions.push_back(condition);
}

void ascii::Game::DeactivateCondition(string condition)
{
    mActiveConditions.erase(remove(
                mActiveConditions.begin(), mActiveConditions.end(), condition));
}

bool ascii::Game::ConditionIsActive(string condition)
{
    return find(mActiveConditions.begin(), mActiveConditions.end(), condition)
        != mActiveConditions.end();
}


