#include "StyleManager.h"

#include <algorithm>

#include "Log.h"
using namespace ascii;


void ascii::StyleManager::LoadStyle(string key, string stylePath)
{
    mStyles[key] = DialogStyle::FromFile(stylePath);
}

void ascii::StyleManager::FreeStyle(string key)
{
    delete mStyles[key];
    mStyles.erase(key);
}

DialogStyle* ascii::StyleManager::GetStyle(string key)
{
    if (!mStyles[key])
    {
        Log::Error("Tried to reference nonexistent style: " + key);
    }

    return mStyles[key];
}
