#pragma once

#include <vector>
using namespace std;

#include "unicode/utypes.h"
#include "unicode/unistr.h"
using namespace icu;

namespace ascii
{

    // Tokenizes a string based on the normal space character (' ')
    class StringTokenizer
    {
        public:
            // Construct a tokenizer for the given string
            StringTokenizer(UnicodeString buffer);
            StringTokenizer(UnicodeString buffer, UnicodeString delimeters);

            // Retrieve the next token in the string
            UnicodeString NextToken(bool trimmed=true);
            // Check if more tokens are left to be retrieved
            bool HasNextToken();

            // Retrieve the rest of the buffer string
            UnicodeString BufferRemainder();

            // Add a word to this Tokenizer so that, if it appears at any
            // location in the string being tokenized, it counts as a token
            // regardless of whether delimeters are present
            void AddSpecialToken(UnicodeString token);

        private:
            bool IsDelimeter(UChar uch);
            UnicodeString mBuffer;
            int32_t mBufferPosition;
            UnicodeString mDelimeters;

            vector<UnicodeString> mSpecialTokens;

            // Retrieve the position and length of the next special token in the
            // buffer. Return false if there are no more special tokens
            bool FindSpecialToken(int32_t* outPostition, int32_t* outLength);
    };

    bool IsWhiteSpace(UChar uch);

}
