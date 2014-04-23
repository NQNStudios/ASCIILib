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
			/// 
			///</summary>
			///<param name="title">The title of the game window.</param>
			///<param name="bufferWidth">The width of the game window's buffer.</param>
			///<param name="bufferHeight">The height of the game window's buffer.</param>
			Graphics(const char* title, int bufferWidth, int bufferHeight);

			~Graphics();

			int charWidth() { return mCharWidth; }
			int charHeight() { return mCharHeight; }

			///<summary>
			/// Loads a bitmap image and ensures that its dimensions will align with the "console" buffer.
			///</summary>
			SDL_Texture* loadTexture(const char* texturePath);

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

