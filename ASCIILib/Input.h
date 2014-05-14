#pragma once

#include <map>
#include <SDL.h>

namespace ascii
{
	///<summary>
	/// Mouse button values.
	///</summary>
	enum MouseButton
	{
		LEFT = 1,
		MIDDLE = 2,
		RIGHT = 3
	};

	///<summary>
	/// Manages the game's input and provides useful methods for checking the input state.
	///</summary>
	class Input
	{
		public:
			Input()
				: mMouseX(0), mMouseY(0), mLastMouseX(0), mLastMouseY(0), mMouseState(0), mLastMouseState(0), mScrollX(0), mScrollY(0)
			{
			}

			///<summary>
			/// Checks whether a key was pressed this frame.
			///</summary>
			///<param name="key">SDL_Keycode of the key to check.</param>
			bool wasKeyPressed(SDL_Keycode key);

			///<summary>
			/// Checks whether a key was released this frame.
			///</summary>
			///<param name="key">SDL_Keycode of the key to check.</param>
			bool wasKeyReleased(SDL_Keycode key);

			///<summary>
			/// Checks whether a key is held this frame.
			///</summary>
			///<param name="key">SDL_Keycode of the key to check.</param>
			bool isKeyHeld(SDL_Keycode key);

			int mouseX() { return mMouseX; }
			int mouseY() { return mMouseY; }
			int mouseChangeX() { return mMouseX - mLastMouseX; }
			int mouseChangeY() { return mMouseY - mLastMouseY; }

			bool mouseButtonDown(MouseButton button) { return mMouseState & SDL_BUTTON(button); }
			bool mouseButtonUp(MouseButton button) { return !(mMouseState & SDL_BUTTON(button)); }

			bool mouseButtonClicked(MouseButton button) { return mMouseState & SDL_BUTTON(button) && !(mLastMouseState & SDL_BUTTON(button)); }
			bool mouseButtonReleased(MouseButton button) { return !(mMouseState & SDL_BUTTON(button)) && (mLastMouseState & SDL_BUTTON(button)); }

			int scrollX() { return mScrollX; }
			int scrollY() { return mScrollY; }
		private:
			friend class Game;

			void beginNewFrame();

			void keyDownEvent(const SDL_Event& event);

			void keyUpEvent(const SDL_Event& event);

			void scrollEvent(const SDL_Event& event);

			std::map<SDL_Keycode, bool> mHeldKeys;
			std::map<SDL_Keycode, bool> mPressedKeys;
			std::map<SDL_Keycode, bool> mReleasedKeys;

			int mMouseX, mMouseY;
			Uint32 mMouseState;

			int mLastMouseX, mLastMouseY;
			Uint32 mLastMouseState;

			int mScrollX, mScrollY;
	};

};