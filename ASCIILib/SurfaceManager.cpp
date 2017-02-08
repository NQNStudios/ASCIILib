#include "SurfaceManager.h"

#include "Log.h"
using namespace ascii;

#include "FilePaths.h"


void ascii::SurfaceManager::LoadSurface(string key, string surfaceFile)
{
    mSurfaces[key] = Surface::FromFile(surfaceFile.c_str());
}

Surface* ascii::SurfaceManager::CreateSurface(string key, int width, int height)
{
    mSurfaces[key] = new Surface(width, height);
    return mSurfaces[key];
}

void ascii::SurfaceManager::FreeSurface(string key)
{
    delete mSurfaces[key];
    mSurfaces.erase(key);
}

Surface* ascii::SurfaceManager::GetSurface(string key)
{
    if (mSurfaces.find(key) == mSurfaces.end())
    {
        Log::Error("Tried to retrieve nonexistent surface: " + key);
        return NULL;
    }

    return mSurfaces[key];
}

void ascii::SurfaceManager::PrintContents()
{
    Log::Print("Surface manager contents:");
    for (auto it = mSurfaces.begin(); it != mSurfaces.end(); ++it)
    {
        Log::Print(" " + it->first);
    }
}
