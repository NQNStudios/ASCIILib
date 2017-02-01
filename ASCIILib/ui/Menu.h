#pragma once

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "unicode/utypes.h"

#include "../Color.h"
using namespace ascii;

#include "../json.h"

#include "../State.h"
#include "../Game.h"
using namespace ascii;

#include "../input/InputAction.h"

#include "Label.h"
#include "Button.h"


namespace ascii
{

// A game menu that accepts input through buttons and whose layout can be
// parsed from a JSON file
class Menu : public State
{
    public:
        // Construct an empty menu
        Menu(Game* game);

        void SetBackgroundColor(Color color) { mBackgroundColor = color; }

        // Add a label to the menu
        void AddLabel(Label label) { mLabels.push_back(label); }
        // Add a button to the menu, returning its button index
        int AddButton(Button button);
        // Add an action trigger based on an InputAction to the menu
        void AddActionTrigger(InputAction* action, string actionKey);

        // Select the first button in the list that is keyboard-selectable
        void SeekFirstButton() { SeekDown(); }

        // Get the index of a button by searching for its action key
        void GetIndexByKey(string actionKey);

        // Set the currently selected button, by its index
        void SetSelectedIndex(int index);
        // Set the currently selected button, by its action key
        void SetSelectedIndexByKey(string actionKey);
        // Set the currently selected button by its action key if and only if
        // the currently selected button corresponds to the first given action key
        void SwitchSelectedIndexByKey(string currentActionKey, string newActionKey);

        // Hide a button
        void HideButton(int index) { mButtons[index].Hide(); }
        // Show a button that is hidden
        void ShowButton(int index) { mButtons[index].Show(); }
        // Set the text of a button in the menu

        void SetButtonText(int index, UnicodeString value)
        { mButtons[index].SetText(value); }

        void SetButtonTextByKeys(string actionKey, string messageKey);
        void AppendButtonTextByKey(string actionKey, UnicodeString appendage);

        virtual void Update(int deltaMS) { }
        virtual void HandleInput(Input& input);
        virtual void Draw(Graphics& graphics);

        bool IsFinished() { return mpNextState != NULL; }
        bool MustRefreshScreen() { return true; }
        State* NextState(Game* game) { return mpNextState; }

        // Find and select the first keyboard-enabled button by moving down and
        // wrapping if necessary. Includes the originally selected button.
        void SeekDown();

        // Find and select the first keyboard-enabled button by moving up and
        // wrapping if necessary. Includes the originally selected button.
        void SeekUp();

        // Find and select the first keyboard-selectable button to finish
        // initializing the menu
        void SeekInitialSelection();

        // Check if a button is being hovered over by the mouse
        bool IsMouseOverSelectedButton(Input& input);

        string Filename;
    private:
        // Move the selection down to the next keyboard-enabled button,
        // wrapping to the top if necessary
        void SelectDown();

        // Move the selection up to the next keyboard-enabled button,
        // wrapping to the bottom if necessary
        void SelectUp();

        // Move the selection down to the next button, wrapping to the top if
        // necessary
        void MoveSelectionDown();

        // Move the selction up to the next button, wrapping to the bottom if
        // necessary
        void MoveSelectionUp();

        Color mBackgroundColor;

        vector<Label> mLabels;

        map<InputAction*, string> mActionTriggers;

        Button* ButtonForKey(string actionKey, int* outIndex=NULL);
    protected:
        Game* mpGame;
        State* mpNextState;

        Label* LabelForKey(string textKey);

        bool IsButtonForKey(string actionKey);
        Button* GetButtonForKey(string actionKey, int* outIndex=NULL);

        vector<Button> mButtons;
        InputAction* mpSelectDownAction;
        InputAction* mpSelectUpAction;
        InputAction* mpTriggerSelectionAction;

        bool mUsingMouse;
        bool mFirstFrame;

        int mSelectedIndex;
        int mLastSelectedIndex;

        string mBackgroundTrack;
};
    
}
