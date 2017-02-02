#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

#include "unicode/utypes.h"
#include "unicode/unistr.h"
using namespace icu;

#include "json.h"

#include "LanguageManager.h"

#include "content.h"


// Reads game text from the file system in the currently selected language.
class TextManager
{
    public:
        // Construct a TextManager by loading all available language packs
        TextManager(LanguageManager* languageManager);

        // Load desired text file in the proper language
        void LoadFile(Handle fileHandle);
        // Unload text from the desired text file
        void UnloadFile(Handle fileHandle);
        // Reload all files in the currently selected language
        void ReloadFiles();

        // Retrieve a string of text from a loaded text file
        UnicodeString GetText(string key);

        // Retrieve a random string of text from the currently loaded files,
        // with the requisite minimum length
        UnicodeString GetRandomText(int minLength);

    private:
        // text currently loaded by the manager, mapped by identifier
        map<string, UnicodeString> mText;
        // Files currently loaded by the manager and keys to text they own
        map<Handle, vector<string> > mFiles;
        vector<string> mMessageKeys;

        // Pointer to the game's language manager, for retrieving configuration
        // details about the current language
        LanguageManager* mpLanguageManager;
};
