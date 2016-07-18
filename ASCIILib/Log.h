#pragma once

#include <iostream>
using namespace std;

#include "unicode/ustream.h"
#include "unicode/unistr.h"
using namespace icu;


namespace ascii
{
    // Handles log output, respecting the runtime log argument
    class Log
    {
        public:
            template<typename T> static void Print(T message, bool newLine=true);
            template<typename T> static void Error(T errorMessage);

            static void SDLError();

        private:
            static bool Enabled();
    };
}

#include "Log.tpp"
