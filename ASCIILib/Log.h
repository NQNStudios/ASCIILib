#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
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

            static void SetOutputFilename(string filename);

        private:
            static bool Enabled();
            static string sOutputFilename;
    };
}

#include "Log.tpp"
