#ifndef GAME_H
#define GAME_H

#include "ImageCache.h"
#include "SoundManager.h"
#include "Input.h"
#include "Graphics.h"

namespace ascii
{

	///<summary>
	/// A generic ASCIILib game
	///</summary>
	class Game
	{
		public:
			///<summary>
			/// Creates an ASCIILib game.
			///</summary>
			///<param name="title">The title of the game's window.</param>
			///<param name="bufferWidth">The width of the game's buffer, in cells.</param>
			///<param name="bufferHeight">The height of the game's buffer, in cells.</param>
			Game(const char* title, const int bufferWidth, const int bufferHeight,
                    int charWidth, int charHeight);

            ~Game();

            void ShowMouseCursor();
            void HideMouseCursor();

			///<summary>
			/// The game's SoundManager.
			///</summary>
			ascii::SoundManager* soundManager() { return mpSoundManager; }

			///<summary>
			/// The game's Graphics.
			///</summary>
			ascii::Graphics* graphics() { return mpGraphics; }

            ///<summary>
            /// Accesses the game's image cache.
            ///</summary>
            ascii::ImageCache* imageCache() { return mpGraphics->imageCache(); }

            ///<summary>
            /// Accesses the game's Input object
            ///</summary>
            ascii::Input* input() { return mpInput; }


			///<summary>
			/// Runs the game's event loop.
			///</summary>
			void Run();

			///<summary>
			/// Stops running the game's event loop and quits the game.
			///</summary>
			void Quit();
        protected:
			virtual void LoadContent(ImageCache* imageCache, SoundManager* soundManager)=0;
			virtual void UnloadContent(ImageCache* imageCache, SoundManager* soundManager)=0;
            virtual void Update(int deltaMS)=0;
            virtual void HandleInput(Input& input)=0;
            virtual void Draw(Graphics& graphics)=0;

            virtual void HandleWindowEvent(SDL_Event event)=0;

		private:
			const char* mWindowTitle;
            const char* mFontpath;
			const int mBufferWidth, mBufferHeight;

			ascii::SoundManager* mpSoundManager;
			ascii::Graphics* mpGraphics;
            ascii::Input* mpInput;

			bool mRunning;
	};

};

#endif
