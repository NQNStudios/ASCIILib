#include "Surface.h"


ascii::Surface::Surface(int width, int height)
	: mWidth(width), mHeight(height), 
		mCharacters(width, std::vector<char>(height, ' ')),
		mBackgroundColors(width, std::vector<Color>(height, Color::Black)),
		mCharacterColors(width, std::vector<Color>(height, Color::White))
{

}
