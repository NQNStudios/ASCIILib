#include "LanguageManager.h"

#include "json-util.h"
#include "FilePaths.h"


// Language pack JSON parsing constants
namespace
{
    const string LANGUAGES_FILE("content/languages.json");
    const string LANGUAGE_KEY("language");
    const string AUTHOR_KEY("author");
    const string PRINT_MODE_KEY("print-mode");
    const string DIRECTORY_KEY("directory");
    const string PAUSE_CHARACTERS_KEY("pause-characters");
    const string IGNORE_CHARACTERS_KEY("ignore-characters");
    const string PAUSE_WORDS_KEY("pause-words");
    const string IGNORE_WORDS_KEY("ignore-words");
}


ascii::LanguageManager::LanguageManager()
    : mSelectedPackIndex(0)
{
    // Load the game's available language configurations
    Json::Value* rootPtr = Json::Load(FileAccessPath(LANGUAGES_FILE));
    Json::Value root = *rootPtr;

    // Parse each element as its own language pack
    for (Json::ArrayIndex i = 0; i < root.size(); ++i)
    {
        Json::Value packElement = root[i];

        LoadPack(packElement);
    }

    delete rootPtr;
}

LanguagePack ascii::LanguageManager::CurrentPack()
{
    return mLanguagePacks[mSelectedPackIndex];
}

LanguagePack ascii::LanguageManager::GetPack(int index)
{
    return mLanguagePacks[index];
}

void ascii::LanguageManager::LoadPack(Json::Value jsonElement)
{
    // Make an empty pack
    LanguagePack pack;

    // Set the pack information by parsing from JSON
    pack.language = Json::GetUString(jsonElement, LANGUAGE_KEY);
    Log::Print(pack.language);
    pack.author = Json::GetUString(jsonElement, AUTHOR_KEY);
    pack.mode = ParsePrintMode(Json::GetString(jsonElement, PRINT_MODE_KEY));
    pack.directory = Json::GetString(jsonElement, DIRECTORY_KEY);

    // Parse words and characters for identifying internal breaks in dialogue
    if (Json::ElementExists(jsonElement, PAUSE_CHARACTERS_KEY))
    {
        pack.pauseCharacters = Json::GetUString(jsonElement, PAUSE_CHARACTERS_KEY);
    }
    if (Json::ElementExists(jsonElement, IGNORE_CHARACTERS_KEY))
    {
        pack.ignoreCharacters = Json::GetUString(jsonElement, IGNORE_CHARACTERS_KEY);
    }
    if (Json::ElementExists(jsonElement, PAUSE_WORDS_KEY))
    {
        Json::Value pauseWordsList = jsonElement[PAUSE_WORDS_KEY];

        for (int i = 0; i < pauseWordsList.size(); ++i)
        {
            pack.pauseWords.push_back(Json::GetUString(pauseWordsList[i]));
        }
    }
    if (Json::ElementExists(jsonElement, IGNORE_WORDS_KEY))
    {
        Json::Value ignoreWordsList = jsonElement[IGNORE_WORDS_KEY];

        for (int i = 0; i < ignoreWordsList.size(); ++i)
        {
            pack.ignoreWords.push_back(Json::GetUString(ignoreWordsList[i]));
        }
    }

    // Store the pack
    mLanguagePacks.push_back(pack);
}

bool ascii::LanguageManager::PauseAfterToken(UnicodeString token)
{
    // Don't process the trailing space
    token.trim();

    // Always pause if the token is a match to a pause word
    if (ContainsWord(CurrentPack().pauseWords, token))
    {
        return true;
    }

    // Never pause if the token is a match to an ignore word
    if (ContainsWord(CurrentPack().ignoreWords, token))
    {
        return false;
    }

    // Retrieve the last character in the token that is not to be ignored
    UnicodeString ignoreCharacters = CurrentPack().ignoreCharacters;

    UChar lastChar = ' ';
    for (int i = token.length() - 1; i >= 0; --i)
    {
        if (ContainsLetter(ignoreCharacters, token[i]))
        {
            continue;
        }
        else
        {
            lastChar = token[i];
            break;
        }
    }

    // The final test is whether the final significant character is a pause character
    return ContainsLetter(CurrentPack().pauseCharacters, lastChar);
}

bool ascii::ContainsWord(const vector<UnicodeString>& list, const UnicodeString& word)
{
    return find(list.begin(), list.end(), word) != list.end();
}

bool ascii::ContainsLetter(const UnicodeString& list, UChar letter)
{
    for (int i = 0; i < list.length(); ++i)
    {
        if (letter == list[i])
        {
            return true;
        }
    }

    return false;
}


PrintMode ascii::LanguageManager::ParsePrintMode(string printMode)
{
    // mode codes
    map<string, PrintMode> printModes;

    printModes["lefttoright"] = LEFT_TO_RIGHT;
    printModes["righttoleft"] = RIGHT_TO_LEFT;

    return printModes[printMode];
}
