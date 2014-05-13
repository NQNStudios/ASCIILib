#pragma once

#include <map>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Surface.h"
#include "Rectangle.h"

namespace ascii
{

	///<summary>
	/// Handles all rendering for an ASCIILib game.
	///</summary>
	class Graphics : public Surface
	{
		public:
			///<summary>
			/// The default buffer width of ASCIILib games, to match a regular console.
			///</summary>
			static const unsigned int kBufferWidth;

			///<summary>
			/// The default buffer height of ASCIILib games, to match a regular console.
			///</summary>
			static const unsigned int kBufferHeight;

			///<summary>
			/// Creates a game window and sets up the Graphics instance.
			///</summary>
			///<param name="title">The title of the game window.</param>
			Graphics(const char* title);

			///<summary>
			/// Creates a game window and sets up the Graphics instance.
			///</summary>
			///<param name="title">The title of the game window.</param>
			///<param name="bufferWidth">The width of the game window's buffer.</param>
			///<param name="bufferHeight">The height of the game window's buffer.</param>
			Graphics(const char* title, int bufferWidth, int bufferHeight);
			~Graphics();

			///<summary>
			/// Returns the image cache for this Graphics instance.
			///</summary>
			ImageCache* imageCache() { return mCache; }

			int charWidth() { return mCharWidth; }
			int charHeight() { return mCharHeight; }

			///<summary>
			/// Renders the rendering buffer in its current state.
			///</summary>
			void update();

			void setBackgroundColor(Color color) { mBackgroundColor = color; }

			///<summary>
			/// Adds an image in the background of the game window.
			///</summary>
			///<param name="key">The key with which this image should be stored.</param>
			///<param name="textureKey">The key with which this image has been stored in the image cache.</param>
			void addBackgroundImage(const char* key, const char* textureKey, int x, int y);

			///<summary>
			/// Removes an image from the background of the game window.
			///</summary>
			///<param name="key">The key with which this image was stored.</param>
			void removeBackgroundImage(const char* key);

			///<summary>
			/// Adds an image in the foreground of the game window.
			///</summary>
			///<param name="key">The key with which this image should be stored.</param>
			///<param name="textureKey">The key with which this image has been stored in the image cache.</param>
			void addForegroundImage(const char* key, const char* textureKey, int x, int y);

			///<summary>
			/// Removes an image from the foreground of the game window.
			///</summary>
			///<param name="key">The key with which this image was stored.</param>
			void removeForegroundImage(const char* key);

			///<summary>
			/// Clears all images from the window.
			///</summary>
			void clearImages();
		private:
			///<summary>
			/// Ensures that this Graphics instance was not created with dimensions too small to fit
			///</summary>
			void checkSize();

			typedef std::pair<std::string, Color> Glyph;
			typedef std::pair<SDL_Texture*, Point> Image;

			SDL_Window* mWindow;
			SDL_Renderer* mRenderer;
			ImageCache* mCache;

			TTF_Font* mFont;
			int mCharWidth, mCharHeight;

			std::map<Glyph, SDL_Texture*> mStringTextures;

			Color mBackgroundColor;

			std::map<const char*, Image> mBackgroundImages;
			std::map<const char*, Image> mForegroundImages;
	};

};

