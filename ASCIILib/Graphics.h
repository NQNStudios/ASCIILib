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
			/// Creates a game window and sets up the Graphics instance.
			///</summary>
			///<param name="title">The title of the game window.</param>
			Graphics(const char* title);
			~Graphics();

			static const unsigned int kBufferWidth;
			static const unsigned int kBufferHeight;

			///<summary>
			/// Renders the rendering buffer in its current state.
			///</summary>
			void update();
		private:
			typedef std::pair<std::string, Color> Glyph;

			SDL_Window* mWindow;
			SDL_Renderer* mRenderer;

			TTF_Font* mFont;
			int mCharWidth, mCharHeight;

			std::map<Glyph, SDL_Texture*> mStringTextures;
	};

};

