#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <vector>
#include <SDL.h>

namespace ascii
{
	///<summary>
	/// Mouse button values.
	///</summary>
	enum MouseButton
	{
        MB_BEGIN = 1,
		LEFT = 1,
		MIDDLE = 2,
		RIGHT = 3,
        MB_END = 3
	};

	///<summary>
	/// Manages the game's input and provides useful methods for checking the input state.
	///</summary>
	class Input
	{
		public:
			Input()
				: mMouseState(0), mLastMouseState(0), mMouseX(0), mMouseY(0), mLastMouseX(0), mLastMouseY(0), mScrollX(0), mScrollY(0)
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

			///<summary>
			/// Checks if any key at all was pressed.
			///</summary>
			bool anyKeyPressed();

            ///<summary>
            /// Return the list of keys that were newly pressed this frame
            ///</summary>
            std::vector<SDL_Keycode> allPressedKeys();

			///<summary>The x-coordinate of the mouse.</summary>
			int mouseX() { return mMouseX; }

			///<summary>The y-coordinate of the mouse.</summary>
			int mouseY() { return mMouseY; }

			///<summary>The change in the mouse's x-coordinate since the last frame.</summary>
			int mouseChangeX() { return mMouseX - mLastMouseX; }
			
			///<summary>The change in the mouse's y-coordinate since the last frame.</summary>
			int mouseChangeY() { return mMouseY - mLastMouseY; }

			///<summary>Checks whether a mouse button is pressed.</summary>
			bool mouseButtonDown(MouseButton button) { return mMouseState & SDL_BUTTON(button); }

			///<summary>Checks whether a mouse button is currently released.</summary>
			bool mouseButtonUp(MouseButton button) { return !(mMouseState & SDL_BUTTON(button)); }

			///<summary>Checks whether a mouse button was clicked this frame.</summary>
			bool mouseButtonClicked(MouseButton button) { return mMouseState & SDL_BUTTON(button) && !(mLastMouseState & SDL_BUTTON(button)); }
			
			///<summary>Checks whether a mouse button was released this frame.</summary>
			bool mouseButtonReleased(MouseButton button) { return !(mMouseState & SDL_BUTTON(button)) && (mLastMouseState & SDL_BUTTON(button)); }

			///<summary>The amount the scroll bar was moved horizontally this frame.</summary>
			int scrollX() { return mScrollX; }

			///<summary>The amount the scroll bar was moved vertically this frame.</summary>
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

#endif
