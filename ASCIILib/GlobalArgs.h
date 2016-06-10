#pragma once

#include <vector>
#include <string>
using namespace std;

namespace ascii
{
    // Stores global arguments for the game being run with ASCIILib.
    class GlobalArgs
    {
        public:
            static void Add(string arg);
            static void AddList(vector<string> argList);

            static void Remove(string arg);
            static void Clear();

            static bool Enabled(string arg);

        private:
            static vector<string> args;
    };
}
