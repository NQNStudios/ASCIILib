#include "Surface.h"

#include <sstream>

ascii::Surface::Surface(int width, int height)
	: mWidth(width), mHeight(height), 
		mCharacters(width, std::vector<char>(height, ' ')),
		mBackgroundColors(width, std::vector<Color>(height, Color::Black)),
		mCharacterColors(width, std::vector<Color>(height, Color::White))
{

}

ascii::Surface::Surface(int width, int height, char character, Color backgroundColor, Color characterColor)
	: mWidth(width), mHeight(height),
		mCharacters(width, std::vector<char>(height, character)),
		mBackgroundColors(width, std::vector<Color>(height, backgroundColor)),
		mCharacterColors(width, std::vector<Color>(height, characterColor))
{

}

ascii::Surface::Surface(char character, Color backgroundColor, Color characterColor)
	: mWidth(1), mHeight(1),
		mCharacters(1, std::vector<char>(1, character)),
		mBackgroundColors(1, std::vector<Color>(1, backgroundColor)),
		mCharacterColors(1, std::vector<Color>(1, characterColor))
{

}

void ascii::Surface::clear()
{
	fill(' ', Color::Black, Color::White);
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

void ascii::Surface::blitSurface(Surface* surface, int x, int y)
{
	for (int destx = x, srcx = 0; destx < mWidth && srcx < surface->mWidth; ++destx, ++srcx)
	{
		for (int desty = y, srcy = 0; desty < mWidth && srcy < surface->mHeight; ++desty, ++srcy)
		{
			mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
			mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
			mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
		}
	}
}

void ascii::Surface::blitSurface(Surface* surface, Rectangle source, int x, int y)
{
	for (int destx = x, srcx = source.x; destx < mWidth && srcx < source.right(); ++destx, ++srcx)
	{
		for (int desty = y, srcy = source.y; desty < mHeight && srcy < source.bottom(); ++desty, ++srcy)
		{
			mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
			mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
			mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
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

		x += tempstr.size() + 1;
	}
}