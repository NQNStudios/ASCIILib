#include "InputMappings.h"

#include "Input.h"
using namespace ascii;

#include "Game.h"


InputMappings::InputMappings(Preferences* preferences)
    : mpPreferences(preferences)
{
    // Parse each InputAction defined in the JSON
    Json::Value::Members inputActionNames = preferences->GetMemberNames();
    for (auto it = inputActionNames.begin(); it != inputActionNames.end(); ++it)
    {
        string inputActionName = *it;

        // The "button-order" value is special, and not a real input mapping
        if (!inputActionName.compare("button-order")
                || !inputActionName.compare("mouse-mapping-order"))
            continue;

        Json::Value inputActionJson = preferences->GetValue(inputActionName);

        // Parse each keycode
        vector<SDL_Keycode> keys;
        Json::Value keyList = inputActionJson["keys"];
        for (Json::ArrayIndex i = 0; i < keyList.size(); ++i)
        {
            keys.push_back(keyList[i].asInt());
        }

        // Parse each mouse button
        vector<MouseButton> mouseButtons;
        Json::Value mouseButtonList = inputActionJson["mouse-buttons"];
        for (Json::ArrayIndex i = 0; i < mouseButtonList.size(); ++i)
        {
            MouseButton mouseButton =
                ParseMouseButton(mouseButtonList[i].asString());

            mouseButtons.push_back(mouseButton);
        }

        // Construct an InputAction object and map it
        mInputActions[inputActionName] = new InputAction(keys, mouseButtons);
    }
}

InputMappings::~InputMappings()
{
    for (auto it = mInputActions.begin(); it != mInputActions.end(); ++it)
    {
        delete it->second;
    }
}

void InputMappings::WriteMappings()
{
    // Generate a JSON value for each input action, then place it in the
    // Preferences object replacing the JSON value that was there first
    for (auto it = mInputActions.begin(); it != mInputActions.end(); ++it)
    {
        mpPreferences->SetValue(it->first, it->second->AsJson());
    }

    // Write any changes to the Preferences object
    mpPreferences->WriteValues();
}

void InputMappings::Update()
{
    for (auto it = mInputActions.begin(); it != mInputActions.end(); ++it)
    {
        it->second->Update();
    }
}

InputAction* InputMappings::GetAction(string actionName)
{
    return mInputActions[actionName];
}

bool InputMappings::AnyKeyQualifies(vector<SDL_Keycode> keys, string actionName)
{
    for (int i = 0; i < keys.size(); ++i)
    {
        SDL_Keycode key = keys[i];

        bool keyFailed = false;
        // Discard any keys pressed that belong to the given action
        for (auto it = mInputActions.begin(); it != mInputActions.end(); ++it)
        {
            string otherKey = it->first;
            InputAction* action = it->second;

            if (!otherKey.compare(actionName))
            {
                if (action->ContainsKey(key))
                {
                    keyFailed = true;
                    break;
                }
            }
        }

        // This key is not used by any other actions so it qualifies!
        if (!keyFailed) return true;
    }

    return false;
}

bool InputMappings::AnyKeyPressed(Input& input, string actionName = "")
{
    // Check every key currently being pressed to see if it qualifies
    vector<SDL_Keycode> pressedKeys = input.allPressedKeys();

    return AnyKeyQualifies(pressedKeys, actionName);
}

bool InputMappings::AnyKeyHeld(Input& input, string actionName="")
{
    // Check every key currently being held to see if it qualifies
    vector<SDL_Keycode> heldKeys = input.allHeldKeys();
    return AnyKeyQualifies(heldKeys, actionName);
}

string InputMappings::AddKeyMapping(string actionName, SDL_Keycode key)
{
    // Retrieve the InputAction to modify
    InputAction* action = mInputActions[actionName];

    // Throw an error if the action already contains the given key
    if (action->ContainsKey(key))
    {
        return "ALREADY_CONTAINS_KEY";
    }

    // Throw an error if the action already has the maximum number of keys
    if (action->KeyCount() >= MAX_KEYS_PER_ACTION)
    {
        return "MAX_REACHED";
    }

    // Check every other InputAction to see if the given key must be "stolen"
    // from it
    for (auto it = mInputActions.begin(); it != mInputActions.end(); ++it)
    {
        // Check every InputAction except for the one in question
        if (actionName.compare(it->first))
        {
            InputAction* otherAction = it->second;

            // If it contains the given key, we must remove it in order to add
            // it to the new action
            if (otherAction->ContainsKey(key))
            {
                // Throw an error if stealing this key would leave the other
                // InputAction without any key mappings
                if (otherAction->KeyCount() == 1)
                {
                    return "NO_KEYS_IN_ACTION";
                }
                // Otherwise we can safely steal it
                else
                {
                    otherAction->RemoveKey(key);
                    action->AddKey(key);

                    // No error
                    return "";
                }
            }
        }
    }

    // If no other InputAction has this key mapped to it, simply add it
    action->AddKey(key);

    // No error
    return "";
}

void InputMappings::ClearKeyMappings(string actionName)
{
    mInputActions[actionName]->ClearKeys();
}

MouseButton InputMappings::ParseMouseButton(string mouseButtonName)
{
    // Initialize the map used to parse mouse button enums
    map<string, MouseButton> mouseButtons;
    mouseButtons["left"] = LEFT;
    mouseButtons["right"] = RIGHT;
    mouseButtons["middle"] = MIDDLE;

    // Return the mouse button corresponding to the given key
    return mouseButtons[mouseButtonName];
}

UnicodeString InputMappings::KeyNameList(vector<SDL_Keycode> keyList)
{
    UnicodeString toReturn;

    // String them together separated by commas
    for (int i = 0; i < keyList.size(); ++i)
    {
        SDL_Keycode key = keyList[i];

        // Get the human-readable name for the key that's mapped
        string keyName = SDL_GetKeyName(key);

        // If there is no human-readable name, call it "Unknown"
        if (keyName.empty())
        {
            keyName = "UNKNOWN";
        }

        // Add it to the list string
        toReturn += UnicodeString(keyName.c_str());

        // If it's not the last key, append a comma and a space
        if (i < keyList.size() - 1)
        {
            toReturn += ", ";
        }
    }

    return toReturn;
}

UnicodeString InputMappings::MouseButtonNameList(Game* game,
        vector<MouseButton> mouseButtonList)
{
    UnicodeString toReturn = "";

    // String them together separated by commas
    for (int i = 0; i < mouseButtonList.size(); ++i)
    {
        MouseButton button = mouseButtonList[i];

        // Get the human-readable name for the button that's mapped
        UnicodeString buttonName;

        switch (button)
        {
            case LEFT:
                buttonName = game->textManager()->GetText("left-click");
                break;
            case RIGHT:
                buttonName = game->textManager()->GetText("right-click");
                break;
            case MIDDLE:
                buttonName = game->textManager()->GetText("middle-click");
                break;
        }

        // Add it to the list string
        toReturn += buttonName;

        // If it's not the last key, append a comma and a space
        if (i < mouseButtonList.size() - 1)
        {
            toReturn += ", ";
        }
    }

    return toReturn;
}

void InputMappings::MapMouseButton(MouseButton button, string actionName)
{
    // Find whichever input action the mouse button is already mapped to
    // and unmap it
    for (auto it = mInputActions.begin(); it != mInputActions.end(); ++it)
    {
        InputAction* action = it->second;

        if (action->ContainsMouseButton(button))
        {
            action->RemoveMouseButton(button);
        }
    }

    // Add this mouse button to the proper action
    mInputActions[actionName]->AddMouseButton(button);
}

void InputMappings::UnmapMouseButton(MouseButton button)
{
    // Find whichever input action the mouse button is already mapped to
    // and unmap it
    for (auto it = mInputActions.begin(); it != mInputActions.end(); ++it)
    {
        InputAction* action = it->second;

        if (action->ContainsMouseButton(button))
        {
            action->RemoveMouseButton(button);
        }
    }
}

const int InputMappings::MAX_KEYS_PER_ACTION;
