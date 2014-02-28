#pragma once

#include "Graphics.h"
#include "Surface.h"

namespace ascii
{

	///<summary>
	/// A camera with a fixed size viewport that renders the visible portion of a surface.
	///</summary>
	class Camera
	{
		public:
			///<summary>
			/// Constructs a camera.
			///</summary>
			///<param name="viewSurface">The surface for the camera to view.</param>
			///<param name="viewportWidth">The width of the camera's viewport.</param>
			///<param name="viewportHeight">The height of the camera's viewport.</param>
			Camera(Surface* viewSurface, int viewportWidth, int viewportHeight);

			///<summary>
			/// Sets the viewport position to the given value.
			///</summary>
			void setViewportPosition(int x, int y);

			///<summary>
			/// Moves the viewport by the given amount.
			///</summary>
			void moveViewport(int deltax, int deltay);

			///<summary>
			/// Draws the visible portion of the view surface at the given position.
			///</summary>
			void draw(Graphics& graphics, int x, int y);
		private:
			///<summary>
			/// Clamps the viewport position to stay within the bounds of the viewing surface.
			///</summary>
			void clampToBounds();

			Surface* mViewSurface;
			Rectangle mViewport;
	};

};