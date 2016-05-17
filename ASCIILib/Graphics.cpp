#include "Graphics.h"

#include <sstream>
#include <iostream>

#include "unicode/uchar.h"
#include "unicode/schriter.h"
#include "unicode/locid.h"

#include "SDL_image.h"

const int kFontSize = 12;

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;

//static
const unsigned int ascii::Graphics::kBufferHeight = 25;

ascii::Graphics::Graphics(const char* title, const char* fontpath)
	: Surface(kBufferWidth, kBufferHeight),
    mTitle(title), mScale(1.0f), mFullscreen(false),
    mBackgroundColor(ascii::Color::Black), mWindow(NULL), mRenderer(NULL),
    mHidingImages(false), mHasSpecialCharTable(false)
{
	TTF_Init();

	mFont = TTF_OpenFont(fontpath, kFontSize);
    
    Initialize(mScale);
}

ascii::Graphics::Graphics(const char* title, const char* fontpath,
        int bufferWidth, int bufferHeight)
	: Surface(bufferWidth, bufferHeight), mTitle(title), mScale(1.0f),
    mFullscreen(false), mBackgroundColor(ascii::Color::Black),
    mWindow(NULL), mRenderer(NULL), mHidingImages(false),
    mHasSpecialCharTable(false)
{
	TTF_Init();

	mFont = TTF_OpenFont(fontpath, kFontSize);

    Initialize(mScale);
}

ascii::Graphics::~Graphics(void)
{
    Dispose();

	TTF_CloseFont(mFont);

	TTF_Quit();
}

void ascii::Graphics::Initialize(float scale)
{
    mScale = scale;
    mFullscreen = false;

    UpdateCharSize();

    int flags = SDL_WINDOW_SHOWN;

	mWindow = SDL_CreateWindow(mTitle, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		width() * mCharWidth, height() * mCharHeight, 
		flags);

	checkSize();

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

	mCache = new ascii::ImageCache(mRenderer,
            mCharWidth / mScale,
            mCharHeight / mScale);
}

void ascii::Graphics::Dispose()
{
    clearGlyphs();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    delete mCache;
}

void ascii::Graphics::LoadSpecialCharTable(const char* path)
{
    if (mHasSpecialCharTable)
    {
        DisposeSpecialCharTable();
    }

    // Open the Unicode file
    ifstream file(path);

    // Make sure it opened properly
    if (file.is_open())
    {
        // Make a reusable word break iterator
        StringCharacterIterator charIt("");

        // Read the first line, which holds the path to the flair sheet
        string line;
        getline(file, line);

        // Load the sheet as a texture
        SDL_Surface* surface = IMG_Load(line.c_str());
        mpFlairSheet = SDL_CreateTextureFromSurface(mRenderer, surface);
        SDL_FreeSurface(surface);

        // Parse each line of the special char table
        while (getline(file, line))
        {
            // Parse in Unicode for special characters
            UnicodeString lineUnicode(line.c_str());

            // Split the line into tokens
            charIt.setText(lineUnicode);
            vector<UnicodeString> tokens;

            UnicodeString token;
            UChar c = charIt.first();
            UChar lastc = ' ';
            while (charIt.hasNext())
            {
                if (isspace((char)c) && !isspace((char)lastc))
                {
                    tokens.push_back(token);
                    token = "";
                }
                else
                {
                    token += c;
                }

                // Skip the white-space between tokens
                lastc= c;
                c = charIt.next();
            }
            tokens.push_back(token);

            // The line will be structured as follows:
            // [Unicode char] [ASCII char] [flair index] [(optional) y offset]
            UChar specialChar = tokens[0][0];

            // Don't read a normal char if that token has more than one
            // character i.e. "NONE"
            UChar normalChar = ' ';
            if (tokens[1].length() == 1)
                normalChar = tokens[1][0];

            UnicodeString indexString = tokens[2];
            string temp;
            int flairIndex = atoi(indexString.toUTF8String(temp).c_str());

            int flairOffset = 0;
            if (tokens.size() > 3)
            {
                UnicodeString offsetString = tokens[3];
                temp = "";
                flairOffset = atoi(offsetString.toUTF8String(temp).c_str());
                cout << "Changing flair offset: " << flairOffset << endl;
            }

            ComboChar comboChar;
            comboChar.base = normalChar;
            comboChar.flairIndex = flairIndex;
            comboChar.flairOffset = flairOffset;
            mSpecialCharTable[specialChar] = comboChar;
        }

        mHasSpecialCharTable = true;
    }
    else
    {
        cout << "Error loading special character table " << path << endl;
    }
}

void ascii::Graphics::DisposeSpecialCharTable()
{
    SDL_DestroyTexture(mpFlairSheet);
    mSpecialCharTable.clear();
    mHasSpecialCharTable = false;
}

void ascii::Graphics::SetScale(float scale)
{
    if (mFullscreen)
    {
        mScale = scale;
        UpdateCharSize();
    }
    else
    {
        Dispose();
        Initialize(scale);
    }
}

void ascii::Graphics::SetFullscreen(bool fullscreen)
{
    mFullscreen = fullscreen;

    // Delete the old window if its scale is not 1, so fullscreen can scale
    // automatically
    if (fullscreen && mScale != 1.0f)
    {
        SetScale(1.0f);
    }

    Uint32 flags = 0; 
    if (fullscreen)
    {
        flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    else
    {
        Dispose();
        Initialize(1.0f);
    }

    if (SDL_SetWindowFullscreen(mWindow, flags) != 0)
    {
        cout << SDL_GetError() << endl;
    }
}

void ascii::Graphics::ToggleFullscreen()
{
    SetFullscreen(!mFullscreen);
}

int ascii::Graphics::pixelToCellX(int pixelX)
{
    if (drawOrigin().x > 0)
    {
        pixelX -= drawOrigin().x;
    }
    return pixelX / mCharWidth;
}

int ascii::Graphics::pixelToCellY(int pixelY)
{
    if (drawOrigin().y > 0)
    {
        pixelY -= drawOrigin().y;
    }
    return pixelY / mCharHeight;
}

ascii::Point ascii::Graphics::drawOrigin()
{
    // Calculate the top-left corner of the rendering region
    int drawX = 0;
    int drawY = 0;

    Point actualRes = actualResolution();
    Point drawRes = drawResolution();

    if (actualRes.x > drawRes.x || actualRes.y > drawRes.y)
    {
        // Dealing with a fullscreen window
        drawX = actualRes.x / 2 - drawRes.x / 2;
        drawY = actualRes.y / 2 - drawRes.y / 2;
    }

    return Point(drawX, drawY);
}

void ascii::Graphics::clearScreen()
{
	// Draw background color
	SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, ascii::Color::kAlpha);
	SDL_RenderFillRect(mRenderer, NULL);
}

void ascii::Graphics::drawImages(std::map<std::string, Image>* images)
{
    // Don't draw any images if they're currently being hidden
    if (!mHidingImages)
    {
        int drawX = drawOrigin().x;
        int drawY = drawOrigin().y;

        // Draw every image in the given map otherwise, using their specified
        // positions
        for (auto it = images->begin(); it != images->end(); ++it)
        {
            SDL_Rect dest;
            
            dest.x = drawX + it->second.second.x * mCharWidth;
            dest.y = drawY + it->second.second.y * mCharHeight;

            SDL_QueryTexture(it->second.first, NULL, NULL, &dest.w, &dest.h);

            dest.w *= mScale;
            dest.h *= mScale;

            SDL_RenderCopy(mRenderer, it->second.first, NULL, &dest);
        }
    }
}

void ascii::Graphics::drawBackgroundColors(ascii::Surface* surface, int x, int y)
{
    int drawX = drawOrigin().x;
    int drawY = drawOrigin().y;

	for (int ySrc = 0; ySrc < surface->height(); ++ySrc) 
    {
		int xSrc = 0;

		while (xSrc < surface->width())
		{
			//chain all adjacent background colors in a row for more efficient rendering
			SDL_Rect colorRect;

			colorRect.x = drawX + (x + xSrc) * mCharWidth;
			colorRect.y = drawY + (y + ySrc) * mCharHeight;
			colorRect.w = 0;
			colorRect.h = mCharHeight;

			Color backgroundColor = surface->getBackgroundColor(xSrc, ySrc);

			do
			{
				if (!surface->isCellOpaque(xSrc, ySrc))
                {
                    ++xSrc;
                    break;
                }

				colorRect.w += mCharWidth;
				++xSrc;
			} while (xSrc < surface->width() && surface->getBackgroundColor(xSrc, ySrc) == backgroundColor);

			SDL_SetRenderDrawColor(mRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, Color::kAlpha);
			SDL_RenderFillRect(mRenderer, &colorRect);
		}
	}
}

void ascii::Graphics::drawCharacters(ascii::Surface* surface, int x, int y)
{
    int drawX = drawOrigin().x;
    int drawY = drawOrigin().y;

	//draw all characters
	for (int ySrc = 0; ySrc < surface->height(); ++ySrc)
	{
		int xSrc = 0;

		while (xSrc < surface->width())
		{
			// Chain all adjacent characters with the same color into strings
            // for more efficient rendering

            // Don't bother chaining spaces together
			char ch = getCharacter(xSrc, ySrc);

			if (isspace(ch))
			{
				++xSrc;
				continue;
			}

            // If the character is not a space, start chaining with its
            // neighbors
            UnicodeString charChain;
			SDL_Rect textRect;

			textRect.x = drawX + (x + xSrc) * mCharWidth;
			textRect.y = drawY + (y + ySrc) * mCharHeight;
			textRect.w = 0;
			textRect.h = mCharHeight;
			Color characterColor = surface->getCharacterColor(xSrc, ySrc);

			do
			{
				if (!surface->isCellOpaque(xSrc, ySrc))
				{
					++xSrc;
					break;
				}

                // First process each character as unicode to see if it must be
                // rendered as a combo of a normal character and a flair
				UChar uch = surface->getCharacter(xSrc, ySrc);
                ch = (char)uch;

                if (mHasSpecialCharTable && mSpecialCharTable.find(uch)
                        != mSpecialCharTable.end())
                {
                    // Must process as a special character
                    ComboChar combo = mSpecialCharTable[uch];
                    // Adopt a normal character as base
                    ch = combo.base;
                    // Retrieve the index of the flair to draw in conjunction
                    int flairIndex = combo.flairIndex;
                    // Retrieve the y offset for drawing the flair
                    int flairOffset = combo.flairOffset;

                    cout << "Flair offset: " << flairOffset << endl;

                    // Draw the flair
                    SDL_Rect src = Rectangle(
                            flairIndex * mCharWidth,
                            0,
                            mCharWidth,
                            mCharHeight);
                    SDL_Rect dest = Rectangle(
                            xSrc * mCharWidth,
                            ySrc * mCharHeight + flairOffset,
                            mCharWidth,
                            mCharHeight);

                    // Using the proper color
                    SDL_SetTextureColorMod(mpFlairSheet,
                            characterColor.r,
                            characterColor.g,
                            characterColor.b);

                    SDL_RenderCopy(mRenderer, mpFlairSheet, &src, &dest);
                }

                // Don't chain empty space in a word. Empty space can exist
                // here if it is used as the base for a non-space character
                // combo
                if (!isspace(ch))
                {
                    charChain += uch;
                    textRect.w += mCharWidth;
                }

				++xSrc;
			} while (
                // Stop when we reach the end of the row
                xSrc < surface->width()
                // Stop if the next character has a different color
                && surface->getCharacterColor(xSrc, ySrc) == characterColor
                // Stop if the next character is another space
                && !isspace((char)surface->getCharacter(xSrc, ySrc)));

            // Convert the unicode into an appropriate string encoding for
            // TTF_RenderText()
            int32_t start = 0;
            uint32_t size = 0;
            int32_t charsNeeded = charChain.extract(start, charChain.length(), NULL, size);
            char cstr[charsNeeded + 1];
            charChain.extract(start, charChain.length(), cstr, charsNeeded + 1);
            string str(cstr);

			Glyph glyph = std::make_pair(str, characterColor);

			SDL_Texture* texture = NULL;

			if (mGlyphTextures[glyph])
			{
				texture = mGlyphTextures[glyph];
			}
			else
			{
				SDL_Surface* surface = TTF_RenderText_Solid(mFont, cstr, characterColor);
				texture = SDL_CreateTextureFromSurface(mRenderer, surface);

				mGlyphTextures[glyph] = texture;
				
				SDL_FreeSurface(surface);
			}

			SDL_RenderCopy(mRenderer, texture, NULL, &textRect);
		}
	}
}

void ascii::Graphics::drawSurface(ascii::Surface* surface, int x, int y)
{
	// Draw all background colors
    drawBackgroundColors(surface, x, y);

    // Draw all characters
    drawCharacters(surface, x, y);
}

void ascii::Graphics::refresh()
{
    SDL_RenderPresent(mRenderer);
}

void ascii::Graphics::update()
{
    // Clear the screen for drawing
    clearScreen();

	// Draw background images
    drawImages(&mBackgroundImages);

    // Draw the buffer surface in between
    drawSurface(this, 0, 0);

	// Draw foreground images
    drawImages(&mForegroundImages);

    // Draw foreground surfaces
    for (int i = 0; i < mForegroundSurfaces.size(); ++i)
    {
        Surface* surface = mForegroundSurfaces[i].first;
        Point position = mForegroundSurfaces[i].second;

        drawSurface(surface, position.x, position.y);
    }

    // Clear any surfaces from the foreground
    mForegroundSurfaces.clear();

    // Refresh the window to show all changes
    refresh();
}

void ascii::Graphics::drawForegroundSurface(ascii::Surface* surface, int x, int y)
{
    mForegroundSurfaces.push_back(make_pair(surface, Point(x, y)));
}

void ascii::Graphics::addBackgroundImage(std::string key, std::string textureKey, int x, int y)
{
	mBackgroundImages[key] = std::make_pair(mCache->getTexture(textureKey), ascii::Point(x, y));
}

void ascii::Graphics::removeBackgroundImage(std::string key)
{
	mBackgroundImages.erase(key);
}

void ascii::Graphics::addForegroundImage(std::string key, std::string textureKey, int x, int y)
{
	mForegroundImages[key] = std::make_pair(mCache->getTexture(textureKey), ascii::Point(x, y));
}

void ascii::Graphics::removeForegroundImage(std::string key)
{
	mForegroundImages.erase(key);
}

void ascii::Graphics::clearImages()
{
	mBackgroundImages.clear();
	mForegroundImages.clear();
}

void ascii::Graphics::hideImages()
{
    mHidingImages = true;
}

void ascii::Graphics::showImages()
{
    mHidingImages = false;
}

void ascii::Graphics::clearGlyphs()
{
	for (std::map<Glyph, SDL_Texture*>::iterator it = mGlyphTextures.begin(); it != mGlyphTextures.end(); ++it)
	{
		SDL_DestroyTexture(it->second); //destroy all stored string textures
	}

	mGlyphTextures.clear();
}

ascii::Point ascii::Graphics::drawResolution()
{
    return ascii::Point(mCharWidth * width(), mCharHeight * height());
}

ascii::Point ascii::Graphics::actualResolution()
{
    int w, h;
    SDL_GetWindowSize(mWindow, &w, &h);
    return ascii::Point(w, h);
}

void ascii::Graphics::checkSize()
{
    if (!mFullscreen)
    {
        int w, h;
        SDL_GetWindowSize(mWindow, &w, &h);

        SDL_assert(width() * mCharWidth == w && height() * mCharHeight == h);
    }
}

void ascii::Graphics::UpdateCharSize()
{
	TTF_SizeText(mFont, " ", &mCharWidth, &mCharHeight);
    mCharWidth *= mScale;
    mCharHeight *= mScale;
}
