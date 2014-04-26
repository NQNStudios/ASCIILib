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

//Color constants
const ascii::Color ascii::Color::Red = ascii::Color(255, 0, 0);
const ascii::Color ascii::Color::DarkRed = ascii::Color(139, 0, 0);
const ascii::Color ascii::Color::Crimson = ascii::Color(220, 20, 60);
const ascii::Color ascii::Color::Maroon = ascii::Color(128, 0, 0);

const ascii::Color ascii::Color::Green = ascii::Color(0, 255, 0);
const ascii::Color ascii::Color::ForestGreen = ascii::Color(34, 139, 34);
const ascii::Color ascii::Color::DarkGreen = ascii::Color(0, 100, 0);
const ascii::Color ascii::Color::DarkOliveGreen = ascii::Color(85, 107, 47);

const ascii::Color ascii::Color::Blue = ascii::Color(0, 0, 255);
const ascii::Color ascii::Color::DarkBlue = ascii::Color(0, 0, 139);
const ascii::Color ascii::Color::BlueViolet = ascii::Color(138, 43, 226);
const ascii::Color ascii::Color::MidnightBlue = ascii::Color(25, 25, 112);

const ascii::Color ascii::Color::Yellow = ascii::Color(255, 255, 0);
const ascii::Color ascii::Color::Gold = ascii::Color(255, 215, 0);
const ascii::Color ascii::Color::GoldenRod = ascii::Color(218, 165, 32);

const ascii::Color ascii::Color::White = ascii::Color(255, 255, 255);

const ascii::Color ascii::Color::Gray = ascii::Color(128, 128, 128);
const ascii::Color ascii::Color::DimGray = ascii::Color(105, 105, 105);
const ascii::Color ascii::Color::DarkGray = ascii::Color(85, 85, 85);
const ascii::Color ascii::Color::LightGray = ascii::Color(211, 211, 211);
const ascii::Color ascii::Color::LightSlateGray = ascii::Color(119, 136, 153);

const ascii::Color ascii::Color::Black = ascii::Color(0, 0, 0);

ascii::Color::operator SDL_Color()
{
	SDL_Color color;

	color.a = kMaxColorValue;
	color.r = r;
	color.g = g;
	color.b = b;

	return color;
}

Uint32 ascii::Color::ToUint32(const SDL_PixelFormat* format)
{
	return SDL_MapRGB(format, r, g, b);
}