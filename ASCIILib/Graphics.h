#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include <string>
using namespace std;

#include <SDL.h>
#include <SDL_ttf.h>

#include "Surface.h"
#include "Rectangle.h"
#include "Point.h"

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
			Graphics(const char* title, string fontpath, int bufferWidth=kBufferWidth, int bufferHeight=kBufferHeight);
			~Graphics();

            ///<summary>
            /// Modifies the graphics video mode, even after construction.
            /// Warning: This rebuilds the image cache. All images will need
            /// to be reloaded.
            ///</summary>
            void Initialize();

            ///<summary>
            /// Cleans up everything created by Graphics, allowing a new
            /// call to Initialize()
            ///</summary>
            void Dispose();

            ///<summary>
            /// Sets the window to fullscreen or not
            ///</summary>
            void SetFullscreen(bool fullscreen);

            ///<summary>
            /// Toggles fullscreen rendering (but does not cause scaling to
            /// fit). Modifies the graphics video mode, even after
            /// construction.
            ///</summary>
            void ToggleFullscreen();

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
			/// Draw a surface in the VERY foreground of the screen
			///</summary>
            void drawForegroundSurface(Surface* surface, int x, int y);

			///<summary>
			/// Adds an image in the background of the game window.
			///</summary>
			///<param name="key">The key with which this image should be stored.</param>
			///<param name="textureKey">The key with which this image has been stored in the image cache.</param>
			void addBackgroundImage(string key, string textureKey, int x, int y);

			///<summary>
			/// Removes an image from the background of the game window.
			///</summary>
			///<param name="key">The key with which this image was stored.</param>
			void removeBackgroundImage(string key);

			///<summary>
			/// Adds an image in the foreground of the game window.
			///</summary>
			///<param name="key">The key with which this image should be stored.</param>
			///<param name="textureKey">The key with which this image has been stored in the image cache.</param>
			void addForegroundImage(string key, string textureKey, int x, int y);

			///<summary>
			/// Removes an image from the foreground of the game window.
			///</summary>
			///<param name="key">The key with which this image was stored.</param>
			void removeForegroundImage(string key);

			///<summary>
			/// Clears all images from the window.
			///</summary>
			void clearImages();

            ///<summary>
            /// Temporarily hides images
            ///</summary>
            void hideImages();

            ///<summary>
            /// Shows all images after hiding them
            ///</summary>
            void showImages();

			///<summary>
			/// Clears all glyph textures stored in Graphics.
			///</summary>
			void clearGlyphs();

            Point drawOrigin();
            Point drawResolution();
            Point actualResolution();

		private:
			typedef pair<string, Color> Glyph;
			typedef pair<SDL_Texture*, Point> Image;
            typedef pair<Surface*, Point> ForegroundSurface;

            void clearScreen();
            void drawImages(map<string, Image>* images);
            void drawBackgroundColors(Surface* surface, int x, int y);
            void drawCharacters(Surface* surface, int x, int y);
            void drawSurface(Surface* surface, int x, int y);
            void refresh();

			///<summary>
			/// Ensures that this Graphics instance was not created with dimensions too small to fit
			///</summary>
			void checkSize();

            ///<summary>
            /// Update the values of mCharWidth and mCharHeight to reflect
            /// scale
            ///</summary>
            void UpdateCharSize();

			SDL_Window* mWindow;
			SDL_Renderer* mRenderer;
			ImageCache* mCache;

			TTF_Font* mFont;
			int mCharWidth, mCharHeight;

			map<Glyph, SDL_Texture*> mGlyphTextures;

            const char* mTitle;
            bool mFullscreen;
			Color mBackgroundColor;

			map<string, Image> mBackgroundImages;
			map<string, Image> mForegroundImages;
            vector<ForegroundSurface> mForegroundSurfaces;

            bool mHidingImages;

            int mCharHeightCorrection;
	};

};

#endif
