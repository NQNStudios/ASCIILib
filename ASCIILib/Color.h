#ifndef COLOR_H
#define COLOR_H

#include <SDL.h>

namespace ascii
{

	///<summary>
	/// Contains the RGB values of a color.
	///</summary>
	struct Color
	{
		public:
			/// <summary>
			/// Constructs a color from RGB values. Values must be between 0 and 255, inclusive.
			/// </summary>
			Color(int r, int g, int b);

			/// <summary>
			/// Constructs a color from float RGB values. Values must be between 0 and 1, inclusive.
			/// </summary>
			Color(float r, float g, float b);

			/// <summary>
			/// Constructs a default color (black).
			/// </summary>
			Color();

        private:
            // Create a "none" color
            Color(void* ptr);

        public:
			/// <summary>
			/// Maps this Color to an SDL Uint32 using the given pixel format.
			/// </summary>
			Uint32 ToUint32(const SDL_PixelFormat* format);

			/// <summary>
			/// The alpha value of all colors in ASCIILib graphics (255).
			/// </summary>
			static const Uint8 kAlpha;

			//Color constants

            // NOTE: DON'T USE THIS ONE EXCEPT IN A SITUATION WHERE YOU
            // EXPLICITLY KNOW ITS USAGE IS VALID
            static const Color None;

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
			static const Color LightGray;
			static const Color LightSlateGray;

			static const Color Black;

			operator SDL_Color();

			Uint8 r, g, b;
            bool isNone;
	};

	inline bool operator==(const Color& c1, const Color& c2)
	{
		return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
	}

	inline bool operator<(const Color& c1, const Color& c2)
	{
		if (c1.r != c2.r)
		{
			return c1.r < c2.r;
		}

		if (c1.g != c2.g)
		{
			return c1.g < c2.g;
		}

		if (c1.b != c2.b)
		{
			return c1.b < c2.b;
		}

		return false;
	}

};

#endif
