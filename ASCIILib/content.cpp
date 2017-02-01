#include "content.h"

#include "Log.h"
using namespace ascii;


// ContentGroup JSON formatting constants
namespace
{
    const string IMAGES_KEY("images");
    const string SOUNDS_KEY("sounds");
    const string SOUND_GROUPS_KEY("sound-groups");
    const string TRACKS_KEY("tracks");
    const string SURFACES_KEY("surfaces");
    const string STYLES_KEY("styles");
    const string TEXT_FILES_KEY("text-files");
}

namespace ascii
{

AssetName HandleToName(Handle handle)
{
    // Strip the file extension by searching for the last instance of a dot
    size_t dotIndex = handle.find_last_of(".");
    string handleWithoutExtension = handle.substr(0, dotIndex);

    // Strip the extra enclosing directories by running a looped search for '/'
    string handleWithoutDirectories = handleWithoutExtension;
    size_t slashIndex = handleWithoutDirectories.find("/");
    while (slashIndex != string::npos)
    {
        handleWithoutDirectories = handleWithoutDirectories.substr(slashIndex + 1);

        slashIndex = handleWithoutDirectories.find("/");
    }

    return handleWithoutDirectories;
}

string HandleDirectory(Handle handle)
{
    // Strip the filename by finding the last instance of '/'
    size_t slashIndex = handle.find_last_of("/");

    // Return an empty string if there is no enclosing directory
    if (slashIndex == string::npos)
    {
        return "";
    }

    string handleDirectory = handle.substr(0, slashIndex + 1);

    return handleDirectory;
}

void PrintHandleList(HandleList list, string listName)
{
    Log::Print(listName);
    for (int i = 0; i < list.size(); ++i)
    {
        Log::Print(" " + list[i]);
    }
}

void PrintContentGroup(ContentGroup group)
{
    PrintHandleList(group.images, "Images:");
    PrintHandleList(group.sounds, "Sounds:");
    PrintHandleList(group.soundGroups, "Sound Groups:");
    PrintHandleList(group.tracks, "Tracks:");
    PrintHandleList(group.surfaces, "Surfaces:");
    PrintHandleList(group.styles, "Styles:");
    PrintHandleList(group.textFiles, "Text Files:");
}

HandleList ParseHandleList(Json::Value& groupJson, string assetTypeKey)
{
    // Create an empty HandleList
    HandleList handleList;

    // Retrieve the member names of the group json for indexing
    Json::Value::Members groupMembers = groupJson.getMemberNames();
    // Retrieve the iterators for reuse while looping
    auto memBegin = groupMembers.begin();
    auto memEnd = groupMembers.end();


    // Check if the element exists, otherwise simply skip and return an empty
    // list
    if (find(memBegin, memEnd, assetTypeKey) != memEnd)
    {
        // We assume this value to be an array type
        Json::Value handleListJson = groupJson[assetTypeKey];

        // Loop through values of the asset handle array
        Json::ArrayIndex handleCount = handleListJson.size();
        for (Json::ArrayIndex index = 0; index < handleCount; ++index)
        {
            // Extract each handle from the array
            Json::Value assetHandle = handleListJson[index];
            // Add it to the HandleList
            handleList.push_back(assetHandle.asString());
        }
    }

    // Return the list, empty or otherwise
    return handleList;
}

ContentGroup ParseContentGroup(string jsonPath, bool locked)
{
    // Parse the JSON file
    Json::Value* groupJsonPtr = Json::Load(jsonPath);
    Json::Value groupJson = *groupJsonPtr;

    // Parse the group from the root element
    ContentGroup group = ParseContentGroup(groupJson, locked);

    // Clean up the parsed JSON
    delete groupJsonPtr;

    // Return our content group
    return group;
}

ContentGroup ParseContentGroup(Json::Value& groupJson, bool locked)
{
    // Parse file handles into every handle list of a new ContentGroup
    ContentGroup group = {
        ParseHandleList(groupJson, IMAGES_KEY),
        ParseHandleList(groupJson, SOUNDS_KEY),
        ParseHandleList(groupJson, SOUND_GROUPS_KEY),
        ParseHandleList(groupJson, TRACKS_KEY),
        ParseHandleList(groupJson, SURFACES_KEY),
        ParseHandleList(groupJson, STYLES_KEY),
        ParseHandleList(groupJson, TEXT_FILES_KEY),
        locked
    };

    return group;
}

HandleList& operator+=(HandleList& a, HandleList& b)
{
    // Add all elements from b to a that do not already exist in a
    remove_copy_if(b.begin(), b.end(), back_inserter(a), IsDuplicate(a));

    // Return the sum (now contained in a)
    return a;
}

ContentGroup& operator+=(ContentGroup& a, ContentGroup& b)
{
    // Combine every HandleList in both ContentGroups
    a.images += b.images;
    a.sounds += b.sounds;
    a.soundGroups += b.soundGroups;
    a.tracks += b.tracks;
    a.surfaces += b.surfaces;
    a.styles += b.styles;
    a.textFiles += b.textFiles;

    // Return a (the result, a combination of both ContentGroups without
    // duplicate asset handles)
    return a;
}

}
