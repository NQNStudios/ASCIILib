#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL.h>

#include "Point.h"
using namespace ascii;

namespace ascii
{

	///<summary>
	/// Defines an axis-aligned rectangle by its top left corner and dimensions.
	///</summary>
	struct Rectangle
	{
		public:
			///<summary>
			/// Constructs a Rectangle.
			///</summary>
			Rectangle(int x, int y, int width, int height);
	
			///<summary>
			/// Constructs a default rectangle with no dimensions.
			///</summary>
			Rectangle();

            void operator=(const Rectangle& other);

            bool isEmpty();

            bool contains(Point point);

			///<summary>
			/// An empty rectangle.
			///</summary>
			static const Rectangle Empty;

			int top() { return y; }
			int bottom() { return y + height; }
			int left() { return x; }
			int right() { return x + width; }

			operator SDL_Rect();

			int x, y, width, height;
	};

	inline bool operator==(const Rectangle& a, const Rectangle& b)
	{
		return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
	}

};

#endif