#include "ContentManager.h"

#include "json.h"
#include "FilePaths.h"

#include "Game.h"
using namespace ascii;


namespace
{
    const string GROUPS_DIRECTORY("content/groups/");
    const string IMAGE_DIRECTORY("content/images/");
    const string SOUND_DIRECTORY("content/sounds/");
    const string MUSIC_DIRECTORY("content/music/");
    const string SURFACE_DIRECTORY("content/surfaces/");
    const string STYLE_DIRECTORY("content/styles/");
}


ascii::ContentManager::ContentManager(Game* game)
    : mpGame(game), mpSoundManager(game->soundManager()),
    mpTextManager(game->textManager()) 
{
    mpStyleManager = new StyleManager();
    mpSurfaceManager = new SurfaceManager();
}

ascii::ContentManager::~ContentManager()
{
    delete mpStyleManager;
    delete mpSurfaceManager;
}

ascii::ImageCache* ascii::ContentManager::imageCache()
{
    return mpGame->graphics()->imageCache();
}

void ascii::ContentManager::RequireContentGroup(string groupFile, bool locked)
{
    // Load the group from its handle relative to the groups directory
    groupFile = FileAccessPath(GROUPS_DIRECTORY + groupFile);
    // Parse the group structure from the JSON
    ContentGroup group = ParseContentGroup(groupFile, locked);
    // Reference the group from now on by its asset name
    AssetName name = HandleToName(groupFile);
    // Mark that the group has just been added, so we can check if any of its
    // assets must be newly loaded
    mRequiredGroups[name] = group;
}

void ascii::ContentManager::RequireContentGroup(string groupName, Json::Value& groupJson, bool locked)
{
    // Parse a ContentGroup from the given JSON element
    ContentGroup group = ParseContentGroup(groupJson, locked);
    // Store the content group by the given name
    mRequiredGroups[groupName] = group;
}

void ascii::ContentManager::ReleaseContentGroup(string groupName, bool lockOverride)
{
    ContentGroup group = mContentGroups[groupName];

    if (group.locked && !lockOverride) return;

    // Mark that the content group is being removed, so we can check if any of
    // its assets must now be freed
    mReleasedGroups.push_back(group);
    // Remove the content group referenced by the given asset name from the map
    // of required groups
    mContentGroups.erase(groupName);
}

void ascii::ContentManager::ClearContent()
{
    vector<string> groupNames;
    for (auto it = mContentGroups.begin(); it != mContentGroups.end(); ++it)
    {
        groupNames.push_back(it->first);
    }
    for (auto it = groupNames.begin(); it != groupNames.end(); ++it)
    {
        ReleaseContentGroup(*it);
    }
}

void ascii::ContentManager::UpdateContent()
{
    // Gather the required content from every group that has already been
    // loaded
    ContentGroup oldContent;
    // Gather all content which should not be unloaded
    ContentGroup neededContent;

    for (auto it = mContentGroups.begin(); it != mContentGroups.end(); ++it)
    {
        oldContent += it->second;
        neededContent += it->second;
    }

    // Gather the required content from every group that has newly been added
    // and must be acquired if necessary
    ContentGroup requiredContent;
    for (auto it = mRequiredGroups.begin(); it != mRequiredGroups.end(); ++it)
    {
        requiredContent += it->second;

        // Add this content to the retained content group, to prevent any of it
        // from being released unnecessarily
        neededContent += it->second;
    }

    // Gather the content released by every group that has been newly removed
    // and must be unloaded if necessary
    ContentGroup releasedContent;
    for (auto it = mReleasedGroups.begin(); it != mReleasedGroups.end(); ++it)
    {
        releasedContent += *it;
    }

    // Free everything from released groups that is no longer referenced by
    // another group
    ProcessHandleList(releasedContent.images, neededContent.images, &ascii::ContentManager::FreeImage);
    ProcessHandleList(releasedContent.sounds, neededContent.sounds, &ascii::ContentManager::FreeSound);
    ProcessHandleList(releasedContent.soundGroups, neededContent.soundGroups,
                      &ascii::ContentManager::FreeSoundGroup);
    ProcessHandleList(releasedContent.tracks, neededContent.tracks, &ascii::ContentManager::FreeTrack);

    ProcessHandleList(releasedContent.surfaces, neededContent.surfaces,
                      &ascii::ContentManager::FreeSurface);

    ProcessHandleList(releasedContent.styles, neededContent.styles, &ascii::ContentManager::FreeStyle);
    ProcessHandleList(releasedContent.textFiles, neededContent.textFiles,
                      &ascii::ContentManager::FreeText);

    mReleasedGroups.clear();

    // Load everything that is referenced in a newly required group
    ProcessHandleList(requiredContent.images, oldContent.images, &ascii::ContentManager::LoadImage);
    ProcessHandleList(requiredContent.sounds, oldContent.sounds, &ascii::ContentManager::LoadSound);
    ProcessHandleList(requiredContent.soundGroups, oldContent.soundGroups,
                      &ascii::ContentManager::LoadSoundGroup);
    ProcessHandleList(requiredContent.tracks, oldContent.tracks, &ascii::ContentManager::LoadTrack);
    ProcessHandleList(requiredContent.surfaces, oldContent.surfaces,
                      &ascii::ContentManager::LoadSurface);
    ProcessHandleList(requiredContent.styles, oldContent.styles, &ascii::ContentManager::LoadStyle);
    ProcessHandleList(requiredContent.textFiles, oldContent.textFiles,
                      &ascii::ContentManager::LoadText);

    // Add every newly required group to the map of retained groups
    for (auto it = mRequiredGroups.begin(); it != mRequiredGroups.end(); ++it)
    {
        mContentGroups[it->first] = it->second;
    }

    mRequiredGroups.clear();

    // TODO debug print content still retained after updating
    ContentGroup stillRetainedContent;
    for (auto it = mContentGroups.begin(); it != mContentGroups.end(); ++it)
    {
        stillRetainedContent += it->second;
    }
}

void ascii::ContentManager::ProcessHandleList(HandleList& handleList,
        HandleList& retainedHandles, AssetProcessor process)
{
    // Check every handle in the given list
    for (auto it = handleList.begin(); it != handleList.end(); ++it)
    {
        // Search the complete required handle list, and process the asset if its
        // handle is not found
        auto beg = retainedHandles.begin();
        auto end = retainedHandles.end();

        if (find(beg, end, *it) == end)
        {
            (this->*process)(*it);
        }
    }
}

void ascii::ContentManager::LoadImage(Handle imageHandle)
{
    imageHandle = FileAccessPath(IMAGE_DIRECTORY + imageHandle);
    imageCache()->loadTexture(HandleToName(imageHandle), imageHandle.c_str()); 
}

void ascii::ContentManager::FreeImage(Handle imageHandle)
{
    imageCache()->freeTexture(HandleToName(imageHandle));
}

void ascii::ContentManager::LoadSound(Handle soundHandle)
{
    soundHandle = FileAccessPath(SOUND_DIRECTORY + soundHandle);
    mpSoundManager->loadSound(HandleToName(soundHandle), soundHandle.c_str());
}

void ascii::ContentManager::FreeSound(Handle soundHandle)
{
    soundHandle = SOUND_DIRECTORY + soundHandle;
    mpSoundManager->freeSound(HandleToName(soundHandle));
}

void ascii::ContentManager::LoadSoundGroup(Handle groupHandle)
{
    // Retrieve the name of the group and its enclosing directory.
    // Sound files will be loaded into the group from the enclosing directory
    // of the sound group JSON.
    string groupName = HandleToName(groupHandle);
    string groupDirectory = HandleDirectory(groupHandle);

    // Load the sound group as JSON
    groupHandle = FileAccessPath(SOUND_DIRECTORY + groupHandle);
    Json::Value* groupJsonPtr = Json::Load(groupHandle);
    Json::Value groupJson = *groupJsonPtr;

    // Retrieve the list element "sounds"
    Json::Value sounds = groupJson["sounds"];

    // Iterate through all the sound handles defined in the "sounds" array
    // and load them into the group
    Json::ArrayIndex groupSize = sounds.size();
    for (Json::ArrayIndex idx = 0; idx < groupSize; ++idx)
    {
        Json::Value soundElement = sounds[idx];
        Handle soundHandle = FileAccessPath(SOUND_DIRECTORY + groupDirectory + soundElement.asString());

        mpSoundManager->loadGroupSound(groupName, soundHandle.c_str());
    }

    delete groupJsonPtr;
}

void ascii::ContentManager::FreeSoundGroup(Handle groupHandle)
{
    mpSoundManager->freeSoundGroup(HandleToName(groupHandle));
}

void ascii::ContentManager::LoadTrack(Handle trackHandle)
{
    trackHandle = FileAccessPath(MUSIC_DIRECTORY + trackHandle);
    mpSoundManager->loadTrack(HandleToName(trackHandle), trackHandle.c_str());
}

void ascii::ContentManager::FreeTrack(Handle trackHandle)
{
    mpSoundManager->freeTrack(HandleToName(trackHandle));
}

void ascii::ContentManager::LoadSurface(Handle surfaceHandle)
{
    surfaceHandle = FileAccessPath(SURFACE_DIRECTORY + surfaceHandle);
    Log::Print(surfaceHandle);
    mpSurfaceManager->LoadSurface(HandleToName(surfaceHandle), surfaceHandle);
}

void ascii::ContentManager::FreeSurface(Handle surfaceHandle)
{
    mpSurfaceManager->FreeSurface(HandleToName(surfaceHandle));
}

void ascii::ContentManager::LoadStyle(Handle styleHandle)
{
    styleHandle = FileAccessPath(STYLE_DIRECTORY + styleHandle);
    mpStyleManager->LoadStyle(HandleToName(styleHandle), styleHandle);
}

void ascii::ContentManager::FreeStyle(Handle styleHandle)
{
    mpStyleManager->FreeStyle(HandleToName(styleHandle));
}

void ascii::ContentManager::LoadText(Handle textHandle)
{
    mpTextManager->LoadFile(textHandle);
}

void ascii::ContentManager::FreeText(Handle textHandle)
{
    mpTextManager->UnloadFile(textHandle);
}

