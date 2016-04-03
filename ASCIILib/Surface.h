#pragma once

#include <vector>
#include <fstream>
#include <string>
using namespace std;

#include <SDL.h>

#include "Color.h"
#include "Rectangle.h"
#include "Point.h"
#include "ImageCache.h"

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
			static Surface* FromFile(const char* filepath);

			int width() { return mWidth; }
			int height() { return mHeight; }

			char getCharacter(int x, int y) { return mCharacters[x][y]; }
			Color getBackgroundColor(int x, int y) { return mBackgroundColors[x][y]; }
			Color getCharacterColor(int x, int y) { return mCharacterColors[x][y]; }
			bool isCellOpaque(int x, int y) { return mCellOpacity[x][y]; }
			string getSpecialInfo(int x, int y) { return mSpecialInfo[x][y]; }

			void setCharacter(int x, int y, char value) { mCharacters[x][y] = value; }
			void setBackgroundColor(int x, int y, Color value) { mBackgroundColors[x][y] = value; }
			void setCharacterColor(int x, int y, Color value) { mCharacterColors[x][y] = value; }
			void setCellOpacity(int x, int y, bool value) { mCellOpacity[x][y] = value; }
			void setSpecialInfo(int x, int y, string value) { mSpecialInfo[x][y] = value; }

			///<summary>
			/// Clears the surface of all characters and non-black colors.
			///</summary>
			void clear();

			///<summary>
			/// Clears the surface of all cell opacity.
			///</summary>
			void clearTransparent();

			///<summary>
			/// Clears the surface of all cell transparency.
			///</summary>
			void clearOpaque();

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
			/// Copies all information from a given surface to this one, including transparency.
			///</summary>
			///<param name="surface">The surface to copy to this one.</param>
			void copySurface(Surface* surface, int x, int y);

			///<summary>
			/// Copies all information from a given surface to this one, including transparency.
			///</summary>
			///<param name="surface">The surface to copy to this one.</param>
			///<param name="source">The source rectangle from which to copy.</param>
			void copySurface(Surface* surface, Rectangle source, int x, int y);
			
			///<summary>
			/// Blits an entire surface to this surface at the given location.
			///</summary>
			///<param name="surface">The surface to blit to this one.</param>
			void blitSurface(Surface* surface, int x, int y);

            ///<summary>
            /// Set every cell opaque in this surface on which the given surface's
            /// opaque cells would lay when blitted
            ///</summary
            void applyMask(Surface* surface, int x, int y);

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
			/// Finds the final x position of the text drawn across multiple lines.
			///</summary>
			int stringMultilineEndX(const char* text, Rectangle destination);

			///<summary>
			/// Measures the number of lines needed to blit the given string across multiple lines.
			///</summary>
			int measureStringMultilineY(const char* text, Rectangle destination);

            ///<summary>
            /// Locate the first appearance of the given string on one line of
            /// the graphics buffer
            ///</summary>
            ascii::Point findString(string text);

            ///<summary>
            /// Highlight the given string on one line of the Surface using the
            /// given color
            ///</summary>
            void highlightString(string text, ascii::Color color);

            ///<summary>
            /// Highlight the tokens of the given string on whichever lines
            /// where they first appear given color
            ///</summary>
            void highlightTokens(string text, ascii::Color color);

            ///<summary>
            /// Prints out the characters stored in the graphics buffer to
            /// the debug console.
            ///</summary>
            void printContents();
		private:
            // HELPERS
            static void readLine(ifstream* file, string& str);

            // FIELDS
			int mWidth, mHeight;

			vector<vector<char> > mCharacters;

			vector<vector<Color> > mBackgroundColors;
			vector<vector<Color> > mCharacterColors;

			vector<bool*> mCellOpacity;

			vector<vector<string> > mSpecialInfo;
	};

}
