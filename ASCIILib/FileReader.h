#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <deque>
using namespace std;

#include "unicode/utypes.h"
#include "unicode/unistr.h"
using namespace icu;

namespace ascii
{

    // Reads a Unicode file line by line or all at once. Will print error messages
    // if a nonexistent file is opened, and will simplify file input code by
    // automatically closing the file read. Returns UTF-8 strings.
    class FileReader
    {
        public:
            // Construct a FileReader of the given filepath.
            FileReader(string path);

            // Check if the file was successfully opened
            inline bool Exists() { return mExists; }

            // Check if the file contains another line to read.
            bool HasNextLine();
            // Retrieve the next line from the file being read, UTF-8 encoded.
            string NextLine();
            // Retrieve the next line from the file being read as
            // a UnicodeString 
            UnicodeString NextLineUnicode();


            // Return a string containing all text in the file being read.
            string FullContents();

        private:
            deque<UnicodeString> mLines;
            bool mExists;
    };

}
