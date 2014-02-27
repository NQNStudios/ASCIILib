#include "Input.h"


void ascii::Input::beginNewFrame()
{
	pressed_keys_.clear();
	released_keys_.clear();
}

void ascii::Input::keyDownEvent(const SDL_Event& event)
{
	pressed_keys_[event.key.keysym.sym] = true;
	held_keys_[event.key.keysym.sym] = true;
}

void ascii::Input::keyUpEvent(const SDL_Event& event)
{
	released_keys_[event.key.keysym.sym] = true;
	held_keys_[event.key.keysym.sym] = false;
}

bool ascii::Input::wasKeyPressed(SDL_Keycode key)
{
	return pressed_keys_[key];
}

bool ascii::Input::wasKeyReleased(SDL_Keycode key)
{
	return released_keys_[key];
}

bool ascii::Input::isKeyHeld(SDL_Keycode key)
{
	return held_keys_[key];
}