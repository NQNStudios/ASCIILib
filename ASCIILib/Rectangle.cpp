#include "Rectangle.h"

//static
const ascii::Rectangle ascii::Rectangle::Empty = ascii::Rectangle(0, 0, 0, 0);

ascii::Rectangle::Rectangle(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height)
{
}

ascii::Rectangle::Rectangle()
    : x(0), y(0), width(0), height(0)
{
}

bool ascii::Rectangle::isEmpty()
{
    return x == 0 && y == 0 && width == 0 && height == 0;
}

ascii::Rectangle::operator SDL_Rect()
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	return rect;
}

bool ascii::Rectangle::contains(Point point)
{
    return point.x >= x && point.y >= y && point.x < right() && point.y < bottom();
}

void ascii::Rectangle::operator=(const Rectangle& other)
{
    this->x = other.x;
    this->y = other.y;
    this->width = other.width;
    this->height = other.height;
}
