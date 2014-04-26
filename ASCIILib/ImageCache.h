#pragma once

#include <map>

#include <SDL.h>

namespace ascii
{
	
	///<summary>
	/// Loads and handles the memory life-cycle of all images used in ASCIILib games.
	///</summary>
	class ImageCache
	{
		public:
			///<summary>
			/// Creates an ImageCache and prepares it for loading textures.
			///</summary>
			ImageCache(SDL_Renderer* renderer, int charWidth, int charHeight);
			~ImageCache();

			///<summary>
			/// Loads a texture and stores it in the cache. Ensures that the texture will align with the "Console" buffer.
			///</summary>
			///<param name="key">A simple, unique, and descriptive key to associate this texture with.</param>
			///<param name="path">The filepath of the texture to load (must be a bitmap).</param>
			void loadTexture(const char* key, const char* path);

			///<summary>
			/// Frees the texture in the cache associated with the given key string.
			///</summary>
			void freeTexture(const char* key);

			///<summary>
			/// Gets a texture from the cache.
			///</summary>
			///<param name="key">The unique key with which this texture was loaded.</param>
			///<returns>The texture associated with the given key.</returns>
			SDL_Texture* getTexture(const char* key);

			///<summary>
			/// Frees all textures currently held in the cache.
			///</summary>
			void clearTextures();
		private:
			SDL_Renderer* mRenderer;
			int mCharWidth, mCharHeight;

			std::map<const char*, SDL_Texture*> mTextures;
	};

};