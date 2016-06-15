#pragma once

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

            // Retrieve the next token in the string
            UnicodeString NextToken(bool trimmed=true);
            // Check if more tokens are left to be retrieved
            bool HasNextToken();

            // Retrieve the rest of the buffer string
            UnicodeString BufferRemainder();

        private:
            UnicodeString mBuffer;
            int32_t mBufferPosition;
    };

    bool IsWhiteSpace(UChar uch);

}
