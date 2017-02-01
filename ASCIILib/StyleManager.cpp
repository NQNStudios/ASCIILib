#include "StyleManager.h"

#include <algorithm>

#include "Log.h"
using namespace ascii;


void StyleManager::LoadStyle(string key, string stylePath)
{
    mStyles[key] = DialogStyle::FromFile(stylePath);
}

void StyleManager::FreeStyle(string key)
{
    delete mStyles[key];
    mStyles.erase(key);
}

DialogStyle* StyleManager::GetStyle(string key)
{
    if (!mStyles[key])
    {
        Log::Error("Tried to reference nonexistent style: " + key);
    }

    return mStyles[key];
}
