#include "Surface.h"

#include <iostream>
#include <map>
#include <sstream>
using namespace std;

#include "unicode/schriter.h"
#include "unicode/brkiter.h"
#include "unicode/locid.h"
#include "unicode/ustdio.h"
#include "unicode/ustream.h"

#include "FileReader.h"
#include "Log.h"
#include "StringTokenizer.h"
using namespace ascii;


const string kEmptyInfo(".");

ascii::Surface::Surface(int width, int height)
	: mWidth(width), mHeight(height), 
		mCharacters(width, vector<UChar>(height, ' ')),
		mBackgroundColors(width, vector<Color>(height, Color::Black)),
		mCharacterColors(width, vector<Color>(height, Color::White)),
		mSpecialInfo(width, vector<string>(height, ""))
{
	for (int x = 0; x < width; ++x)
	{
		mCellOpacity.push_back(new bool[height]);
        
        for (int y = 0; y < height; ++y)
        {
            mCellOpacity.back()[y] = true;
        }
	}
}

ascii::Surface::Surface(int width, int height, UChar character, Color backgroundColor, Color characterColor)
	: mWidth(width), mHeight(height),
		mCharacters(width, vector<UChar>(height, character)),
		mBackgroundColors(width, vector<Color>(height, backgroundColor)),
		mCharacterColors(width, vector<Color>(height, characterColor)),
		mSpecialInfo(width, vector<string>(height, ""))
{
	for (int x = 0; x < width; ++x)
	{
		mCellOpacity.push_back(new bool[height]);

        for (int y = 0; y < height; ++y)
        {
            mCellOpacity.back()[y] = true;
        }
	}
}

ascii::Surface::Surface(UChar character, Color backgroundColor, Color characterColor)
	: mWidth(1), mHeight(1),
		mCharacters(1, vector<UChar>(1, character)),
		mBackgroundColors(1, vector<Color>(1, backgroundColor)),
		mCharacterColors(1, vector<Color>(1, characterColor)),
		mSpecialInfo(1, vector<string>(1, ""))
{
	mCellOpacity.push_back(new bool[1]);
    mCellOpacity[0][0] = true;
}

ascii::Surface* ascii::Surface::FromFile(const char* filepath)
{
    FileReader file(filepath);

	map<char, Color> colors;
	map<char, string> infoCodes;

	string str;
	stringstream sstream;

    str = file.NextLine(); //COLORS
    str = file.NextLine();

    string symbol;
    string red;
    string green;
    string blue;

	do
	{
        stringstream sstreamb(str);

		sstreamb >> symbol;
		sstreamb >> red;
		sstreamb >> green;
		sstreamb >> blue;

        int rval = atoi(red.c_str());
        int gval = atoi(green.c_str());
        int bval = atoi(blue.c_str());

		colors[symbol[0]] = Color(rval, gval, bval);
		
        str = file.NextLine();
	} while (str.compare("INFO CODES")); //do-while loop used because COLORS will never be empty section

	//INFO CODES
    str = file.NextLine();

	char infoCode[1+1];
	string infoVal;

	while (str.compare("SIZE")) //while loop used because INFO CODES may be empty section
	{
        stringstream sstreamb(str);
		
		sstreamb >> infoCode;
		sstreamb >> infoVal;

		if (!infoVal.compare(kEmptyInfo))
		{
			infoVal = "";
		}

		infoCodes[infoCode[0]] = infoVal;

        str = file.NextLine();
	}

	//SIZE
    str = file.NextLine();
	sstream.str(str);
    string width;
    string height;
	sstream >> width;
	sstream >> height;

	Surface* surface = new Surface(atoi(width.c_str()), atoi(height.c_str()));

    // CHARACTERS
    str = file.NextLine();
	
	UChar character = ' ';
	for (int r = 0; r < surface->height(); ++r) //for loop used because this section will have fixed size
	{
		UnicodeString ustr = file.NextLineUnicode();
		int c = 0;
		for (int i = 0; i < ustr.length(); ++i)
		{
			character = ustr[i];

			surface->setCharacter(c, r, character);

			++c;
		}
	}

	file.NextLine(); //BACKGROUND COLORS
	
	char colorsymbol = ' ';
	for (int r = 0; r < surface->height(); ++r) //for loop used because this section will have fixed size
	{
        str = file.NextLine();

		int c = 0;
		for (string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			surface->setBackgroundColor(c, r, colors[colorsymbol]);

			++c;
		}
	}

	file.NextLine(); //CHARACTER COLORS
	
	for (int r = 0; r < surface->height(); ++r) //for loop used because this section will have fixed size
	{
        str = file.NextLine();

		int c = 0;
		for (string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			surface->setCharacterColor(c, r, colors[colorsymbol]);

			++c;
		}
	}

	file.NextLine(); //OPACITY

    char codesymbol;
	for (int r = 0; r < surface->height(); ++r) //for loop used because this section will have fixed size
	{
        str = file.NextLine();

		int c = 0;
		for (string::iterator it = str.begin(); it != str.end(); ++it)
		{
			codesymbol = *it;

			bool opaque = codesymbol == '1';

			surface->setCellOpacity(c, r, opaque);

			++c;
		}
	}

	file.NextLine(); //SPECIAL INFO

	for (int r = 0; r < surface->height(); ++r) //for loop used because this section will have fixed size
	{
        str = file.NextLine();

		int c = 0;
		for (string::iterator it = str.begin(); it != str.end(); ++it)
		{
			codesymbol = *it;

			if (codesymbol != ' ')
			{
                string specialInfo = infoCodes[codesymbol];

				surface->setSpecialInfo(c, r, specialInfo);
			}

			++c;
		}
	}

	return surface;
}

void ascii::Surface::clear()
{
	fill(' ', Color::Black, Color::White);
}

void ascii::Surface::clearTransparent()
{
	for (int x = 0; x < mWidth; ++x)
	{
		for (int y = 0; y < mHeight; ++y)
		{
			setCellOpacity(x, y, false);
		}
	}
}

void ascii::Surface::clearOpaque()
{
	for (int x = 0; x < mWidth; ++x)
	{
		for (int y = 0; y < mHeight; ++y)
		{
			setCellOpacity(x, y, true);
		}
	}
}

void ascii::Surface::fill(UChar character, Color backgroundColor, Color characterColor)
{
	for (int x = 0; x < mWidth; ++x)
	{
		for (int y = 0; y < mHeight; ++y)
		{
			setCharacter(x, y, character);
			setBackgroundColor(x, y, backgroundColor);
			setCharacterColor(x, y, characterColor);
		}
	}
}

void ascii::Surface::fillRect(Rectangle destination, UChar character, Color backgroundColor, Color characterColor)
{
	for (int x = destination.left(); x < destination.right(); ++x)
	{
		for (int y = destination.top(); y < destination.bottom(); ++y)
		{
			setCharacter(x, y, character);
			setBackgroundColor(x, y, backgroundColor);
			setCharacterColor(x, y, characterColor);
		}
	}
}

void ascii::Surface::drawBorder(UChar character, Color backgroundColor, Color characterColor)
{
	int x1 = 0;
	int x2 = mWidth - 1;

	int y1 = 0;
	int y2 = mHeight - 1;

	//draw the top and bottom
	for (int x = x1; x <= x2; ++x)
	{
		setCharacter(x, y1, character);
		setBackgroundColor(x, y1, backgroundColor);
		setCharacterColor(x, y1, characterColor);

		setCharacter(x, y2, character);
		setBackgroundColor(x, y2, backgroundColor);
		setCharacterColor(x, y2, characterColor);
	}

	//draw the left and right
	for (int y = y1; y <= y2; ++y)
	{
		setCharacter(x1, y, character);
		setBackgroundColor(x1, y, backgroundColor);
		setCharacterColor(x1, y, characterColor);

		setCharacter(x2, y, character);
		setBackgroundColor(x2, y, backgroundColor);
		setCharacterColor(x2, y, characterColor);
	}
}

void ascii::Surface::drawRect(Rectangle destination, UChar character, Color backgroundColor, Color characterColor)
{
	int x1 = destination.left();
	int x2 = destination.right() - 1;

	int y1 = destination.top();
	int y2 = destination.bottom() - 1;

	//draw the top and bottom
	for (int x = x1; x <= x2; ++x)
	{
		setCharacter(x, y1, character);
		setBackgroundColor(x, y1, backgroundColor);
		setCharacterColor(x, y1, characterColor);

		setCharacter(x, y2, character);
		setBackgroundColor(x, y2, backgroundColor);
		setCharacterColor(x, y2, characterColor);
	}

	//draw the left and right
	for (int y = y1; y <= y2; ++y)
	{
		setCharacter(x1, y, character);
		setBackgroundColor(x1, y, backgroundColor);
		setCharacterColor(x1, y, characterColor);

		setCharacter(x1, y, character);
		setBackgroundColor(x1, y, backgroundColor);
		setCharacterColor(x1, y, characterColor);
	}
}

void ascii::Surface::copySurface(Surface* surface, int x, int y)
{
	//copy cell info from the other surface
	for (int destx = x, srcx = 0; destx < mWidth && srcx < surface->mWidth; ++destx, ++srcx)
	{
		for (int desty = y, srcy = 0; desty < mHeight && srcy < surface->mHeight; ++desty, ++srcy)
		{
			if (destx >= 0 && desty >= 0)
			{
				mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
				mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
				mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
				mSpecialInfo[destx][desty] = surface->mSpecialInfo[srcx][srcy];
				mCellOpacity[destx][desty] = surface->mCellOpacity[srcx][srcy];
			}
		}
	}
}

void ascii::Surface::copySurface(Surface* surface, Rectangle source, int x, int y)
{
	//copy cell info from the other surface
	for (int destx = x, srcx = source.x; destx < mWidth && srcx < source.right(); ++destx, ++srcx)
	{
		for (int desty = y, srcy = source.y; desty < mHeight && srcy < source.bottom(); ++desty, ++srcy)
		{
			if (destx >= 0 && desty >= 0)
			{
				mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
				mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
				mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
				mSpecialInfo[destx][desty] = surface->mSpecialInfo[srcx][srcy];
				mCellOpacity[destx][desty] = surface->mCellOpacity[srcx][srcy];
			}
		}
	}
}

void ascii::Surface::blitSurface(Surface* surface, int x, int y)
{
	//blit the opaque cells from the other surface
	for (int destx = x, srcx = 0; destx < mWidth && srcx < surface->mWidth; ++destx, ++srcx)
	{
		for (int desty = y, srcy = 0; desty < mHeight && srcy < surface->mHeight; ++desty, ++srcy)
		{
			if (destx >= 0 && desty >= 0 && surface->isCellOpaque(srcx, srcy))
			{
				mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
				mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
				mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
				mSpecialInfo[destx][desty] = surface->mSpecialInfo[srcx][srcy];

				mCellOpacity[destx][desty] = true; //Cover transparent cells in the lower surface with opaque ones
			}
		}
	}
}

void ascii::Surface::blitSurface(Surface* surface, Rectangle source, int x, int y)
{
	//blit the opaque cells from the other surface
	for (int destx = x, srcx = source.x; destx < mWidth && srcx < source.right(); ++destx, ++srcx)
	{
		for (int desty = y, srcy = source.y; desty < mHeight && srcy < source.bottom(); ++desty, ++srcy)
		{
			if (destx >= 0 && desty >= 0 && surface->isCellOpaque(srcx, srcy))
			{
				mCharacters[destx][desty] = surface->mCharacters[srcx][srcy];
				mBackgroundColors[destx][desty] = surface->mBackgroundColors[srcx][srcy];
				mCharacterColors[destx][desty] = surface->mCharacterColors[srcx][srcy];
				mSpecialInfo[destx][desty] = surface->mSpecialInfo[srcx][srcy];

				mCellOpacity[destx][desty] = true; //Cover transparent cells in the lower surface with opaque ones
			}
		}
	}
}

void ascii::Surface::transposeSpecialInfo(Surface* surface, int x, int y)
{
	// everywhere where special info exists on the other surface, add it to
	// this one
	for (int destx = x, srcx = 0; destx < mWidth && srcx < surface->mWidth; ++destx, ++srcx)
	{
		for (int desty = y, srcy = 0; desty < mHeight && srcy < surface->mHeight; ++desty, ++srcy)
		{
			if (destx >= 0 && desty >= 0 && !surface->mSpecialInfo[srcx][srcy].empty())
			{
				mSpecialInfo[destx][desty] = surface->mSpecialInfo[srcx][srcy];
			}
		}
	}
}

void ascii::Surface::applyMask(Surface* surface, int x, int y)
{
	// Set cells on this surface opaque if an opaque cell from the given
    // surface would cover them
	for (int destx = x, srcx = 0; destx < mWidth && srcx < surface->width(); ++destx, ++srcx)
	{
		for (int desty = y, srcy = 0; desty < mHeight && srcy < surface->height(); ++desty, ++srcy)
		{
			if (destx >= 0 && desty >= 0 && surface->isCellOpaque(srcx, srcy))
			{
				mCellOpacity[destx][desty] = true;
			}
		}
	}
}

void ascii::Surface::blitString(UnicodeString text, Color color, int x, int y, Color backgroundColor)
{
	int destx = x, desty = y;

    // Iterate through actual characters in the given string
    StringCharacterIterator it(text);

    UChar next = it.first();
	while (destx < mWidth && desty < mHeight && it.hasNext())
	{
		setCharacter(destx, desty, next);

        // Blit the character color to the space
		setCharacterColor(destx, desty, color);
        // If a background color was specified, apply it to the cell
        if (!backgroundColor.isNone)
        {
            setBackgroundColor(destx, desty, backgroundColor);
        }

		++destx;
        next = it.next();
	}
}

void ascii::Surface::blitStringMultiline(UnicodeString text, Color color, Rectangle destination, Color backgroundColor)
{
    int discard;

    // First clear all characters out of the destination rectangle
    for (int x = destination.left(); x < destination.right(); ++x)
    {
        for (int y = destination.top(); y < destination.bottom(); ++y)
        {
            setCharacter(x, y, ' ');
            // If a background color was provided, apply it to every cell in
            // the bounding rectangle
            if (!backgroundColor.isNone)
            {
                setBackgroundColor(x, y, backgroundColor);
            }
        }
    }

    processMultilineString(text, destination, &discard, &discard, this, color);
}

void ascii::Surface::processMultilineString(UnicodeString text, Rectangle destination, int* outEndX, int* outHeightY, Surface* blitTo=NULL, Color color=Color::Black)
{
    StringTokenizer tokenizer(text);

    vector<UnicodeString> sections;

    while (tokenizer.HasNextToken())
    {
        UnicodeString section = tokenizer.NextToken(false);
        sections.push_back(section);
    }

	int x = destination.left();
	int y = destination.top();

    int lines = 1;
	for (int i = 0; i < sections.size(); ++i)
	{
        // Process each section
		UnicodeString section = sections[i];

        // Trim it of leading and trailing whitespace
        UnicodeString trimmed = section;
        trimmed.trim();

        // Wrap to a new line if the section is too large
		if (x + trimmed.length() > destination.right())
		{
			//if there's not enough room on this line, move to the next one
			++y;
            ++lines;
			x = destination.left();

			if (y >= destination.bottom()) break; //make sure not to write on any rows outside of the destination rectangle
		}

        if (blitTo)
        {
            // Blit the section on the line where it belongs
            blitTo->blitString(trimmed, color, x, y);
        }

        // Bump x over to where this section terminates (counting included
        // whitespace)
        x += section.length();
	}

	*outEndX = x;
    *outHeightY = lines;
}

int ascii::Surface::stringMultilineEndX(UnicodeString text, Rectangle destination)
{
    int result;
    int discard;
    processMultilineString(text, destination, &result, &discard);
    return result;
}

int ascii::Surface::measureStringMultilineY(UnicodeString text, Rectangle destination)
{
    int result;
    int discard;
    processMultilineString(text, destination, &discard, &result);
    return result;
}

ascii::Point ascii::Surface::findCharacter(UChar character, Point searchStart)
{
    Log::Print("Searching for character ", false);
    Log::Print(UnicodeString(character), false);
    Log::Print(" at position:");
    Log::Print(searchStart.x);
    Log::Print(searchStart.y);
    // Loop through rows first, so that we can find the next occurance just by
    // incrementing x of the resulting point and using that as the next search
    // start
    for (int y = searchStart.y; y < height(); ++y)
    {
        for (int x = searchStart.x; x < width(); ++x)
        {
            // Only begin the x-wise search at that starting coordinate for the
            // first row checked!
            searchStart.x = 0;

            if (mCharacters[x][y] == character)
            {
                return Point(x, y);
            }
        }
    }

    return Point::Undefined;
}

ascii::Point ascii::Surface::findCharacter(UChar character)
{
    return findCharacter(character, Point::Origin);
}

ascii::Point ascii::Surface::findString(UnicodeString text, Point searchStart)
{
    int textLength = text.length();

    if (textLength == 0)
    {
        Log::Print("Error! Tried to search Surface for occurance of empty string.");
        return Point::Undefined;
    }

    // Check each occurance of the first character, to see if it is the start
    // of a complete appearance of the desired string
    Point occurance = findCharacter(text[0], searchStart);

    while (occurance.defined)
    {
        Log::Print("Found an occurance of character ", false);
        Log::Print(UnicodeString(text[0]), false);
        Log::Print(" at position:");
        Log::Print(occurance.x);
        Log::Print(occurance.y);

        UnicodeString possibleMatch;

        for (int c = 0; c < textLength; ++c)
        {
            possibleMatch += getCharacter(occurance.x+c, occurance.y);
        }

        if (!possibleMatch.compare(text))
        {
            return ascii::Point(occurance.x, occurance.y);
        }

        searchStart = Point(occurance.x + 1, occurance.y);
        occurance = findCharacter(text[0], searchStart);
    }

    // If the string is not found, return an undefined point
    return ascii::Point::Undefined;
}

ascii::Point ascii::Surface::findString(UnicodeString text)
{
    return findString(text, Point::Origin);
}

void ascii::Surface::highlightString(UnicodeString text, ascii::Color color,
        Point searchStart)
{
    // Find the given string
    Point startingPosition = findString(text, searchStart);

    // If it is found...
    if (startingPosition.defined)
    {
        // ...Set the character color of each cell to the highlight
        for (int x = startingPosition.x; x < startingPosition.x + text.length(); ++x)
        {
            setCharacterColor(x, startingPosition.y, color);
        }
    }
}

void ascii::Surface::highlightString(UnicodeString text, ascii::Color color)
{
    highlightString(text, color, Point::Origin);
}

void ascii::Surface::highlightTokens(UnicodeString text, ascii::Color color,
        Point searchStart)
{
    StringTokenizer tokenizer(text);

    // Highlight each token
    while (tokenizer.HasNextToken())
    {
        UnicodeString token = tokenizer.NextToken();
        highlightString(token, color, searchStart);
    }
}

void ascii::Surface::highlightTokens(UnicodeString text, ascii::Color color)
{
    highlightTokens(text, color, Point::Origin);
}

void ascii::Surface::highlightAllTokens(UnicodeString text, ascii::Color color)
{
    StringTokenizer tokenizer(text);

    // Process each token
    while (tokenizer.HasNextToken())
    {
        UnicodeString token = tokenizer.NextToken();

        // Highlight the token every place where it appears
        Point searchStart = findString(token);

        while (searchStart.defined)
        {
            highlightString(token, color, searchStart);

            ++searchStart.x;
            searchStart = findString(token, searchStart);
        }
    }
}

ascii::Rectangle ascii::Surface::getSpecialRectangle(string key)
{
    vector<Point> correspondingPoints;

    // Find the cells that correspond to the given special key
    for (int x = 0; x < width(); ++x)
    {
        for (int y = 0; y < height(); ++y)
        {
            string specialInfo = mSpecialInfo[x][y];

            if (!specialInfo.compare("POINT_" + key))
            {
                correspondingPoints.push_back(Point(x, y));
            }
        }
    }

    // If there are any number other than two corresponding points,
    // something is wrong
    if (correspondingPoints.size() != 2)
    {
        Log::Print("Error! Tried to construct special rectangle with other than 2 points: ", false);
        Log::Print(key);

        return Rectangle::Empty;
    }

    // Construct the rectangle from the two points
    Point p1 = correspondingPoints[0];
    Point p2 = correspondingPoints[1];

    //cout << "Point 1: (" << p1.x << ", " << p1.y << ")" << endl;
    //cout << "Point 2: (" << p2.x << ", " << p2.y << ")" << endl;

    int x = p1.x;
    int y = p1.y;
    int width = p2.x - p1.x + 1;
    int height = p2.y - p1.y + 1;

    return Rectangle(x, y, width, height);
}
void ascii::Surface::printContents()
{
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x < width(); ++x)
        {
            cout << getCharacter(x, y);
        }

        cout << endl;
    }
}
