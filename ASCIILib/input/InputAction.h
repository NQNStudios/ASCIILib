#pragma once

#include <vector>
using namespace std;

#include "SDL.h"

#include "../Input.h"
using namespace ascii;

#include "../json/json.h"


// Holds a set of input mappings which will trigger an action. Checks these
// mappings against the current input frame, to decide whether the action fires.
class InputAction
{
    public:
        // Construct an InputAction with specifically desired mappings
        InputAction(vector<SDL_Keycode> keys, vector<MouseButton> mouseButtons);

        // Whether the InputAction is triggered
        bool IsTriggered(Input& input, bool counts=false);

        // Whether the InputAction is continuously held
        bool IsHeld(Input& input, bool counts=false);

        // Whether the InputAction is triggered by the keyboard specifically
        bool IsKeyTriggered(Input& input, bool counts=false);

        // Whether the InputAction is triggered by the mouse specifically
        bool IsMouseTriggered(Input& input, bool counts=false);

        // Whether the InputAction is continuously held by the keyboard specifically
        bool IsKeyHeld(Input& input, bool counts=false);

        // Whether the InputAction is continuously held by the mouse specifically
        bool IsMouseHeld(Input& input, bool counts=false);

        // Called every frame to reset state for WasChecked()
        void Update() { mCheckedThisFrame = false; }
        // Whether the InputAction's state has been read this frame
        bool WasChecked() { return mCheckedThisFrame; }

        // Clear the keys this action will respond to
        void ClearKeys() { mKeys.clear(); }
        // Clear the mouse buttons this action will respond to
        void ClearMouseButtons() { mMouseButtons.clear(); }


        // Add a key to this action's triggers
        void AddKey(SDL_Keycode key) { mKeys.push_back(key); }
        // Add a mouse button this action's triggers
        void AddMouseButton(MouseButton button) { mMouseButtons.push_back(button); }

        // Remove a key from this action's triggers
        void RemoveKey(SDL_Keycode key);
        // Remove a mouse button from this action's triggers
        void RemoveMouseButton(MouseButton button);

        // Check if this action responds to the given key
        bool ContainsKey(SDL_Keycode key);
        // Check if this action responds to the given mouse button
        bool ContainsMouseButton(MouseButton button);

        // Return the number of keys that will trigger this action
        int KeyCount() { return mKeys.size(); }
        // Return the number of mouse buttons that will trigger this action
        int MouseButtonCount() { return mMouseButtons.size(); }

        // Return the list of keys listened to
        vector<SDL_Keycode> KeyList() { return mKeys; }
        // Return the list of mouse buttons listened to
        vector<MouseButton> MouseButtonList() { return mMouseButtons; }

        // Return this InputAction expressed as a JSON object
        Json::Value AsJson();

        void SetEnabled(bool value) { mEnabled = value; }
        void QueueFalsePositive();

    private:
        vector<SDL_Keycode> mKeys;
        vector<MouseButton> mMouseButtons;

        string MouseButtonAsString(MouseButton button);

        bool mEnabled;
        bool mCheckedThisFrame;

        bool CheckFalsePositive();
        bool mFalsePositive;
};
