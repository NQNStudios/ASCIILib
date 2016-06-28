#include "FileReader.h"

#include <cstdio>
#include "unicode/ustdio.h"

#include "Log.h"
using namespace ascii;

namespace
{
    const int LINE_CHARACTER_LIMIT = 500;
}


ascii::FileReader::FileReader(string path)
{
    // Open the file
    //Log("Opening file: " + path);
    UFILE* ufile = u_fopen(path.c_str(), "r", NULL, "UTF-8");

    mExists = (ufile != NULL);

    // Output a warning if the file wasn't found
    if (!Exists())
    {
        Log::Print("Possible error: Tried to open nonexistent file: " + path);
    }
    // Otherwise, extract each line from the file
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
        while (!u_feof(ufile))
        {
            contents[index++] = u_fgetc(ufile);
        }

        // Add the string termination
        contents[index] = 0;

        // Close the file
        u_fclose(ufile);

        // Check if the UTF-8 BOM is present
        UChar a,b,c;
        a = contents[0];
        b = contents[1];
        c = contents[2];

        if(!(a!=(UChar)0xEF || b!=(UChar)0xBB || c!=(UChar)0xBF))
        {
            Log::Print("Warning. File contains UTF-8 bit order mark:");
            Log::Print(path);
        }

        // Split the file contents by line endings
        UnicodeString line = "";
        for (int i = 0; i < index; ++i)
        {
            UChar nextChar = contents[i];

            // Store the characters that are chained together as a line
            if (nextChar == '\n' || nextChar == 0)
            {
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

        // TODO is it right to do this?
        // Clean up the memory used to read the UChars
        delete[] contents;
    }
}

bool ascii::FileReader::HasNextLine()
{
    return !mLines.empty();
}

string ascii::FileReader::NextLine()
{
    UnicodeString nextLine = mLines.front();
    mLines.pop_front();

    string temp;
    //ULog(nextLine);
    return nextLine.toUTF8String(temp);
}

UnicodeString ascii::FileReader::NextLineUnicode()
{
    UnicodeString nextLine = mLines.front();
    mLines.pop_front();
    return nextLine;
}

string ascii::FileReader::FullContents()
{
    // Collect the whole contents of the buffer in a new string stream
    stringstream buffer;

    while (HasNextLine())
    {
        buffer << NextLine();
    }

    // Return the contents as a string
    return buffer.str();
}
