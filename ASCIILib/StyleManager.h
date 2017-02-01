#pragma once

#include <map>
#include <string>
using namespace std;

#include "DialogStyle.h"

// Loads and frees dialog styles
class StyleManager
{
    public:
        // Load a dialog style with a key from the given JSON file path
        void LoadStyle(string key, string stylePath);
        // Free the dialog style with the given key
        void FreeStyle(string key);
        
        // Retrieve the dialog style with the given key
        DialogStyle* GetStyle(string key);
    private:
        map<string, DialogStyle*> mStyles;
};
