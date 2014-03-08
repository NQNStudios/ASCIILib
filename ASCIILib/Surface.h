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

			///<summary>
			/// Constructs a surface of the given dimensions filled with the given character and background color.
			///</summary>
			Surface(int width, int height, char character, Color backgroundColor, Color characterColor);

			///<summary>
			/// Constructs a 1x1 surface with the given character and background color.
			///</summary>
			Surface(char character, Color backgroundColor, Color characterColor);

			///<summary>
			/// Loads a surface from a text file.
			///</summary>
			static Surface* FromFile(char* filepath);

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
			/// Fills the entire surface with the given character and background color.
			///</summary>
			void fill(char character, Color backgroundColor, Color characterColor);

			///<summary>
			/// Fills a rectangle on this surface with the given character and background color.
			///</summary>
			void fillRect(Rectangle destination, char character, Color backgroundColor, Color characterColor);

			///<summary>
			/// Draws a border around the entire surface using the given character and background color.
			///</summary>
			void drawBorder(char character, Color backgroundColor, Color characterColor);

			///<summary>
			/// Draws an empty rectangle on this surface with the given character and background color.
			///</summary>
			void drawRect(Rectangle destination, char character, Color backgroundColor, Color characterColor);

			///<summary>
			/// Blits an entire surface to this surface at the given location.
			///</summary>
			///<param name="surface">The surface to blit to this one.</param>
			void blitSurface(Surface* surface, int x, int y);

			///<summary>
			/// Blits part of another surface to this surface at the given location.
			///</summary>
			///<param name="surface">The surface to blit to this one.</param>
			///<param name="source">The source rectangle from which to blit.</param>
			void blitSurface(Surface* surface, Rectangle source, int x, int y);

			///<summary>
			/// Blits a string to this surface.
			///</summary>
			///<param name="text">The string to blit to this surface.</param>
			///<param name="color">The color of the text.</param>
			void blitString(const char* text, Color color, int x, int y);

			///<summary>
			/// Blits a large string to this surface, wrapping it across multiple lines to fit the given destination rectangle.
			///</summary>
			void blitStringMultiline(const char* text, Color color, Rectangle destination);

			///<summary>
			/// Measures the final x position of the text drawn across multiple lines.
			///</summary>
			int measureStringMultilineX(const char* text, Rectangle destination);

			///<summary>
			/// Measures the number of lines needed to blit the given string across multiple lines.
			///</summary>
			int measureStringMultilineY(const char* text, Rectangle destination);
		private:
			int mWidth, mHeight;

			std::vector<std::vector<char> > mCharacters;

			std::vector<std::vector<Color> > mBackgroundColors;
			std::vector<std::vector<Color> > mCharacterColors;
	};

}
