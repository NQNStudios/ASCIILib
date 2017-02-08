#pragma once

#include <map>
#include <string>
using namespace std;

#include "Surface.h"
using namespace ascii;

namespace ascii
{


// Loads and frees ASCIILib surfaces
class SurfaceManager
{
    public:
        // Load a surface into memory with the given key
        void LoadSurface(string key, string surfaceFile);
        // Free the surface with the given key from memory
        void FreeSurface(string key);

        // Print the surfaces currently loaded by the manager
        void PrintContents();

        // Retrieve a surface from memory
        Surface* GetSurface(string key);
        // Create a new surface in memory
        Surface* CreateSurface(string key, int width, int height);

    private:
        map<string, Surface*> mSurfaces;
};

}
