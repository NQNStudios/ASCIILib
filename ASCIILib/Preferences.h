#pragma once

#include <string>
using namespace std;

#include "json/json.h"
using namespace Json;

#include "FileReader.h"
using namespace ascii;


// Manages game configuration which is saved in a JSON file. Attributes are
// indexed with string keys
class Preferences
{
    public:
        // Try to load a configuration file inside the game's proper file directry.
        // Create a default one based on an existing file if necessary
        Preferences(string filename, string defaultFilename);
        // Dispose of the currently stored JSON configuration
        ~Preferences();

        // Overwrite the config JSON file with the current configuration
        void WriteValues();
        // Create a brand-new default config file (Can also be used to restore
        // default configuration)
        void WriteDefaultValues();

        // Check whether a value exists in the configuration by the given key
        bool ValueExists(string key);
        // Get a configuration JSON element by the given key
        Json::Value GetValue(string key);
        // Get the member names of the root JSON element of these Preferences
        Json::Value::Members GetMemberNames() { return mRoot->getMemberNames(); }
        // Get a configuration integer by the given key
        int GetInt(string key);
        // Get a configuration float by the given key
        float GetFloat(string key);
        // Get a configuration boolean by the given key
        bool GetBool(string key);
        // Get a configuration string by the given key
        string GetString(string key);

        // Set a configuration value by the given key
        void SetValue(string key, Value value);
        // Set a configuration integer by the given key
        void SetInt(string key, int value);
        // Set a configuration float by the given key
        void SetFloat(string key, float value);
        // Set a configuration boolean by the given key
        void SetBool(string key, bool value);
        // Set a configuration string by the given key
        void SetString(string key, string value);

    private:
        // Load values from a Preferences JSON file
        void LoadValues(FileReader* file);

        string mFilename;
        string mDefaultFilename;

        Value* mRoot;
};
