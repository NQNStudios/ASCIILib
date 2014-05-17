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
			///<param name="loadContent">The LoadContent function of the game's functioning loop.</param>
			///<param name="update">The Update function of the game's functioning loop.</param>
			///<param name="handleInput">The HandleInput function of the game's functioning loop.</param>
			///<param name="draw">The Draw function of the game's functioning loop.</param>
			Game(const char* title, const int bufferWidth, const int bufferHeight, 
				void (*loadContent)(ImageCache*, SoundManager*), 
				void (*update)(Game*, int), 
				void (*handleInput)(Game*, Input&), 
				void (*draw)(Graphics&));

			///<summary>
			/// Creates an ASCIILib game.
			///</summary>
			///<param name="title">The title of the game's window.</param>
			///<param name="loadContent">The LoadContent function of the game's functioning loop.</param>
			///<param name="update">The Update function of the game's functioning loop.</param>
			///<param name="handleInput">The HandleInput function of the game's functioning loop.</param>
			///<param name="draw">The Draw function of the game's functioning loop.</param>
			Game(const char* title, 
				void (*loadContent)(ImageCache*, SoundManager*), 
				void (*update)(Game*, int),
				void (*handleInput)(Game*, Input&), 
				void (*draw)(Graphics&));

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
		private:
			void (*mLoadContent)(ImageCache*, SoundManager*);
			void (*mUpdate)(Game*, int);
			void (*mHandleInput)(Game*, Input&);
			void (*mDraw)(Graphics&);

			const char* mWindowTitle;
			const int mBufferWidth, mBufferHeight;

			ascii::ImageCache* mCache;
			ascii::SoundManager* mSoundManager;
			ascii::Graphics* mGraphics;

			bool mRunning;
	};

};