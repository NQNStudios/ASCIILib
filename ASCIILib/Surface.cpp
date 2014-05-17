#include "Surface.h"

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

ascii::Surface::Surface(int width, int height)
	: mWidth(width), mHeight(height), 
		mCharacters(width, std::vector<char>(height, ' ')),
		mBackgroundColors(width, std::vector<Color>(height, Color::Black)),
		mCharacterColors(width, std::vector<Color>(height, Color::White)),
		mSpecialInfo(width, std::vector<std::string>(height, ""))
{
	for (int x = 0; x < width; ++x)
	{
		mCellOpacity.push_back(new bool[height]);
	}
}

ascii::Surface::Surface(int width, int height, char character, Color backgroundColor, Color characterColor)
	: mWidth(width), mHeight(height),
		mCharacters(width, std::vector<char>(height, character)),
		mBackgroundColors(width, std::vector<Color>(height, backgroundColor)),
		mCharacterColors(width, std::vector<Color>(height, characterColor)),
		mSpecialInfo(width, std::vector<std::string>(height, ""))
{
	for (int x = 0; x < width; ++x)
	{
		mCellOpacity.push_back(new bool[height]);
	}
}

ascii::Surface::Surface(char character, Color backgroundColor, Color characterColor)
	: mWidth(1), mHeight(1),
		mCharacters(1, std::vector<char>(1, character)),
		mBackgroundColors(1, std::vector<Color>(1, backgroundColor)),
		mCharacterColors(1, std::vector<Color>(1, characterColor)),
		mSpecialInfo(1, std::vector<std::string>(1, ""))
{
	mCellOpacity.push_back(new bool[1]);
}

ascii::Surface* ascii::Surface::FromFile(const char* filepath)
{
	std::ifstream file;
	file.open(filepath);

	std::map<char, Color> colors;
	std::map<char, std::string> infoCodes;

	std::string str;
	std::stringstream sstream;

	std::getline(file, str); //COLORS
	std::getline(file, str);

	char symbol[1+1];
	char red[3+1];
	char green[3+1];
	char blue[3+1];

	do
	{
		sstream = std::stringstream(str);

		sstream >> symbol;
		sstream >> red;
		sstream >> green;
		sstream >> blue;

		colors[symbol[0]] = Color(atoi(red), atoi(green), atoi(blue));
		
		std::getline(file, str);
	} while (str.compare("INFO CODES")); //do-while loop used because COLORS will never be empty section

	//INFO CODES
	std::getline(file, str);

	char infoCode[1+1];
	std::string infoVal;

	while (str.compare("SIZE")) //while loop used because INFO CODES may be empty section
	{
		sstream = std::stringstream(str);
		
		sstream >> infoCode;
		sstream >> infoVal;

		infoCodes[infoCode[0]] = infoVal;

		std::getline(file, str);
	}

	//SIZE
	std::getline(file, str);
	sstream = std::stringstream(str);
	char width[3+1];
	char height[3+1];
	sstream >> width;
	sstream >> height;

	Surface* surface = new Surface(atoi(width), atoi(height));

	std::getline(file, str); //CHARACTERS
	
	char character = ' ';
	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		std::getline(file, str);

		int c = 0;
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			character = *it;

			surface->setCharacter(c, r, character);

			++c;
		}
	}

	std::getline(file, str); //BACKGROUND COLORS
	
	char colorsymbol = ' ';
	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		std::getline(file, str);

		int c = 0;
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			surface->setBackgroundColor(c, r, colors[colorsymbol]);

			++c;
		}
	}

	std::getline(file, str); //CHARACTER COLORS
	
	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		std::getline(file, str);

		int c = 0;
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			surface->setCharacterColor(c, r, colors[colorsymbol]);

			++c;
		}
	}

	std::getline(file, str); //OPACITY

	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		std::getline(file, str);

		int c = 0;
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			bool opaque = colorsymbol == '1';

			surface->setCellOpacity(c, r, opaque);

			++c;
		}
	}

	std::getline(file, str); //SPECIAL INFO

	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		std::getline(file, str);

		int c = 0;
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			if (colorsymbol != ' ')
			{
				surface->setSpecialInfo(c, r, infoCodes[colorsymbol]);
			}

			++c;
		}
	}

	file.close();

	return surface;
}

void ascii::Surface::clear()
{
	fill(' ', Color::Black, Color::White);
}

void ascii::Surface::clearTransparent()
{
	for (int x = 0; x < mWidth; ++x)
	{
		for (int y = 0; y < mHeight; ++y)
		{
			setCellOpacity(x, y, false);
		}
	}
}

void ascii::Surface::fill(char character, Color backgroundColor, Color characterColor)
{
	for (int x = 0; x < mWidth; ++x)
	{
		for (int y = 0; y < mHeight; ++y)
		{
			setCharacter(x, y, character);
			setBackgroundColor(x, y, backgroundColor);
			setCharacterColor(x, y, characterColor);
		}
	}
}

void ascii::Surface::fillRect(Rectangle destination, char character, Color backgroundColor, Color characterColor)
{
	for (int x = destination.left(); x < destination.right(); ++x)
	{
		for (int y = destination.top(); y < destination.bottom(); ++y)
		{
			setCharacter(x, y, character);
			setBackgroundColor(x, y, backgroundColor);
			setCharacterColor(x, y, characterColor);
		}
	}
}

void ascii::Surface::drawBorder(char character, Color backgroundColor, Color characterColor)
{
	int x1 = 0;
	int x2 = mWidth - 1;

	int y1 = 0;
	int y2 = mHeight - 1;

	//draw the top and bottom
	for (int x = x1; x <= x2; ++x)
	{
		setCharacter(x, y1, character);
		setBackgroundColor(x, y1, backgroundColor);
		setCharacterColor(x, y1, characterColor);

		setCharacter(x, y2, character);
		setBackgroundColor(x, y2, backgroundColor);
		setCharacterColor(x, y2, characterColor);
	}

	//draw the left and right
	for (int y = y1; y <= y2; ++y)
	{
		setCharacter(x1, y, character);
		setBackgroundColor(x1, y, backgroundColor);
		setCharacterColor(x1, y, characterColor);

		setCharacter(x2, y, character);
		setBackgroundColor(x2, y, backgroundColor);
		setCharacterColor(x2, y, characterColor);
	}
}

void ascii::Surface::drawRect(Rectangle destination, char character, Color backgroundColor, Color characterColor)
{
	int x1 = destination.left();
	int x2 = destination.right() - 1;

	int y1 = destination.top();
	int y2 = destination.bottom() - 1;

	//draw the top and bottom
	for (int x = x1; x <= x2; ++x)
	{
		setCharacter(x, y1, character);
		setBackgroundColor(x, y1, backgroundColor);
		setCharacterColor(x, y1, characterColor);

		setCharacter(x, y2, character);
		setBackgroundColor(x, y2, backgroundColor);
		setCharacterColor(x, y2, characterColor);
	}

	//draw the left and right
	for (int y = y1; y <= y2; ++y)
	{
		setCharacter(x1, y, character);
		setBackgroundColor(x1, y, backgroundColor);
		setCharacterColor(x1, y, characterColor);

		setCharacter(x1, y, character);
		setBackgroundColor(x1, y, backgroundColor);
		setCharacterColor(x1, y, characterColor);
	}
}

void ascii::Surface::copySurface(Surface* surface, int x, int y)
{
	//copy cell info from the other surface
	for (int destx = x, srcx = 0; destx < mWidth && srcx < surface->mWidth; ++destx, ++srcx)
	{
		for (int desty = y, srcy = 0; desty < mHeight && srcy < surface->mHeight; ++desty, ++srcy)
		{
			mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
			mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
			mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
			mSpecialInfo[destx][desty] = surface->mSpecialInfo[srcx][srcy];
			mCellOpacity[destx][desty] = surface->mCellOpacity[srcx][srcy];
		}
	}
}

void ascii::Surface::copySurface(Surface* surface, Rectangle source, int x, int y)
{
	//copy cell info from the other surface
	for (int destx = x, srcx = source.x; destx < mWidth && srcx < source.right(); ++destx, ++srcx)
	{
		for (int desty = y, srcy = source.y; desty < mHeight && srcy < source.bottom(); ++desty, ++srcy)
		{
			mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
			mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
			mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
			mSpecialInfo[destx][desty] = surface->mSpecialInfo[srcx][srcy];
			mCellOpacity[destx][desty] = surface->mCellOpacity[srcx][srcy];
		}
	}
}

void ascii::Surface::blitSurface(Surface* surface, int x, int y)
{
	//blit the opaque cells from the other surface
	for (int destx = x, srcx = 0; destx < mWidth && srcx < surface->mWidth; ++destx, ++srcx)
	{
		for (int desty = y, srcy = 0; desty < mHeight && srcy < surface->mHeight; ++desty, ++srcy)
		{
			if (surface->isCellOpaque(srcx, srcy))
			{
				mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
				mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
				mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
				mSpecialInfo[destx][desty] = surface->mSpecialInfo[srcx][srcy];

				mCellOpacity[destx][desty] = true; //Cover transparent cells in the lower surface with opaque ones
			}
		}
	}
}

void ascii::Surface::blitSurface(Surface* surface, Rectangle source, int x, int y)
{
	//blit the opaque cells from the other surface
	for (int destx = x, srcx = source.x; destx < mWidth && srcx < source.right(); ++destx, ++srcx)
	{
		for (int desty = y, srcy = source.y; desty < mHeight && srcy < source.bottom(); ++desty, ++srcy)
		{
			if (surface->isCellOpaque(srcx, srcy))
			{
				mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
				mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
				mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
				mSpecialInfo[destx][desty] = surface->mSpecialInfo[srcx][srcy];

				mCellOpacity[destx][desty] = true; //Cover transparent cells in the lower surface with opaque ones
			}
		}
	}
}

void ascii::Surface::blitString(const char* text, Color color, int x, int y)
{
	std::string str(text);

	int destx = x, desty = y;
	std::string::iterator it = str.begin();
	while (destx < mWidth && desty < mHeight && it != str.end())
	{
		mCharacters[destx][desty] = *it;
		mCharacterColors[destx][desty] = color;

		++it, ++destx;
	}
}

void ascii::Surface::blitStringMultiline(const char* text, Color color, Rectangle destination)
{
	std::stringstream sstream(text);
	std::string tempstr;

	std::vector<std::string> words;

	while (sstream >> tempstr)
	{
		words.push_back(tempstr); //collect the individual words in a vector
	}

	int x = destination.left();
	int y = destination.top();

	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
	{
		//blit each word
		tempstr = *it;
		
		if (x + tempstr.size() > destination.right())
		{
			//if there's not enough room on this line, move to the next one
			++y;
			x = destination.left();

			if (y >= destination.bottom()) break; //make sure not to write on any rows outside of the destination rectangle
		}

		blitString(tempstr.c_str(), color, x, y);
		setCharacter(x + tempstr.size(), y, ' '); //fill in spaces

		x += tempstr.size() + 1;
	}

}

int ascii::Surface::measureStringMultilineX(const char* text, Rectangle destination)
{
	std::stringstream sstream(text);
	std::string tempstr;

	std::vector<std::string> words;

	while (sstream >> tempstr)
	{
		words.push_back(tempstr); //collect the individual words in a vector
	}

	int x = destination.left();
	int y = destination.top();

	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
	{
		tempstr = *it;
		
		if (x + tempstr.size() > destination.right())
		{
			//if there's not enough room on this line, move to the next one
			++y;
			x = destination.left();

			if (y >= destination.bottom()) break; //make sure not to write on any rows outside of the destination rectangle
		}

		x += tempstr.size() + 1;
	}

	return x;
}

int ascii::Surface::measureStringMultilineY(const char* text, Rectangle destination)
{
	std::stringstream sstream(text);
	std::string tempstr;

	std::vector<std::string> words;

	while (sstream >> tempstr)
	{
		words.push_back(tempstr); //collect the individual words in a vector
	}

	int x = destination.left();
	int y = destination.top();

	int lines = 1;

	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
	{
		tempstr = *it;
		
		if (x + tempstr.size() > destination.right())
		{
			//if there's not enough room on this line, move to the next one
			++y;
			++lines;
			x = destination.left();

			if (y >= destination.bottom()) break; //make sure not to write on any rows outside of the destination rectangle
		}

		x += tempstr.size() + 1;
	}

	return lines;
}