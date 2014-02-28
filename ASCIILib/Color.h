#pragma once

#include <SDL.h>

namespace ascii
{

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

			///<summary>
			///Constructs a default color (black).
			///</summary>
			Color();

			///<summary>
			/// The alpha value of all colors in ASCIILib graphics (255).
			///</summary>
			static const Uint8 kAlpha;

			//Color constants
			static const Color Red;
			static const Color DarkRed;
			static const Color Crimson;
			static const Color Maroon;

			static const Color Green;
			static const Color ForestGreen;
			static const Color DarkGreen;
			static const Color DarkOliveGreen;
			
			static const Color Blue;
			static const Color DarkBlue;
			static const Color BlueViolet;
			static const Color MidnightBlue;

			static const Color Yellow;
			static const Color Gold;
			static const Color GoldenRod;

			static const Color White;

			static const Color Gray;
			static const Color DarkGray;
			static const Color DimGray;
			static const Color LightSlateGray;

			static const Color Black;

			operator SDL_Color();
			
			Uint8 r, g, b;
	};

	inline bool operator==(const Color& c1, const Color& c2)
	{
		return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
	}

};