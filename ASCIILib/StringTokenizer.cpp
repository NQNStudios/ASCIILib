#include "StringTokenizer.h"

#include "unicode/locid.h"

#include "Log.h"
using namespace ascii;

    
ascii::StringTokenizer::StringTokenizer(UnicodeString buffer)
    : mBuffer(buffer), mBufferPosition(0)
{
}

UnicodeString ascii::StringTokenizer::NextToken(bool trimmed)
{
    int32_t start = mBufferPosition;

    while (mBufferPosition < mBuffer.length())
    {
        ++mBufferPosition;
        if (mBuffer[mBufferPosition] == UnicodeString(" ")[0])
        {
            ++mBufferPosition;
            break;
        }
    }

    int32_t end = mBufferPosition;

    // Retrieve the next section ending with a line break boundary
    UnicodeString token = mBuffer.tempSubStringBetween(start, end);

    // Discard any trailing whitespace if directed
    if (trimmed)
    {
        token.trim();
    }

    return token;
}

bool ascii::StringTokenizer::HasNextToken()
{
    return mBufferPosition != mBuffer.length();
}

UnicodeString ascii::StringTokenizer::BufferRemainder()
{
    return mBuffer.tempSubString(mBufferPosition);
}

bool ascii::IsWhiteSpace(UChar uch)
{
	return uch == UnicodeString(" ")[0];
}
