#include "Camera.h"


ascii::Camera::Camera(Surface* viewSurface, int viewportWidth, int viewportHeight)
	: mViewport(0, 0, viewportWidth, viewportHeight)
{
	mViewSurface = viewSurface;
}

void ascii::Camera::setViewportPosition(int x, int y)
{
	mViewport.x = x;
	mViewport.y = y;

	clampToBounds();
}

void ascii::Camera::moveViewport(int deltax, int deltay)
{
	mViewport.x += deltax;
	mViewport.y += deltay;

	clampToBounds();
}

void ascii::Camera::draw(Graphics& graphics, int x, int y)
{
	graphics.blitSurface(mViewSurface, mViewport, x, y);
}

void ascii::Camera::clampToBounds()
{
	//check top left corner
	if (mViewport.x < 0) mViewport.x = 0;
	if (mViewport.y < 0) mViewport.y = 0;

	//check bottom right corner
	if (mViewport.x > mViewSurface->width() - mViewport.width) 
		mViewport.x = mViewSurface->width() - mViewport.width;

	if (mViewport.y > mViewSurface->height() - mViewport.height) 
		mViewport.y = mViewSurface->height() - mViewport.height;
}