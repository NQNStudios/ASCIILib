#include "Graphics.h"

#include <algorithm>

#include "unicode/uchar.h"
#include "unicode/locid.h"
#include "unicode/ustream.h"
#include "unicode/ustdio.h"

#include <SDL_image.h>

#include "Log.h"
#include "StringTokenizer.h"
#include "FileReader.h"
using namespace ascii;

const int kFontSize = 12;

//static
const unsigned int ascii::Graphics::kBufferWidth = 80;

//static
const unsigned int ascii::Graphics::kBufferHeight = 25;

namespace
{
    // Needs to be big enough for the file path of the flair sheet
    const int32_t MAX_FLAIR_TABLE_LINE_SIZE = 30;
    const string FLAIR_SHEET_KEY("FLAIR_SHEET");
}


ascii::Graphics::Graphics(const char* title, const char* fontpath,
        int bufferWidth, int bufferHeight)
	: Surface(bufferWidth, bufferHeight), mTitle(title),
    mFullscreen(false), mBackgroundColor(ascii::Color::Black),
    mWindow(NULL), mRenderer(NULL), mHidingImages(false),
    mHasFlairTable(false), mHasInversionTable(false)
{
	if(TTF_Init() == -1)
    {
        // TODO put a log error message
    }

	mFont = TTF_OpenFont(fontpath, kFontSize);

    Initialize();
}

ascii::Graphics::~Graphics(void)
{
    Dispose();

	TTF_CloseFont(mFont);

	TTF_Quit();
}

void ascii::Graphics::Initialize()
{
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
            mCharWidth,
            mCharHeight);

    if (mHasFlairTable)
    {
        LoadFlairTable(mFlairTablePath.c_str());
    }
}

void ascii::Graphics::Dispose()
{
    // Free textures we've created for rendering tokens
    clearGlyphs();
    // Free the texture of the flair table
    DisposeFlairTable();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    delete mCache;
}

void ascii::Graphics::LoadFlairTable(const char* path)
{
    if (mHasFlairTable)
    {
        DisposeFlairTable();
    }

    mFlairTablePath = path;

    // Open the UTF-8 file
    FileReader file(path);

    // Make sure it opened properly
    if (file.Exists())
    {
        // Read the first line, which holds the path to the flair sheet
        string line;

        line = file.NextLine();

        // Load the sheet as a texture
        // Strip the trailing newline
        string temp;
        string sheetPath = line;
        //Log::Print("Loading texture " + sheetPath);
        mCache->loadTexture(FLAIR_SHEET_KEY, sheetPath.c_str());

        // Parse each line of the special char table
        while(file.HasNextLine())
        {
            // Parse in Unicode for special characters
            string line = file.NextLine();
            UnicodeString lineUnicode = UnicodeString::fromUTF8(StringPiece(line.c_str()));

            //string lineOutput = lineUnicode.toUTF8String(temp);
            //Log::Print(lineUnicode);
            //Log::Print("Line size:");
            //Log::Print(lineUnicode.length());

            // Split the line into tokens
            StringTokenizer tokenizer(lineUnicode);

            // The line will be structured as follows:
            // [Unicode char] [ASCII char] [flair index] [(optional) y offset]
            UChar specialChar = tokenizer.NextToken()[0];

            // Don't read a normal char if that token has more than one
            // character i.e. "NONE"
            UChar normalChar = ' ';
            UnicodeString baseCharToken = tokenizer.NextToken();
            if (baseCharToken.length() == 1)
                normalChar = baseCharToken[0];

            UnicodeString indexString = tokenizer.NextToken();
            string temp;
            int flairIndex = atoi(indexString.toUTF8String(temp).c_str());

            int flairOffset = 0;
            if (tokenizer.HasNextToken())
            {
                UnicodeString offsetString = tokenizer.NextToken();
                temp = "";
                flairOffset = atoi(offsetString.toUTF8String(temp).c_str());
                //cout << "Changing flair offset: " << flairOffset << endl;
            }

            FlairChar flairChar;
            flairChar.base = normalChar;
            flairChar.flairIndex = flairIndex;
            flairChar.flairOffset = flairOffset;
            mFlairTable[specialChar] = flairChar;
        }

        mHasFlairTable = true;
    }
    else
    {
        Log::Print("Error loading special character table:");
        Log::Print(path);
    }
}

void ascii::Graphics::DisposeFlairTable()
{
    mCache->freeTexture(FLAIR_SHEET_KEY);
    mFlairTable.clear();
    mHasFlairTable = false;
}

void ascii::Graphics::LoadInversionTable(const char* path)
{
    // Clear out any previously loaded inversion table
    if (mHasInversionTable)
    {
        mInversionTable.clear();
    }

    // Open the file
    FileReader file(path);

    // Parse each line
    while (file.HasNextLine())
    {
        UnicodeString lineUnicode = file.NextLineUnicode();

        // The line will be in format:
        // [original character] [reference character] [inversion axes]
        //     [(optional) x offset] [(optional) y offset]
        // Inversion axes are strung together from options "x" and "y"
        // Ex. "xy" for both
        StringTokenizer tokenizer(lineUnicode);

        UChar specialChar = tokenizer.NextToken()[0];
        UChar baseChar = tokenizer.NextToken()[0];
        UnicodeString axesUnicode = tokenizer.NextToken();

        string temp;
        string axesString = axesUnicode.toUTF8String(temp);

        InvertedChar invertedChar;
        invertedChar.base = baseChar;
        invertedChar.invertX =
            (find(axesString.begin(), axesString.end(), 'x') != axesString.end());
        invertedChar.invertY =
            (find(axesString.begin(), axesString.end(), 'y') != axesString.end());

        invertedChar.offsetX = 0;
        invertedChar.offsetY = 0;
        if (tokenizer.HasNextToken())
        {
            UnicodeString offsetString;
            offsetString = tokenizer.NextToken();
            invertedChar.offsetX = atoi(offsetString.toUTF8String(temp).c_str());
            offsetString = tokenizer.NextToken();
            invertedChar.offsetY = atoi(offsetString.toUTF8String(temp).c_str());
        }

        mInversionTable[specialChar] = invertedChar;
    }

    mHasInversionTable = true;
}

void ascii::Graphics::SetFullscreen(bool fullscreen)
{
    // Don't go into any of these operations if unnecessary
    if (fullscreen == mFullscreen) return;

    mFullscreen = fullscreen;

    Uint32 flags = 0; 
    if (fullscreen)
    {
        flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    else
    {
        Dispose();
        Initialize();
    }

    if (SDL_SetWindowFullscreen(mWindow, flags) != 0)
    {
        Log::SDLError();
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
			UChar uch = surface->getCharacter(xSrc, ySrc);

			if (IsWhiteSpace(uch))
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
                // rendered as a FlairChar or InvertedChar
				UChar uch = surface->getCharacter(xSrc, ySrc);

                if (mHasFlairTable && mFlairTable.find(uch)
                        != mFlairTable.end())
                {
                    //cout << "Processing special character" << endl;
                    // Must process as a special character
                    FlairChar flairChar = mFlairTable[uch];
                    // Adopt a normal character as base
                    uch = flairChar.base;
                    // Retrieve the index of the flair to draw in conjunction
                    int flairIndex = flairChar.flairIndex;
                    // Retrieve the y offset for drawing the flair
                    int flairOffset = flairChar.flairOffset;

                    //cout << "Flair index: " << flairIndex << endl;
                    //cout << "Flair offset: " << flairOffset << endl;

                    // Draw the flair
                    SDL_Rect src = Rectangle(
                            flairIndex * mCharWidth,
                            0,
                            mCharWidth,
                            mCharHeight);
                    SDL_Rect dest = Rectangle(
                            drawX + (x + xSrc) * mCharWidth,
                            drawY + (y + ySrc) * mCharHeight + flairOffset,
                            mCharWidth,
                            mCharHeight);

                    SDL_Texture* flairSheet = mCache->getTexture(FLAIR_SHEET_KEY);
                    // Using the proper color
                    SDL_SetTextureColorMod(flairSheet,
                            characterColor.r,
                            characterColor.g,
                            characterColor.b);

                    SDL_RenderCopy(mRenderer, flairSheet, &src, &dest);
                }
                else if (mHasInversionTable && mInversionTable.find(uch)
                        != mInversionTable.end())
                {
                    // Must process as an Inverted character
                    InvertedChar invertedChar = mInversionTable[uch];

                    SDL_Rect dest = Rectangle(
                            drawX + (x + xSrc) * mCharWidth + invertedChar.offsetX,
                            drawY + (y + ySrc) * mCharHeight + invertedChar.offsetY,
                            mCharWidth,
                            mCharHeight);

                    UnicodeString ustr; ustr += uch;
                    string temp;
                    string str = ustr.toUTF8String(temp);
                    Glyph glyph = std::make_pair(str, characterColor);

                    SDL_Texture* texture = NULL;

                    if (mGlyphTextures[glyph])
                    {
                        texture = mGlyphTextures[glyph];
                    }
                    else
                    {
                        UnicodeString baseCharUString = invertedChar.base;
                        temp = "";
                        string baseCharString = baseCharUString.toUTF8String(temp);
                        SDL_Surface* surface = TTF_RenderUTF8_Solid(mFont, baseCharString.c_str(), characterColor);
                        texture = SDL_CreateTextureFromSurface(mRenderer, surface);

                        // TODO flip the texture

                        mGlyphTextures[glyph] = texture;
                        
                        SDL_FreeSurface(surface);
                    }
                    // Using the proper color
                    SDL_SetTextureColorMod(texture,
                            characterColor.r,
                            characterColor.g,
                            characterColor.b);

                    SDL_RendererFlip flip = SDL_FLIP_NONE;
                    if (invertedChar.invertX) flip =
                        (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
                    if (invertedChar.invertY) flip =
                        (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);

                    SDL_RenderCopyEx(mRenderer, texture, NULL, &dest, 0, NULL, flip);
                    
                    // Don't chain this character with the rest or the inverted
                    // version will be overlapped with the regular version
                    uch = ' ';
                }

                // Don't chain empty space in a word. Empty space can exist
                // here if it is used as the base for a non-space character
                // combo
                if (!IsWhiteSpace(uch))
                {
                    charChain += uch;
                    textRect.w += mCharWidth;
                }
                else
                {
                    ++xSrc;
                    break;
                }

				++xSrc;
			} while (
                // Stop when we reach the end of the row
                xSrc < surface->width()
                // Stop if the next character has a different color
                && surface->getCharacterColor(xSrc, ySrc) == characterColor
                // Stop if the next character is another space
                && !IsWhiteSpace(surface->getCharacter(xSrc, ySrc)));

            // Convert the unicode into an appropriate string encoding for
            // TTF_RenderText()
            string temp;
            string str = charChain.toUTF8String(temp);

			Glyph glyph = std::make_pair(str, characterColor);

			SDL_Texture* texture = NULL;

			if (mGlyphTextures[glyph])
			{
				texture = mGlyphTextures[glyph];
			}
			else
			{
				SDL_Surface* surface = TTF_RenderUTF8_Solid(mFont, str.c_str(), characterColor);
				texture = SDL_CreateTextureFromSurface(mRenderer, surface);

				mGlyphTextures[glyph] = texture;
				
				SDL_FreeSurface(surface);
			}

            SDL_RenderCopy(mRenderer, texture, NULL, &textRect);
		}
	}

    //SDL_RenderCopy(mRenderer, mpFlairSheet, NULL, NULL);
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
    //cout << "Drawing flair sheet everywhere" << endl;
    //SDL_RenderCopy(mRenderer, mpFlairSheet, NULL, NULL);
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
}
