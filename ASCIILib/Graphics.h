#ifndef GRAPHICS_H
#define GRAPHICS_H

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
			Graphics(const char* title, const char* fontpath);

			///<summary>
			/// Creates a game window and sets up the Graphics instance.
			///</summary>
			///<param name="title">The title of the game window.</param>
			///<param name="bufferWidth">The width of the game window's buffer.</param>
			///<param name="bufferHeight">The height of the game window's buffer.</param>
			Graphics(const char* title, const char* fontpath, int bufferWidth, int bufferHeight);
			~Graphics();

            ///<summary>
            /// Modifies the graphics video mode, even after construction.
            /// Warning: This rebuilds the image cache. All images will need
            /// to be reloaded.
            ///</summary>
            void SetVideoMode(float scale, bool fullscreen);

			///<summary>
			/// Returns the image cache for this Graphics instance.
			///</summary>
			ImageCache* imageCache() { return mCache; }

			int charWidth() { return mCharWidth; }
			int charHeight() { return mCharHeight; }

            int pixelToCellX(int pixelX);
            int pixelToCellY(int pixelY);

			///<summary>
			/// Renders the rendering buffer in its current state.
			///</summary>
			void update();

			void setWindowBackgroundColor(Color color) { mBackgroundColor = color; }

			///<summary>
			/// Adds an image in the background of the game window.
			///</summary>
			///<param name="key">The key with which this image should be stored.</param>
			///<param name="textureKey">The key with which this image has been stored in the image cache.</param>
			void addBackgroundImage(std::string key, std::string textureKey, int x, int y);

			///<summary>
			/// Removes an image from the background of the game window.
			///</summary>
			///<param name="key">The key with which this image was stored.</param>
			void removeBackgroundImage(std::string key);

			///<summary>
			/// Adds an image in the foreground of the game window.
			///</summary>
			///<param name="key">The key with which this image should be stored.</param>
			///<param name="textureKey">The key with which this image has been stored in the image cache.</param>
			void addForegroundImage(std::string key, std::string textureKey, int x, int y);

			///<summary>
			/// Removes an image from the foreground of the game window.
			///</summary>
			///<param name="key">The key with which this image was stored.</param>
			void removeForegroundImage(std::string key);

			///<summary>
			/// Clears all images from the window.
			///</summary>
			void clearImages();

			///<summary>
			/// Clears all glyph textures stored in Graphics.
			///</summary>
			void clearGlyphs();
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

			std::map<Glyph, SDL_Texture*> mGlyphTextures;

            const char* mTitle;
            float mScale;
            bool mFullscreen;
			Color mBackgroundColor;

			std::map<std::string, Image> mBackgroundImages;
			std::map<std::string, Image> mForegroundImages;
	};

};

#endif
