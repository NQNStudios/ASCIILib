#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Surface.h"
#include "Rectangle.h"

namespace ascii
{

	class Graphics
	{
		public:
			Graphics(const char* title);
			~Graphics();

			static const unsigned int kBufferWidth;
			static const unsigned int kBufferHeight;

			///<summary>
			/// Clears the rendering buffer completely.
			///</summary>
			void clear();

			///<summary>
			/// Renders the rendering buffer in its current state.
			///</summary>
			void update();

			///<summary>
			/// Blits a surface to the rendering buffer at the given location.
			///</summary>
			void blitSurface(Surface* surface, int x, int y);

			///<summary>
			/// Blits part of a surface to the rendering buffer at the given location.
			///</summary>
			///<param name="source">The part of the surface to blit.</param>
			void blitSurface(Surface* surface, Rectangle source, int x, int y);
		private:
			SDL_Window* mWindow;
			SDL_Renderer* mRenderer;

			TTF_Font* mFont;
			int mCharWidth, mCharHeight;

			Surface* mBuffer;
	};

};

