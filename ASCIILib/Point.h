#pragma once

namespace ascii
{

	///<summary>
	/// Represents a 2D point with integer coordinates.
	///</summary>
	struct Point
	{
		public:
			///<summary>
			/// Creates a point with the given coordinates.
			///</summary>
			Point(int x, int y)
				: x(x), y(y), defined(true)
			{
			}

			///<summary>
			/// Creates a point representing the origin, defined or undefined
			///</summary>
            Point(bool defined)
                : x(0), y(0), defined(defined)
            {
            }

			///<summary>
			/// Creates a point representing the origin.
			///</summary>
			Point()
				: x(0), y(0), defined(true)
			{
			}

			///<summary>
			/// The origin point: (0, 0).
			///</summary>
			static const Point Origin;
            static const Point Undefined;

			int x, y;
            bool defined;
	};

	inline bool operator==(const Point& a, const Point& b)
	{
		return !(a.defined || b.defined) || (a.x == b.x && a.y == b.y);
	}

	inline bool operator<(const Point& a, const Point& b)
	{
		if (a.x < b.x) return true;

		return a.y < b.y;
	}

};
