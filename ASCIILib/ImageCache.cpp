#include "ImageCache.h"


ascii::ImageCache::ImageCache(SDL_Renderer* renderer, int charWidth, int charHeight)
	: mRenderer(renderer), mCharWidth(charWidth), mCharHeight(charHeight)
{

}

ascii::ImageCache::~ImageCache()
{
	clearTextures();
}

void ascii::ImageCache::loadTexture(const char* key, const char* path)
{
	SDL_Surface* imageSurface = SDL_LoadBMP(path);

	//Make sure the image dimensions will align to the buffer
	SDL_assert(imageSurface->w % mCharWidth == 0);
	SDL_assert(imageSurface->h % mCharHeight == 0);

	SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(mRenderer, imageSurface);

	mTextures[key] = imageTexture;

	SDL_FreeSurface(imageSurface);
}

void ascii::ImageCache::freeTexture(const char* key)
{
	SDL_Texture* imageTexture = mTextures[key];

	SDL_DestroyTexture(imageTexture);
}

SDL_Texture* ascii::ImageCache::getTexture(const char* key)
{
	return mTextures[key];
}

void ascii::ImageCache::clearTextures()
{
	for (auto it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		SDL_DestroyTexture(it->second);
	}

	mTextures.clear();
}