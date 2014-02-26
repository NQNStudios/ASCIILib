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

void ascii::Surface::blit(Surface* surface, Rectangle source, int x, int y)
{
	if (source == Rectangle::Empty) //If NULL is passed as source, blit the entire surface
	{
		source.x = 0;
		source.y = 0;
		source.width = surface->width();
		source.height = surface->height();
	}

	for (int destx = x, srcx = source.x; destx < mWidth && srcx < source.right(); ++destx, ++srcx)
	{
		for (int desty = y, srcy = source.y; desty < mHeight && srcy < source.top(); ++desty, ++srcy)
		{
			mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
			mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
			mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
		}
	}
}