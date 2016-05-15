#include "Surface.h"

#include <iostream>
#include <map>
#include <sstream>
using namespace std;

#include "unicode/schriter.h"
#include "unicode/brkiter.h"
#include "unicode/locid.h"


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

void ascii::Surface::readLine(ifstream* file, string& str)
{
    getline(*file, str);

    // remove pesky '\r' ending
    if (str.back() == '\r')
    {
        str.pop_back();
    }
}

ascii::Surface* ascii::Surface::FromFile(const char* filepath)
{
	ifstream file;
	file.open(filepath);

    if (!file.is_open())
    {
        cout << "Error! Tried to open nonexistent surface file: "
            << filepath << endl;
    }

	map<char, Color> colors;
	map<char, string> infoCodes;

	string str;
	stringstream sstream;

	readLine(&file, str); //COLORS
    readLine(&file, str);

	char symbol[1+1];
	char red[3+1];
	char green[3+1];
	char blue[3+1];

	do
	{
        stringstream sstreamb(str);

		sstreamb >> symbol;
		sstreamb >> red;
		sstreamb >> green;
		sstreamb >> blue;

        int rval = atoi(red);
        int gval = atoi(green);
        int bval = atoi(blue);

		colors[symbol[0]] = Color(rval, gval, bval);
		
		readLine(&file, str);
	} while (str.compare("INFO CODES")); //do-while loop used because COLORS will never be empty section

	//INFO CODES
	readLine(&file, str);

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

		readLine(&file, str);
	}

	//SIZE
	readLine(&file, str);
	sstream.str(str);
	char width[3+1];
	char height[3+1];
	sstream >> width;
	sstream >> height;

	Surface* surface = new Surface(atoi(width), atoi(height));

	readLine(&file, str); //CHARACTERS
	
	char character = ' ';
	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		readLine(&file, str);

		int c = 0;
		for (string::iterator it = str.begin(); it != str.end(); ++it)
		{
			character = *it;

			surface->setCharacter(c, r, character);

			++c;
		}
	}

	readLine(&file, str); //BACKGROUND COLORS
	
	char colorsymbol = ' ';
	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		readLine(&file, str);

		int c = 0;
		for (string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			surface->setBackgroundColor(c, r, colors[colorsymbol]);

			++c;
		}
	}

	readLine(&file, str); //CHARACTER COLORS
	
	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		readLine(&file, str);

		int c = 0;
		for (string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			surface->setCharacterColor(c, r, colors[colorsymbol]);

			++c;
		}
	}

	readLine(&file, str); //OPACITY

	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		readLine(&file, str);

		int c = 0;
		for (string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			bool opaque = colorsymbol == '1';

			surface->setCellOpacity(c, r, opaque);

			++c;
		}
	}

	readLine(&file, str); //SPECIAL INFO

	for (int r = 0; r < atoi(height); ++r) //for loop used because this section will have fixed size
	{
		readLine(&file, str);

		int c = 0;
		for (string::iterator it = str.begin(); it != str.end(); ++it)
		{
			colorsymbol = *it;

			if (colorsymbol != ' ')
			{
				surface->setSpecialInfo(c, r, infoCodes[colorsymbol]);
			}

			++c;
		}
	}

	file.close();

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

void ascii::Surface::blitString(UnicodeString text, Color color, int x, int y)
{
	int destx = x, desty = y;

    // Iterate through actual characters in the given string
    StringCharacterIterator it(text);

	while (destx < mWidth && desty < mHeight && it.hasNext())
	{
		mCharacters[destx][desty] = it.next();

        // Blit the character color to the space
		mCharacterColors[destx][desty] = color;

		++destx;
	}
}

void ascii::Surface::blitStringMultiline(UnicodeString text, Color color, Rectangle destination)
{
    UErrorCode error;
    BreakIterator* it = BreakIterator::createLineInstance(Locale::getUS(), error);
    it->setText(text);

	vector<UnicodeString> sections;

    // Split the string based on places a line break could go
    int32_t p = it->first();
	while (p != BreakIterator::DONE)
	{
        int start = p;
        p = it->next();
        int end = p;

        UnicodeString section(text, start, end);

		sections.push_back(section); //collect the individual sections
	}

	int x = destination.left();
	int y = destination.top();

	for (auto it = sections.begin(); it != sections.end(); ++it)
	{
		//blit each section
		UnicodeString section = *it;
		
		if (x + section.length() > destination.right())
		{
			//if there's not enough room on this line, move to the next one
			++y;
			x = destination.left();

			if (y >= destination.bottom()) break; //make sure not to write on any rows outside of the destination rectangle
		}

		blitString(section, color, x, y);
		setCharacter(x + section.length(), y, u' '); //fill in spaces

		x += section.length() + 1;
	}

    delete it;
}

void ascii::Surface::processMultilineString(UnicodeString text, Rectangle destination, int* outEndX, int* outHeightY)
{
    UErrorCode error;
    BreakIterator* it = BreakIterator::createLineInstance(Locale::getUS(), error);
    it->setText(text);

    vector<UnicodeString> sections;

    int32_t p = it->first();
    while (p != BreakIterator::DONE)
    {
        int32_t start = p;
        p = it->next();
        int32_t end = p;

        UnicodeString section(text, start, end);
        sections.push_back(section);
    }

	int x = destination.left();
	int y = destination.top();

    int lines = 1;
	for (auto it = sections.begin(); it != sections.end(); ++it)
	{
		UnicodeString section = *it;
		
		if (x + section.length() > destination.right())
		{
			//if there's not enough room on this line, move to the next one
			++y;
            ++lines;
			x = destination.left();

			if (y >= destination.bottom()) break; //make sure not to write on any rows outside of the destination rectangle
		}

		x += section.length() + 1;
	}

	*outEndX = x;
    *outHeightY = y;
}

int ascii::Surface::stringMultilineEndX(UnicodeString text, Rectangle destination)
{
    int result;
    processMultilineString(text, destination, &result, NULL);
    return result;
}

int ascii::Surface::measureStringMultilineY(UnicodeString text, Rectangle destination)
{
    int result;
    processMultilineString(text, destination, NULL, &result);
    return result;
}

ascii::Point ascii::Surface::findString(UnicodeString text)
{
    int textLength = text.length();

    // Check each point in the Surface to see if it marks the beginning of
    // a complete appearance of the desired string
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x + textLength <= width(); ++x)
        {
            UnicodeString possibleMatch;

            for (int c = 0; c < textLength; ++c)
            {
                possibleMatch += getCharacter(x+c, y);
            }

            if (!possibleMatch.compare(text))
            {
                return ascii::Point(x, y);
            }
        }
    }

    // If the string is not found, return an undefined point
    return ascii::Point::Undefined;
}

void ascii::Surface::highlightString(UnicodeString text, ascii::Color color)
{
    // Find the given string
    Point startingPosition = findString(text);

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

void ascii::Surface::highlightTokens(UnicodeString text, ascii::Color color)
{
    // Use a BreakIterator to tokenize the given string
    UErrorCode error;
    BreakIterator* it = BreakIterator::createWordInstance(Locale::getUS(), error);
    it->setText(text);

    // Highlight each token
    int32_t p = it->first();
    while (p != BreakIterator::DONE)
    {
        int32_t start = p;
        p = it->next();
        int32_t end = p;

        UnicodeString token(text, start, end);
        highlightString(token, color);
    }
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
