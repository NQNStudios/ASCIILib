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
            template<typename T> static void Print(T message)
            {
                if (Enabled())
                {
                    cout << message << endl;
                }
            }

        private:
            static bool Enabled();
    };
}
