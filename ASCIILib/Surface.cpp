#include "Surface.h"

ascii::Surface::Surface(int width, int height)
	: mWidth(width), mHeight(height), 
		mCharacters(width, std::vector<char>(height, ' ')),
		mBackgroundColors(width, std::vector<Color>(height, Color::Black)),
		mCharacterColors(width, std::vector<Color>(height, Color::White))
{

}

void ascii::Surface::clear()
{
	for (int x = 0; x < mWidth; ++x)
	{
		for (int y = 0; y < mHeight; ++y)
		{
			setCharacter(x, y, ' ');
			setBackgroundColor(x, y, Color::Black);
			setCharacterColor(x, y, Color::White);
		}
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

void ascii::Surface::blitString(char* text, Color color, int x, int y)
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