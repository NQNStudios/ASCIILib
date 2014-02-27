#pragma once

#include <vector>

#include "Color.h"
#include "Rectangle.h"

namespace ascii
{

	///<summary>
	/// A surface of colored console characters with background colors.
	///</summary>
	class Surface
	{
		public:
			///<summary>
			/// Constructs an empty surface of the given dimensions.
			///</summary>
			Surface(int width, int height);

			int width() { return mWidth; }
			int height() { return mHeight; }

			char getCharacter(int x, int y) { return mCharacters[x][y]; }
			Color getBackgroundColor(int x, int y) { return mBackgroundColors[x][y]; }
			Color getCharacterColor(int x, int y) { return mCharacterColors[x][y]; }

			void setCharacter(int x, int y, char value) { mCharacters[x][y] = value; }
			void setBackgroundColor(int x, int y, Color value) { mBackgroundColors[x][y] = value; }
			void setCharacterColor(int x, int y, Color value) { mCharacterColors[x][y] = value; }

			///<summary>
			/// Clears the surface of all characters and non-black colors.
			///</summary>
			void clear();

			///<summary>
			/// Blits an entire surface to this surface at the given location.
			///</summary>
			///<param name="surface">The surface to blit to this one.</param>
			void blitSurface(Surface* surface, int x, int y);

			///<summary>
			/// Blits part of another surface to this surface at the given location.
			///</summary>
			///<param name="surface">The surface to blit to this one.</param>
			///<param name="source">The source rectangle from which to blit, or NULL if the entire source is desired.</param>
			void blitSurface(Surface* surface, Rectangle source, int x, int y);

			///<summary>
			/// Blits a string to this surface.
			///</summary>
			///<param name="text">The string to blit to this surface.</param>
			///<param name="color">The color of the text.</param>
			void blitString(char* text, Color color, int x, int y);
		private:
			int mWidth, mHeight;

			std::vector<std::vector<char> > mCharacters;

			std::vector<std::vector<Color> > mBackgroundColors;
			std::vector<std::vector<Color> > mCharacterColors;
	};

}
