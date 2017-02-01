#pragma once

#include <map>
#include <string>
using namespace std;

#include "unicode/unistr.h"
using namespace icu;

#include "SDL.h"

#include "../Input.h"

#include "../Preferences.h"
#include "InputAction.h"

namespace ascii
{
class Game;


// Stores all input mappings in a dictionary, each with a string key. Input
// mappings are parsed from config JSON
class InputMappings
{
    public:
        // Parse all input mappings from the given preferences object
        InputMappings(Preferences* preferences);
        ~InputMappings();

        static const int MAX_KEYS_PER_ACTION = 4;

        // Parse a MouseButton enum value from a string ("left", "right" or
        // "middle")
        static MouseButton ParseMouseButton(string mouseButtonName);
        // Construct a string containing the name of each key mapped to an
        // action
        static UnicodeString KeyNameList(vector<SDL_Keycode> keyList);
        // Construct a string containing the name of each mouse button mapped
        // to an action
        static UnicodeString MouseButtonNameList(Game* game,
                vector<MouseButton> mouseButtonList);

        // Write the currently configured mappings to the Preferences JSON file
        void WriteMappings();

        // Resets state for checking whether actions have been checked
        void Update();

        // Retrieve one of the InputActions from the dictionary
        InputAction* GetAction(string actionName);

        // Check whether any key was pressed excluding those that belong to the
        // given action
        bool AnyKeyPressed(Input& input, string actionName);

        // Check whether any key is held excluding those that belong to the
        // given action
        bool AnyKeyHeld(Input& input, string actionName);

        // Add a key to the list of keys to trigger the desired action.
        // If the key is already associated with a different action, remove it
        // from that action's mappings.
        //
        // Return an error code if the operation was unsuccessful. The following
        // situations return error codes:
        //
        //  * "MAX_REACHED"
        //      The action already has the maximum number of associated keys
        //  * "NO_KEYS_IN_ACTION"
        //      The key being added would have to be removed from an input action
        //      which would have no other keys remaining
        //  * "ALREADY_CONTAINS_KEY"
        //      The key being added is already part of the InputAction
        string AddKeyMapping(string actionName, SDL_Keycode key);

        // Clear all key mappings from an InputAction. This should only be used
        // as a temporary operation, because leaving an InputAction without any
        // key mappings might leave the game unplayable.
        void ClearKeyMappings(string actionName);

        // Set the input action which the given mouse button triggers
        void MapMouseButton(MouseButton button, string actionName);

        // Disable the given mouse button from triggering any input
        void UnmapMouseButton(MouseButton button);

    private:
        // Helper for AnyKeyHeld() and AnyKeyPressed()
        bool AnyKeyQualifies(vector<SDL_Keycode>, string actionName);

        map<string, InputAction*> mInputActions;
        map<string, InputAction*> mTemporaryInputActions;

        Preferences* mpPreferences;
};
    
}
