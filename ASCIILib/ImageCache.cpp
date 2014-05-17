#include "ImageCache.h"

#include <SDL_image.h>

ascii::ImageCache::ImageCache(SDL_Renderer* renderer, int charWidth, int charHeight)
	: mRenderer(renderer), mCharWidth(charWidth), mCharHeight(charHeight)
{

}

ascii::ImageCache::~ImageCache()
{
	clearTextures();
}

void ascii::ImageCache::loadTexture(std::string key, const char* path, ascii::Color colorKey)
{
	SDL_Surface* imageSurface = IMG_Load(path);

	//Make sure the image dimensions will align to the buffer
	SDL_assert(imageSurface->w % mCharWidth == 0);
	SDL_assert(imageSurface->h % mCharHeight == 0);

	SDL_SetColorKey(imageSurface, SDL_ENABLE, colorKey.ToUint32(imageSurface->format));

	SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(mRenderer, imageSurface);

	mTextures[key] = imageTexture;

	SDL_FreeSurface(imageSurface);
}

void ascii::ImageCache::loadTexture(std::string key, const char* path)
{
	SDL_Surface* imageSurface = IMG_Load(path);

	//Make sure the image dimensions will align to the buffer
	SDL_assert(imageSurface->w % mCharWidth == 0);
	SDL_assert(imageSurface->h % mCharHeight == 0);

	SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(mRenderer, imageSurface);

	mTextures[key] = imageTexture;

	SDL_FreeSurface(imageSurface);
}

void ascii::ImageCache::freeTexture(std::string key)
{
	SDL_Texture* imageTexture = mTextures[key];

	SDL_DestroyTexture(imageTexture);

	mTextures.erase(key);
}

SDL_Texture* ascii::ImageCache::getTexture(std::string key)
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