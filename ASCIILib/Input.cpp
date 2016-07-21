#include "Input.h"


void ascii::Input::beginNewFrame()
{
	mPressedKeys.clear();
	mReleasedKeys.clear();

	// Take mouse input and update last mouse input
	mLastMouseX = mMouseX;
	mLastMouseY = mMouseY;
	mLastMouseState = mMouseState;

	mMouseState = SDL_GetMouseState(&mMouseX, &mMouseY);

	mScrollX = 0;
	mScrollY = 0;

    mFirstFrame = false;
}

void ascii::Input::keyDownEvent(const SDL_Event& event)
{
	if (!event.key.repeat)
	{
		mPressedKeys[event.key.keysym.sym] = true;
		mHeldKeys[event.key.keysym.sym] = true;
	}
}

void ascii::Input::keyUpEvent(const SDL_Event& event)
{
	mReleasedKeys[event.key.keysym.sym] = true;
	mHeldKeys[event.key.keysym.sym] = false;
}

void ascii::Input::scrollEvent(const SDL_Event& event)
{
	mScrollX = event.wheel.x;
	mScrollY = event.wheel.y;
}

bool ascii::Input::wasKeyPressed(SDL_Keycode key)
{
	return mPressedKeys[key];
}

bool ascii::Input::wasKeyReleased(SDL_Keycode key)
{
	return mReleasedKeys[key];
}

bool ascii::Input::isKeyHeld(SDL_Keycode key)
{
	return mHeldKeys[key];
}

bool ascii::Input::anyKeyPressed()
{
    return !allPressedKeys().empty();
}

std::vector<SDL_Keycode> ascii::Input::allPressedKeys()
{
    std::vector<SDL_Keycode> keyList;

    for (auto it = mPressedKeys.begin(); it != mPressedKeys.end(); ++it)
    {
        if (it->second == true)
        {
            keyList.push_back(it->first);
        }
    }

    return keyList;
}

std::vector<SDL_Keycode> ascii::Input::allHeldKeys()
{
    std::vector<SDL_Keycode> keyList;

    for (auto it = mHeldKeys.begin(); it != mHeldKeys.end(); ++it)
    {
        if (it->second == true)
        {
            keyList.push_back(it->first);
        }
    }

    return keyList;
}

int ascii::Input::mouseChangeX()
{
    // The mouse's position hasn't actually changed on the first frame
    if (mFirstFrame) return 0;
    return mMouseX - mLastMouseX;
}

int ascii::Input::mouseChangeY()
{
    // The mouse's position hasn't actually changed on the first frame
    if (mFirstFrame) return 0;
    return mMouseY - mLastMouseY;
}
