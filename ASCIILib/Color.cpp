#include "Color.h"

const int kMaxColorValue = 255;

ascii::Color::Color(int r, int g, int b)
	: r(r), g(g), b(b)
{
}

ascii::Color::Color(float r, float g, float b)
	: r(r * kMaxColorValue), g(g * kMaxColorValue), b(b * kMaxColorValue)
{
}

ascii::Color::Color()
	: r(0), g(0), b(0)
{
}

//static
const Uint8 ascii::Color::kAlpha = 255;
const ascii::Color ascii::Color::Black(0, 0, 0);
const ascii::Color ascii::Color::White(255, 255, 255);

ascii::Color::operator SDL_Color()
{
	SDL_Color color;

	color.a = kMaxColorValue;
	color.r = r;
	color.g = g;
	color.b = b;

	return color;
}