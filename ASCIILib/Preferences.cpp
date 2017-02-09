#include "Preferences.h"

#include <fstream>
#include <algorithm>

#include "json-util.h"
#include "FilePaths.h"

ascii::Preferences::Preferences(string filename, string defaultFilename)
    : mDefaultFilename(defaultFilename)
{
    filename = FileDirectory() + filename;
    mFilename = filename;
    Log::Print("Creating preferences with file path: " + filename);

    // Check if a user config file has already been created
    FileReader file(filename);
    bool existingConfig = file.Exists();

#ifdef DEMO
    // If this is a demo build, always use a default file
    existingConfig = false;
#endif

    // Either parse the existing config file
    if (existingConfig)
    {
        mRoot = Json::Load(filename);

        // After parsing the existing config file, make sure it contains every
        // key in the default
        Json::Value* defaultValues = Json::Load(defaultFilename);

        CopyMissingValues(mRoot, defaultValues);
        delete defaultValues;

        WriteValues();
    }
    // Or make a default one
    else
    {
        WriteDefaultValues();
    }
}

ascii::Preferences::~Preferences()
{
    delete mRoot;
}

void ascii::Preferences::WriteValues()
{
    Log::Print("Writing Preferences values to path: " + mFilename);
    // Write out the currently stored version of the JSON configuration
    Json::Write(mRoot, mFilename);
}

void ascii::Preferences::WriteDefaultValues()
{
    mRoot = Json::Load(FileAccessPath(mDefaultFilename));
    WriteValues();
}

bool ascii::Preferences::ValueExists(string key)
{
    return Json::ElementExists(*mRoot, key);
}

Json::Value ascii::Preferences::GetValue(string key)
{
    return Json::GetValue(*mRoot, key);
}

int ascii::Preferences::GetInt(string key)
{
    return Json::GetInt(*mRoot, key);
}

float ascii::Preferences::GetFloat(string key)
{
    return Json::GetFloat(*mRoot, key);
}

bool ascii::Preferences::GetBool(string key)
{
    return Json::GetBool(*mRoot, key);
}

string ascii::Preferences::GetString(string key)
{
    return Json::GetString(*mRoot, key);
}

void ascii::Preferences::SetValue(string key, Value value)
{
    if (ValueExists(key))
    {
        // Swap the currently stored value of the given setting for a new one
        // we create on the stack
        (*mRoot)[key].swap(value);
    }
    else
    {
        // Add the new value for the given setting
        (*mRoot)[key] = value;
    }
}

void ascii::Preferences::SetInt(string key, int value)
{
    // Create the newly desired JSON value
    Value newValue(value);
    SetValue(key, newValue);
}

void ascii::Preferences::SetFloat(string key, float value)
{
    // Create the newly desired JSON value
    Value newValue(value);
    SetValue(key, newValue);
}

void ascii::Preferences::SetBool(string key, bool value)
{
    // Create the newly desired JSON value
    Value newValue(value);
    SetValue(key, newValue);
}

void ascii::Preferences::SetString(string key, string value)
{
    // Create the newly desired JSON value
    Value newValue(value);
    SetValue(key, newValue);
}
