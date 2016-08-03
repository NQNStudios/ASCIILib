#include "StringTokenizer.h"

#include "unicode/locid.h"

#include "Log.h"
using namespace ascii;

    
ascii::StringTokenizer::StringTokenizer(UnicodeString buffer)
    : mBuffer(buffer), mBufferPosition(0)
{
    mDelimeters = UnicodeString(" ");
}

ascii::StringTokenizer::StringTokenizer(UnicodeString buffer,
        UnicodeString delimeters)
    : mBuffer(buffer), mDelimeters(delimeters)
{
}

void ascii::StringTokenizer::AddSpecialToken(UnicodeString token)
{
    mSpecialTokens.push_back(token);
}

bool ascii::StringTokenizer::FindSpecialToken(int32_t* outPosition, int32_t* outLength)
{
    // Don't bother with this if no special tokens matter
    if (mSpecialTokens.empty())
    {
        *outPosition = -1;
        return false;
    }

    int32_t start = mBufferPosition;

    // Check each upcoming position in the string to see if it is the start of a
    // special token
    while (start < mBuffer.length())
    {
        // Check this position against each special token
        for (int i = 0; i < mSpecialTokens.size(); ++i)
        {
            UnicodeString specialToken = mSpecialTokens[i];

            // It's only possible if enough characters are left in the buffer
            if (start + specialToken.length() <= mBuffer.length())
            {
                UnicodeString specialTokenCandidate = mBuffer.tempSubStringBetween(start, start + specialToken.length());

                // If the character sequence beginning at this position matches
                // a special token, return this position
                if (!specialToken.compare(specialTokenCandidate))
                {
                    if (outPosition) *outPosition = start;
                    if (outLength) *outLength = specialToken.length();
                    return true;
                }
            }
        }

        ++start;
    }

    *outPosition = -1;
    return false;
}

UnicodeString ascii::StringTokenizer::NextToken(bool trimmed)
{
    int32_t start = mBufferPosition;

    // Get the position and length of the next special token, if there is one
    int nextSpecialPosition = 0;
    int nextSpecialLength = 0;

    // If there is no remaining special token, place its position out of reach
    // so as not to screw things up
    FindSpecialToken(&nextSpecialPosition, &nextSpecialLength);

    // If the current position is the start of a special token, return the
    // special token and move the buffer position forward
    if (mBufferPosition == nextSpecialPosition)
    {
        mBufferPosition += nextSpecialLength;

        // Special tokens are never trimmed
        UnicodeString token = mBuffer.tempSubStringBetween(start, mBufferPosition);

        Log::Print("Next token is a special token: " + token);
        return token;
    }

    // If the next token is not a special token, keep adding characters to the
    // current token until all of the buffer is used, or the next special token
    // begins
    while (mBufferPosition < mBuffer.length() && mBufferPosition != nextSpecialPosition)
    {
        ++mBufferPosition;
        if (IsDelimeter(mBuffer[mBufferPosition]))
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

bool ascii::StringTokenizer::IsDelimeter(UChar uch)
{
    for (int i = 0; i < mDelimeters.length(); ++i)
    {
        if (mDelimeters[i] == uch) return true;
    }
    return false;
}

bool ascii::IsWhiteSpace(UChar uch)
{
	return uch == UnicodeString(" ")[0];
}
