#include "FileReader.h"

#include <iostream>
#include <fstream>

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
    ifstream file(path);

    mExists = (file.is_open());

    // Output a warning if the file wasn't found
    if (!Exists())
    {
        Log::Print("Possible error: Tried to open nonexistent file: " + path);
    }
    // Otherwise, extract each line from the file
    else
    {
        // Skip the UTF-8 BOM if one is present
        char a,b,c;
        a = file.get();
        b = file.get();
        c = file.get();
        if(a!=(char)0xEF || b!=(char)0xBB || c!=(char)0xBF)
        {
            file.seekg(0);
        }
        else
        {
            Log::Print("Warning. File contains UTF-8 bit order mark:");
            Log::Print(path);
        }

        string line;
        while (getline(file, line))
        {
            // Strip trailing carriage returns
            line.erase(line.find_last_not_of("\n\r\t") + 1);
            mLines.push_back(UnicodeString::fromUTF8(StringPiece(line.c_str())));
        }
    }

    // Close the file
    //Log("Closing file: " + path);
    file.close();
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
