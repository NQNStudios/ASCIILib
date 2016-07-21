#include "ImageCache.h"

#include <iostream>
using namespace std;

#include <SDL_image.h>

#include "Log.h"
using ascii::Log;


ascii::ImageCache::ImageCache(SDL_Renderer* renderer, int charWidth, int charHeight)
	: mRenderer(renderer), mCharWidth(charWidth), mCharHeight(charHeight)
{

}

ascii::ImageCache::~ImageCache()
{
	clearTextures();
}

void ascii::ImageCache::loadTexture(std::string key, string path, ascii::Color colorKey)
{
	SDL_Surface* imageSurface = IMG_Load(path.c_str());
    if (!imageSurface)
    {
        Log::Error("Failed to load texture: " + path);
        Log::SDLError();
		return;
    }

	//Make sure the image dimensions will align to the buffer
	if (imageSurface->w % mCharWidth != 0 || imageSurface->h % mCharHeight != 0)
    {
        Log::Error("Warning! Loaded a texture which does not align with buffer: " + path);
    }

    if (!colorKey.isNone)
    {
        SDL_SetColorKey(imageSurface, SDL_ENABLE, colorKey.ToUint32(imageSurface->format));
    }

	SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(mRenderer, imageSurface);
    if (!imageTexture)
    {
        Log::Error("Failed to create texture " + path);
        Log::SDLError();
    }

	mTextures[key] = imageTexture;

	SDL_FreeSurface(imageSurface);
}

void ascii::ImageCache::loadTexture(std::string key, string path)
{
    loadTexture(key, path, Color::None);
}


void ascii::ImageCache::freeTexture(std::string key)
{
    //cout << "Freeing texture " << key << endl;
	SDL_Texture* imageTexture = mTextures[key];

	SDL_DestroyTexture(imageTexture);

	mTextures.erase(key);
}

SDL_Texture* ascii::ImageCache::getTexture(std::string key)
{
    if (mTextures.find(key) == mTextures.end())
    {
        Log::Error("Tried to retrieve nonexistent texture: " + key);
        return NULL;
    }
    else
    {
        return mTextures[key];
    }
}

void ascii::ImageCache::clearTextures()
{
	for (auto it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		SDL_DestroyTexture(it->second);
	}

	mTextures.clear();
}
