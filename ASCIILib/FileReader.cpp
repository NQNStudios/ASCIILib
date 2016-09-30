#include "FileReader.h"

#include <cstdio>
#include "unicode/ustdio.h"

#include "Log.h"
using namespace ascii;

namespace
{
    const int LINE_CHARACTER_LIMIT = 500;
}

map<UChar, bool> ascii::FileReader::charsEncountered;
void ascii::FileReader::PrintEncounteredChars()
{
    UnicodeString str;
    for (auto it = charsEncountered.begin(); it != charsEncountered.end(); ++it)
    {
        UChar uchar = it->first;
        str += uchar;
    }
    Log::Print(str);
}


ascii::FileReader::FileReader(string path)
{
    Initialize(path);
}

ascii::FileReader::FileReader(string path, string forbiddenCharactersPath)
{
    // Read the list of forbidden characters from the given forbidden
    // characters file, if there is one
    mForbiddenCharacters = ReadContents(forbiddenCharactersPath);
    mForbiddenCharacters.trim(); // Trim the trailing newline

    //Log::Print("Loading file ", false);
    //Log::Print(path, false);
    //Log::Print(" with forbidden characters: ", false);
    //Log::Print(mForbiddenCharacters);

    Initialize(path);
}

void FileReader::Initialize(string path)
{
    // Now read the UTF-8 file we're really concerned with
    UnicodeString contents = ReadContents(path);

    // If the file exists, parse each line from the file
    if (mExists)
    {
        ParseLines(contents, path);
    }
}

UnicodeString ascii::FileReader::ReadContents(string path)
{
    // Open the file
    //Log("Opening file: " + path);
    UFILE* ufile = u_fopen(path.c_str(), "r", NULL, "UTF-8");

    // Check if the file exists and was opened properly
    mExists = (ufile != NULL);

    // Output a warning if the file wasn't found
    if (!mExists)
    {
        Log::Print("Possible error: Tried to open nonexistent file: " + path);
        return UnicodeString("");
    }
    else
    {
        // Retrieve the size of the file
        long fileSize = 0;
        FILE* file = u_fgetfile(ufile);
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        u_frewind(ufile);

        // Prepare enough space to read the entire string plus termination
        UChar* contents = new UChar[fileSize + 1];

        // Read every character
        long index = 0;

        // Keep a working count of which line and character we're reading
        int line = 1;
        int lineCharacter = 0;

        while (!u_feof(ufile))
        {
            UChar nextChar = u_fgetc(ufile);
            contents[index++] = nextChar;

            charsEncountered[nextChar] = true;

            // Make sure none of the characters we read are forbidden
            if (mForbiddenCharacters.indexOf(nextChar) != -1)
            {
                Log::Error(UnicodeString("Forbidden character '") + UnicodeString(nextChar) + UnicodeString("' found in file: ") + UnicodeString::fromUTF8(path));
                Log::Print("Line: ", false);
                Log::Print(line);
                Log::Print("Character: ", false);
                Log::Print(lineCharacter);
            }

            // Update the current position in the file
            if (nextChar == '\n')
            {
                ++line;
                lineCharacter = 0;
            }
            ++lineCharacter;
        }

        // Add the string termination
        contents[index] = '\0';

        // Create a UnicodeString object
        UnicodeString contentsUString(contents);
        // Clean up the memory used to read the UChars
        delete[] contents;
        // Close the file
        u_fclose(ufile);

        // Check if the UTF-8 Bit Order Mark is present
		UChar a, b, c;
		a = contentsUString[0];
		b = contentsUString[1];
		c = contentsUString[2];


        if(!(a!=(UChar)0xEF || b!=(UChar)0xBB || c!=(UChar)0xBF))
        {
            Log::Error("Warning. File contains UTF-8 bit order mark: " + path);

            // Strip the BOM
            contentsUString = contentsUString.tempSubString(3);
        }

        return contentsUString;
    }
}

void ascii::FileReader::ParseLines(UnicodeString contents, string path)
{
    int numChars = contents.length();
    if (numChars > 0)
    {
        // Split the file contents by line endings
        UnicodeString line = "";
        for (int i = 0; i < numChars; ++i)
        {
            UChar nextChar = contents[i];

            // Store the characters that are chained together as a line
            if (nextChar == '\n' || nextChar == 0)
            {
                line += nextChar;

                mLines.push_back(line);
                line = "";
            }
            else if (nextChar == '\r')
            {
                continue;
            }
            else
            {
                line += nextChar;
            }
        }
    }
}



bool ascii::FileReader::HasNextLine()
{
    return !mLines.empty();
}

string ascii::FileReader::NextLine(bool trimmed)
{
    UnicodeString nextLine = mLines.front();
    mLines.pop_front();

    string temp;
    string nextLineUTF8 = nextLine.toUTF8String(temp);

    if (trimmed)
    {
        if (nextLineUTF8[nextLineUTF8.size() - 1] == '\n')
        {
            nextLineUTF8 = nextLineUTF8.substr(0, nextLineUTF8.size() - 1);
        }
    }

    return nextLineUTF8;
}

UnicodeString ascii::FileReader::NextLineUnicode(bool trimmed)
{
    UnicodeString nextLine = mLines.front();
    mLines.pop_front();

    if (trimmed)
    {
        nextLine = nextLine.findAndReplace("\n", "");
    }

    return nextLine;
}

string ascii::FileReader::FullContents()
{
    // Collect the whole contents of the buffer in a new string stream
    stringstream buffer;

    while (HasNextLine())
    {
        buffer << NextLine(false);
    }

    // Return the contents as a string
    return buffer.str();
}
