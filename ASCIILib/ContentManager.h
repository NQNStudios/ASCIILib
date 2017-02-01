#pragma once

#include <vector>
#include <algorithm>
using namespace std;

#include "ImageCache.h"
#include "SoundManager.h"
#include "SurfaceManager.h"
using namespace ascii;

#include "StyleManager.h"
#include "TextManager.h"

#include "content.h"
#include "json.h"

namespace ascii
{
    class Game;

    // This monster class manages EVERY form of game assets we use.
    // Never fear, though--it delegates this responsibility to a series of specific
    // content managers.

    // The essential feature afforded by ContentManager is only loading assets when
    // they are required, and only releasing them when they're not. This class
    // keeps memory usage to a constant minimum.
    class ContentManager
    {
        public:
            // Construct the ContentManager with all of its necessary sub-managers
            ContentManager(Game* game);
            // Delete all internally created sub managers
            ~ContentManager();

            // Require the content group defined at the given file handle
            void RequireContentGroup(string groupFile, bool locked=false);
            // Require a content group from any JSON element and give it a name
            void RequireContentGroup(string groupName, Json::Value& groupJson, bool locked = false);

            // Release the content group defined by the given asset name
            void ReleaseContentGroup(string groupName, bool lockOverride=false);

            // Release every content group currently loaded except ones that are
            // specified as locked
            void ClearContent();

            // Free resources that are no longer required, and load resources that
            // are now required
            void UpdateContent();

            SoundManager* soundManager() { return mpSoundManager; }
            SurfaceManager* surfaceManager() { return mpSurfaceManager; }
            StyleManager* styleManager() { return mpStyleManager; }
            TextManager* textManager() { return mpTextManager; }
            ImageCache* imageCache();
        private:
            // Process a handle list. If any handle from the first list is not
            // present in the second one, the AssetProcessor function will be
            // called with the handle as the only argument
            void ProcessHandleList(HandleList& handleList,
                    HandleList& retainedHandles, AssetProcessor process);

            // Asset loaders and unloaders
            void LoadImage(Handle imageHandle);
            void FreeImage(Handle imageHandle);
            void LoadSound(Handle soundHandle);
            void FreeSound(Handle soundHandle);
            void LoadSoundGroup(Handle groupHandle);
            void FreeSoundGroup(Handle groupHandle);
            void LoadTrack(Handle trackHandle);
            void FreeTrack(Handle trackHandle);
            void LoadSurface(Handle surfaceHandle);
            void FreeSurface(Handle surfaceHandle);
            void LoadStyle(Handle styleHandle);
            void FreeStyle(Handle styleHandle);
            void LoadText(Handle textHandle);
            void FreeText(Handle textHandle);

            // Sub-Managers
            Game* mpGame;
            SoundManager* mpSoundManager;
            SurfaceManager* mpSurfaceManager;
            StyleManager* mpStyleManager;
            TextManager* mpTextManager;

            map<string, ContentGroup> mContentGroups;
            map<string, ContentGroup> mRequiredGroups;
            vector<ContentGroup> mReleasedGroups;
    };
}
