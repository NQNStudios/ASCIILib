#include "Color.h"

const int kMaxColorValue = 255;
Color::Color(int r, int g, int b)
	: r(r), g(g), b(b)
{
}

Color::Color(float r, float g, float b)
	: r(r * kMaxColorValue), g(g * kMaxColorValue), b(b * kMaxColorValue)
{
}

Color::operator SDL_Color()
{
	SDL_Color color;

	color.a = kMaxColorValue;
	color.r = r;
	color.g = g;
	color.b = b;

	return color;
}