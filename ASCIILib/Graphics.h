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
#include "PixelFont.h"

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
			Graphics(const char* title, int charWidth, int charHeight,
                    vector<float> scaleOptions, int currentScaleOption,
                    bool fullscreen,
                    int bufferWidth=kBufferWidth, int bufferHeight=kBufferHeight);
			~Graphics();

            // Applies the desired scale option, or the next smallest option
            // which fits on the current display
            void ApplyClosestScaleOption(int option);

            ///<summary>
            /// Modifies the graphics video mode, even after construction.
            /// Warning: This rebuilds the image cache. All images will need
            /// to be reloaded.
            ///</summary>
            void ApplyOptions();

            void AddFont(string key, int size, string fontPath, string fontLayoutPath);
            void SetDefaultFont(string key);
            void UnloadFont(string key, int size);
            void UnloadAllFonts();

            ///<summary>
            /// Cleans up everything created by Graphics, allowing a new
            /// call to Initialize()
            ///</summary>
            void Dispose();

            ///<summary>
            /// Sets the window to fullscreen or not
            ///</summary>
            void SetFullscreen(bool fullscreen);

            vector<float> GetScaleOptions() { return mScaleOptions; }

            int pixelWindowWidth() { return width() * mCharWidth * mScale; }
            int pixelWindowHeight() { return height() * mCharHeight * mScale; }

            ///<summary>
            /// Toggles fullscreen rendering (but does not cause scaling to
            /// fit). Modifies the graphics video mode, even after
            /// construction.
            ///</summary>
            void ToggleFullscreen();

			///<summary>
			/// Returns the image cache for this Graphics instance.
			///</summary>
			ImageCache* imageCache() { return mpCache; }

			int charWidth() { return mCharWidth; }
			int charHeight() { return mCharHeight; }

            int pixelToCellX(int pixelX);
            int pixelToCellY(int pixelY);

            int cellToPixelX(int cellX);
            int cellToPixelY(int cellY);
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

            void clearCellFonts();
            void setCellFont(Rectangle cells, string font);

            // Get the native resolution of the display the game window is
            // currently shown on
            void getCurrentDisplayResolution(int* outWidth, int* outHeight);

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

			SDL_Window* mpWindow;
			SDL_Renderer* mpRenderer;
			ImageCache* mpCache;

			int mCharWidth, mCharHeight;

            const char* mTitle;
            bool mFullscreen;
			Color mBackgroundColor;

			map<string, Image> mBackgroundImages;
			map<string, Image> mForegroundImages;
            vector<ForegroundSurface> mForegroundSurfaces;
            vector<vector<string> > mCellFonts;
            bool mHidingImages;

            map<string, PixelFont*> mFonts;
            PixelFont* GetFont(string key);
            string mDefaultFont;

            float mScale;

            vector<float> mScaleOptions;
            int mCurrentScaleOption;

            int mLastDisplayIndex;
	};

};

#endif
