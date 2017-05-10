#pragma once 

#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

#include "json.h"
#include "FilePaths.h"

#include "Log.h"
#include "FileReader.h"
using namespace ascii;


/***********************
 * Global Json helpers *
 ***********************/

namespace Json
{
    // Parse a file for a root JSON object
    inline Json::Value* Load(string path)
    {
        FileReader file(path, FileAccessPath("content/text/forbidden-characters.txt"));

        string fileJson = file.FullContents();

        // Parse the text file as JSON
        Json::Value* root = new Json::Value();
        Json::Reader reader;

        bool success = reader.parse(fileJson, *root, false);

        if (!success)
        {
            Log::Error("Failed to parse text file as JSON: " + path);
            Log::Error(fileJson);
            Log::Error(reader.getFormattedErrorMessages());
        }

        return root;
    }

    // Write JSON data into a file
	inline void Write(Json::Value* json, string path)
	{
		// Open the file in which to write
		FILE* outputFile = fopen(path.c_str(), "w");

		if (outputFile)
		{
			// Write the given Json data into the file
			StyledWriter writer;
			string jsonToWrite = writer.write(*json);
			fprintf(outputFile, jsonToWrite.c_str());

			// Close the file, persisting changes
			fclose(outputFile);
		}
		else
		{
			Log::Error("Failed to open file for writing: " + path);
		}

    }

    // Check whether a string exists as a key in the given JSON value
    inline bool ElementExists(Json::Value elementJson, string elementKey)
    {
        Json::Value::Members elementMembers = elementJson.getMemberNames();

        // Search the members of the given JSON element for one using the
        // desired key
        return find(elementMembers.begin(), elementMembers.end(), elementKey)
            != elementMembers.end();
    }

    // Retrieve the JSON value associated with the given key, checking to make
    // sure the value exists
    inline Json::Value GetValue(Json::Value root, string elementKey)
    {
        if (!ElementExists(root, elementKey))
        {
            Log::Error("Tried to access nonexistent JSON value with key: " + elementKey);
            return Json::Value();
        }

        return root[elementKey];
    }

    // Retrieve a child string element of a JSON object
    inline string GetString(Json::Value root, string elementKey)
    {
        return GetValue(root, elementKey).asString();
    }

    // Retrieve a JSON value as a UString
    inline UnicodeString GetUString(Json::Value root)
    {
        string temp = root.asString();
        return UnicodeString::fromUTF8(StringPiece(temp.c_str()));
    }

    // Retrieve a child UnicodeString element of a JSON object
    inline UnicodeString GetUString(Json::Value root, string elementKey)
    {
        string temp = GetValue(root, elementKey).asString();
        return UnicodeString::fromUTF8(StringPiece(temp.c_str()));
    }

    // Retrieve a child boolean element of a JSON object
    inline bool GetBool(Json::Value root, string elementKey)
    {
        return GetValue(root, elementKey).asBool();
    }

    // Retrieve a child integer element of a JSON object
    inline int GetInt(Json::Value root, string elementKey)
    {
        return GetValue(root, elementKey).asInt();
    }

    // Retrieve a child float element of a JSON object
    inline float GetFloat(Json::Value root, string elementKey)
    {
        return GetValue(root, elementKey).asFloat();
    }

    // For every value that exists in theirs but not mine, copy theirs
    inline void CopyMissingValues(Json::Value* mine, Json::Value* theirs)
    {
        Json::Value::Members theirMembers = theirs->getMemberNames();

        for (int idx = 0; idx < theirMembers.size(); ++idx)
        {
            string key = theirMembers[idx];
            if (!ElementExists(*mine, key))
            {
                (*mine)[key] = (*theirs)[key];
            }
        }
    }
}
