#include "InputAction.h"

#include <algorithm>

#include "Log.h"


ascii::InputAction::InputAction(vector<SDL_Keycode> keys,
        vector<MouseButton> mouseButtons)
    : mKeys(keys), mMouseButtons(mouseButtons), mEnabled(true),
    mFalsePositive(false)
{
}

bool ascii::InputAction::IsTriggered(Input& input, bool counts)
{
    if (counts && CheckFalsePositive()) return true;
    if (!mEnabled) return false;

    if (counts)
        mCheckedThisFrame = true;
    return IsKeyTriggered(input) || IsMouseTriggered(input);
}

bool ascii::InputAction::IsHeld(Input& input, bool counts)
{
    if (counts && CheckFalsePositive()) return true;
    if (!mEnabled) return false;

    if (counts)
        mCheckedThisFrame = true;
    return IsKeyHeld(input) || IsMouseHeld(input);
}

bool ascii::InputAction::IsKeyTriggered(Input& input, bool counts)
{
    if (counts && CheckFalsePositive()) return true;
    if (!mEnabled) return false;

    if (counts)
        mCheckedThisFrame = true;
    // Check every key we're interested in
    for (auto it = mKeys.begin(); it != mKeys.end(); ++it)
    {
        if (input.wasKeyPressed(*it))
        {
            return true;
        }
    }

    // If none were pressed 
    return false;
}

bool ascii::InputAction::IsMouseTriggered(Input& input, bool counts)
{
    if (counts && CheckFalsePositive()) return true;
    if (!mEnabled) return false;

    if (counts)
        mCheckedThisFrame = true;
    // Check every mouse button we're interested in
    for (auto it = mMouseButtons.begin(); it != mMouseButtons.end(); ++it)
    {
        if (input.mouseButtonClicked(*it))
        {
            return true;
        }
    }

    // If none were clicked
    return false;
}

bool ascii::InputAction::IsKeyHeld(Input& input, bool counts)
{
    if (counts && CheckFalsePositive()) return true;
    if (!mEnabled) return false;

    if (counts)
        mCheckedThisFrame = true;
    // Check every key we're interested in
    for (auto it = mKeys.begin(); it != mKeys.end(); ++it)
    {
        if (input.isKeyHeld(*it))
        {
            return true;
        }
    }

    // If none are held
    return false;
}

bool ascii::InputAction::IsMouseHeld(Input& input, bool counts)
{
    if (counts && CheckFalsePositive()) return true;
    if (!mEnabled) return false;

    if (counts)
        mCheckedThisFrame = true;
    // Check every mouse button we're interested in
    for (auto it = mMouseButtons.begin(); it != mMouseButtons.end(); ++it)
    {
        if (input.mouseButtonDown(*it))
        {
            return true;
        }
    }

    // If none are held
    return false;
}

void ascii::InputAction::RemoveKey(SDL_Keycode key)
{
    mKeys.erase(remove(mKeys.begin(), mKeys.end(), key));
}

void ascii::InputAction::RemoveMouseButton(MouseButton button)
{
    mMouseButtons.erase(remove(mMouseButtons.begin(), mMouseButtons.end(), button));
}

bool ascii::InputAction::ContainsKey(SDL_Keycode key)
{
    return find(mKeys.begin(), mKeys.end(), key) != mKeys.end();
}

bool ascii::InputAction::ContainsMouseButton(MouseButton button)
{
    return find(mMouseButtons.begin(), mMouseButtons.end(), button)
        != mMouseButtons.end();
}

Json::Value ascii::InputAction::AsJson()
{
    // Create an empty value to fill in
    Json::Value value;

    // Make a JSON array object for the keys
    Json::Value keyArray;
    for (int i = 0; i < mKeys.size(); ++i)
    {
        keyArray[i] = Json::Value((int)mKeys[i]);
    }

    // Make a JSON array object for the mouse buttons
    Json::Value mouseArray;
    for (int i = 0; i < mMouseButtons.size(); ++i)
    {
        mouseArray[i] = Json::Value(MouseButtonAsString(mMouseButtons[i]));
    }

    // Attach these both as children of the parent value
    value["keys"] = keyArray;
    value["mouse-buttons"] = mouseArray;

    // Return the constructed value
    return value;
}

string ascii::InputAction::MouseButtonAsString(MouseButton button)
{
    switch (button)
    {
        case LEFT:
            return "left";
        case RIGHT:
            return "right";
        case MIDDLE:
            return "middle";
    }
}
void ascii::InputAction::QueueFalsePositive()
{
    mFalsePositive = true;
    Log::Print("Queueing a false positive!");
}
bool ascii::InputAction::CheckFalsePositive()
{
    bool falsePositive = mFalsePositive;
    mFalsePositive = false;
    return falsePositive;
}
