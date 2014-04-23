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
				: x(x), y(y) 
			{
			}

			///<summary>
			/// Creates a point representing the origin.
			///</summary>
			Point()
				: x(0), y(0)
			{
			}

			///<summary>
			/// The origin point: (0, 0).
			///</summary>
			static const Point Origin;

			int x, y;
	};

	inline bool operator==(const Point& a, const Point& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	inline bool operator<(const Point& a, const Point& b)
	{
		if (a.x < b.x) return true;

		return a.y < b.y;
	}

};
