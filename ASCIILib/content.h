/* This header file contains data structures for convent management of game
 * assets, along with operators for their manipulation */

/* TERMINOLOGY
 * -----------
 *
 * As a case study we will refer to the hypothetical sound file of Wilmarth's
 * journal being opened, located at the following location:
 *
 * content/
 * | sounds/
 * |  journal/
 * |   bookopen.wav
 *
 *  Handle - a path to an asset file, including file extension but relative
 *  only to the proper content directory of the asset. The handle of the
 *  example sound file would be "journal/bookopen.wav", which is implicitly 
 *  relative to "content/sounds" but preserves the extra enclosing directory
 *  "journal/"
 *
 *  HandleList - a list of asset handles described above
 *
 *  AssetName - The name used to reference an asset in a content manager. This
 *  is formed by stripped a Handle of its enclosing directory and file
 *  extension. The asset name of the example sound file would be "bookopen",
 *  and this key would be used to reference the sound file in the SoundManager
 *
 *  ContentGroup - data structure containing a HandleList for every unique type
 *  of asset to be loaded as part of a group. These are defined in JSON files,
 *  and are used in order to group related assets for easy loading and release
 *  by the ContentManager class
 */

#pragma once

#include <vector>
#include <algorithm>
#include <cstddef>
#include <string>
using namespace std;

#include "json/json.h"

#include "json.h"

namespace ascii
{
    

class ContentManager;

// An asset handle (typedef used for code clarity when working with both
// handles and names)
typedef string Handle;

// An asset name (typedef used for code clarity when when working with both
// handles and names)
typedef string AssetName;

// A list containing asset handles
typedef vector<Handle> HandleList;

// Convert an asset Handle into an AssetName to be used as a key in
// a sub-content manager such as SoundManager or ImageCache
AssetName HandleToName(Handle handle);

// Retrieve the enclosing directory of an asset handle
string HandleDirectory(Handle handle);

// Defines a set of content to be managed. Each vector contains a list of
// asset handles
struct ContentGroup
{
    public:
        HandleList images;
        HandleList sounds;
        HandleList soundGroups;
        HandleList tracks;
        HandleList surfaces;
        HandleList styles;
        HandleList textFiles;

        bool locked;
};

// Print every handle in a list with a name
void PrintHandleList(HandleList list, string listName);

// Print the handle of every piece of content in a group
void PrintContentGroup(ContentGroup group);

// Parse a HandleList from a JSON array in the given ContentGroup json
HandleList ParseHandleList(Json::Value& groupJson, string assetTypeKey);

// Parse a ContentGroup from the root element of a JSON file
ContentGroup ParseContentGroup(string jsonPath, bool locked);

// Parse a ContentGroup from an element of a JSON file
ContentGroup ParseContentGroup(Json::Value& groupJson, bool locked);

// Unary predicate struct for checking if a handle is duplicated between two
// HandleLists
struct IsDuplicate
{
    // Store the first list
    const HandleList firstCollection;
    IsDuplicate(HandleList first)
        : firstCollection(first) { }
    // Find duplicates by searching first list for element
    bool operator()(Handle toCheck)
    {
        return find(firstCollection.begin(), firstCollection.end(), toCheck)
            != firstCollection.end();
    }
};

// Combine two handle lists without duplicates
HandleList& operator+=(HandleList& a, HandleList& b);

// Combine two content groups without duplicate asset handles
ContentGroup& operator+=(ContentGroup& a, ContentGroup& b);

// Memory address of a single-argument function that processes the asset
// associated with a given handle
typedef void (ascii::ContentManager::*AssetProcessor)(Handle);
    
}
