#include "TextManager.h"

#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;

#include "Log.h"
using namespace ascii;

#include "json.h"
#include "FilePaths.h"

namespace
{
    const string TEXT_DIR("content/text/");
}

ascii::TextManager::TextManager(LanguageManager* languageManager)
    : mpLanguageManager(languageManager)
{
}

UnicodeString ascii::TextManager::GetText(string key)
{
    // If the desired message is not defined in this language pack, we have an
    // error
    if (mText.find(key) == mText.end())
    {
        // Write it to the console
        Log::Error("Tried to retrieve nonexistent paragraph: " + key);

        // Return a placeholder string indicating which key is missing
        string placeholder = "{{" + key + "}}";
        return UnicodeString(placeholder.c_str());
    }

    return mText[key];
}

bool ascii::TextManager::ContainsText(string key)
{
  return (mText.find(key) != mText.end());
}

UnicodeString ascii::TextManager::GetRandomText(int minLength)
{
    if (mText.empty())
    {
        Log::Error("Tried to retrieve random message when TextManager has no text.");
        return "";
    }

    while (true)
    {
        int randIndex = rand() % mMessageKeys.size();
        UnicodeString message = mText[mMessageKeys[randIndex]];
        if (message.length() >= minLength)
        {
            return message;
        }
    }
}

void ascii::TextManager::LoadFile(Handle fileHandle)
{
    // Construct the paths to the JSON files for text by searching the directory
    // for the currently selected language
    string textPath = TEXT_DIR + mpLanguageManager->CurrentPack().directory
        + "/" + fileHandle;

    textPath = FileAccessPath(textPath);

    Log::Print("Loading text file: " + textPath);

    // Parse JSON data from those paths
    Json::Value* textJson = Json::Load(textPath);

    // Save keys of every string we load, so we can unload the file later.
    vector<string> textKeys;

    // Process every message inside the text file
    Json::Value::Members textMemberNames = textJson->getMemberNames();
    for (auto it = textMemberNames.begin(); it != textMemberNames.end(); ++it)
    {
        // Extract the message
        string key = *it;

        //Log::Print("Retrieving message with key " + key);

        UnicodeString message = Json::GetUString(*textJson, key);

        // Put the message key in textKeys so the message remains associated
        // with this file
        textKeys.push_back(key);
        // Put the message key in a master list for retrieving random messages
        mMessageKeys.push_back(key);

        // Store all messages in the text map, trimmed to avoid forcing the
        // player to press enter twice if there is a trailing space
        this->mText[key] = message.trim();
    }

    // Save the list of keys from this file in a map, so we can unload them all
    // when the file is unloaded
    mFiles[fileHandle] = textKeys;

    // Clean up our loaded JSON
    delete textJson;
}

void ascii::TextManager::UnloadFile(Handle fileHandle)
{
    // Retrieve the list of keys owned by the file
    vector<string> textKeys = mFiles[fileHandle];

    // Erase the message associated with each key
    for (int i = 0; i < textKeys.size(); ++i)
    {
        mText.erase(textKeys[i]);
        // Remove the key from the master list for randomization
        mMessageKeys.erase(
                remove(mMessageKeys.begin(), mMessageKeys.end(), textKeys[i]));
    }

    // Erase the list of keys owned by the file
    mFiles.erase(fileHandle);
}

void ascii::TextManager::ReloadFiles()
{
    // Unload and reload every file that's currently loaded, to ensure all text
    // is loaded in the current language

    vector<Handle> fileHandles;

    for (auto it = mFiles.begin(); it != mFiles.end(); ++it)
    {
        Handle fileHandle = it->first;
        fileHandles.push_back(fileHandle);
    }

    for (auto it = fileHandles.begin(); it != fileHandles.end(); ++it)
    {
        UnloadFile(*it);
        LoadFile(*it);
    }
}
