#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Surface.h"

namespace ascii
{

	class Graphics
	{
		public:
			Graphics(const char* title);
			~Graphics();

			static const unsigned int kBufferWidth;
			static const unsigned int kBufferHeight;

			void clear();
			void update();
		private:
			SDL_Window* mWindow;
			SDL_Renderer* mRenderer;

			TTF_Font* mFont;
			int mCharWidth, mCharHeight;

			Surface* mBuffer;
	};

};

