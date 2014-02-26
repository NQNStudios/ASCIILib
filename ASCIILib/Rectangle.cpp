#include "Rectangle.h"

//static
const ascii::Rectangle ascii::Rectangle::Empty = ascii::Rectangle(0, 0, 0, 0);

ascii::Rectangle::operator SDL_Rect()
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	return rect;
}
