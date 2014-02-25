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