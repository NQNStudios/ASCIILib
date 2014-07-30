#pragma once

#include "ImageCache.h"
#include "SoundManager.h"
#include "Input.h"
#include "Graphics.h"

namespace ascii
{

	///<summary>
	/// A generic ASCIILib game whose functioning loop is provided in the constructor as callback functions.
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
			Game(const char* title, const char* fontpath, const int bufferWidth, const int bufferHeight);

			///<summary>
			/// Creates an ASCIILib game.
			///</summary>
			///<param name="title">The title of the game's window.</param>
			Game(const char* title, const char* fontpath);

			///<summary>
			/// The game's SoundManager.
			///</summary>
			ascii::SoundManager* soundManager() { return mSoundManager; }

			///<summary>
			/// The game's ImageCache.
			///</summary>
			ascii::ImageCache* imageCache() { return mCache; }

			///<summary>
			/// The game's Graphics.
			///</summary>
			ascii::Graphics* graphics() { return mGraphics; }

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
            virtual void Update(Game* game, int deltaMS)=0;
            virtual void HandleInput(Game* game, Input& input)=0;
            virtual void Draw(Graphics& graphics)=0;
		private:
			const char* mWindowTitle;
            const char* mFontpath;
			const int mBufferWidth, mBufferHeight;

			ascii::ImageCache* mCache;
			ascii::SoundManager* mSoundManager;
			ascii::Graphics* mGraphics;

			bool mRunning;
	};

};
