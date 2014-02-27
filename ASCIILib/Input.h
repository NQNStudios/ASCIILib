#pragma once

#include <map>
#include <SDL.h>

namespace ascii
{

	///<summary>
	/// Manages the game's input and provides useful methods for checking the input state.
	///</summary>
	class Input
	{
		public:
			///<summary>
			/// Clears old input information that is no longer valid.
			///</summary>
			void beginNewFrame();

			///<summary>
			/// Called when a key is pressed to update the input state.
			///</summary>
			void keyDownEvent(const SDL_Event& event);

			///<summary>
			/// Called when a key is released to update the input state.
			///</summary>
			void keyUpEvent(const SDL_Event& event);

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

		private:
			std::map<SDL_Keycode, bool> held_keys_;
			std::map<SDL_Keycode, bool> pressed_keys_;
			std::map<SDL_Keycode, bool> released_keys_;
	};

};