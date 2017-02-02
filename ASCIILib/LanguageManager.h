#pragma once

#include <vector>
#include <string>
using namespace std;

#include "unicode/unistr.h"
using namespace std;

#include "json.h"


// Defines two possible modes for printing text
enum PrintMode
{
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT
};

// Defines a language pack for the game
struct LanguagePack
{
    public:
        UnicodeString language;
        UnicodeString author;
        PrintMode mode;
        string directory;
        UnicodeString pauseCharacters;
        UnicodeString ignoreCharacters;
        vector<UnicodeString> pauseWords;
        vector<UnicodeString> ignoreWords;
};


// Manages the language in which the game is playing. Helps TextManager and
// VoiceManager find the proper content for the current language
class LanguageManager
{
    public:
        // Construct the LanguageManager by loading every available language
        // pack
        LanguageManager();

        // Number of available language packs
        int LanguagePacks() { return mLanguagePacks.size(); }
        // Configuration of the language pack currently selected
        LanguagePack CurrentPack();
        // Index of the currently selected language pack
        int CurrentPackIndex() { return mSelectedPackIndex; }
        // Get the language pack corresponding with the given index
        LanguagePack GetPack(int index);
        // Set the current language pack
        void SetPack(int index) { mSelectedPackIndex = index; }

        // Check whether the given token terminates with a character that
        // initiates a pause after revealing
        bool PauseAfterToken(UnicodeString token);

    private:
        // Load a language pack's configuration
        void LoadPack(Json::Value jsonElement);

        PrintMode ParsePrintMode(string printMode);

        vector<LanguagePack> mLanguagePacks;
        int mSelectedPackIndex;
};

// Check if a word is contained in a list of words
bool ContainsWord(const vector<UnicodeString>& list, const UnicodeString& word);
// Check if a letter is contained in a list of letters
bool ContainsLetter(const UnicodeString& list, UChar letter);
