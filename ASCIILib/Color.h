#pragma once

#include <SDL.h>

///<summary>
/// Contains the RGB values of a color.
///</summary>
struct Color
{
public:
	///<summary>
	/// Constructs a color from RGB values. Values must be between 0 and 255, inclusive.
	///</summary>
	Color(int r, int g, int b);

	///<summary>
	/// Constructs a color from float RGB values. Values must be between 0 and 1, inclusive.
	///</summary>
	Color(float r, float g, float b);

	operator SDL_Color();
private:
	Uint8 r, g, b;
};

