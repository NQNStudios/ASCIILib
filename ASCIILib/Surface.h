#pragma once

#include <vector>

#include "Color.h"

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

			void clear();
		private:
			int mWidth, mHeight;

			std::vector<std::vector<char> > mCharacters;

			std::vector<std::vector<Color> > mBackgroundColors;
			std::vector<std::vector<Color> > mCharacterColors;
	};

};
