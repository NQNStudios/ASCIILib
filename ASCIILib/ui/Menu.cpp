#include "Menu.h"

#include <sstream>
using namespace std;

#include "SDL.h"

#include "Log.h"
using namespace ascii;

#include "TextManager.h"

Menu::Menu(Whisperer* game)
    : mpGame(game), mSelectedIndex(0),
    mLastSelectedIndex(0), mpNextState(NULL), mUsingMouse(false),
    mpSelectUpAction(game->inputMappings()->GetAction("Up")),
    mpSelectDownAction(game->inputMappings()->GetAction("Down")),
    mpTriggerSelectionAction(game->inputMappings()->GetAction("Continue/Select")),
    mFirstFrame(true)
{
}

void Menu::SetSelectedIndex(int index)
{
    if (index > mButtons.size() - 1 || index < 0)
    {
        stringstream error;
        error << "Tried to set a menu index out of range. Index: ";
        error << index;
        Log::Error(error.str());
        return;
    }

    mSelectedIndex = index;
}

Button* Menu::ButtonForKey(string actionKey, int* outIndex)
{
    for (int i = 0; i < mButtons.size(); ++i)
    {
        Button* button = &mButtons[i];
        
        if (!button->ActionKey().compare(actionKey))
        {
            // If an output variable adress was specified to store the index of the
            // button, store the index in it
            if (outIndex)
            {
                *outIndex = i;
            }

            return button;
        }
    }

    if (outIndex)
    {
        *outIndex = -1;
    }

    Log::Print("Failed to find button with action key " + actionKey);

    return NULL;
}

Button* Menu::GetButtonForKey(string actionKey, int* outIndex)
{
    Button* button = ButtonForKey(actionKey, outIndex);

    if (button == NULL)
    {
        Log::Error("Tried to select nonexistent button by key: " + actionKey);
    }

    return button;
}

bool Menu::IsButtonForKey(string actionKey)
{
    return ButtonForKey(actionKey) != NULL; 
}


Label* Menu::LabelForKey(string textKey)
{
    for (int i = 0; i < mLabels.size(); ++i)
    {
        Label* label = &mLabels[i];

        if (!label->Text().compare(mpGame->textManager()->GetText(textKey)))
        {
            return label;
        }
    }

    Log::Error("Tried to retrieve nonexistent label by key: " + textKey);
    return NULL;
}

void Menu::SetSelectedIndexByKey(string actionKey)
{
    int i;
    ButtonForKey(actionKey, &i);
    SetSelectedIndex(i);
}

void Menu::SwitchSelectedIndexByKey(string currentActionKey, string newActionKey)
{
    int firstIndex;
    ButtonForKey(currentActionKey, &firstIndex);

    int newIndex;
    ButtonForKey(newActionKey, &newIndex);

    if (mSelectedIndex == firstIndex)
    {
        mSelectedIndex = newIndex;
    }
}

void Menu::SetButtonTextByKeys(string actionKey, string messageKey)
{
    Button* button = ButtonForKey(actionKey);
    button->SetText(mpGame->textManager()->GetText(messageKey));
}

void Menu::AppendButtonTextByKey(string actionKey, UnicodeString appendage)
{
    Button* button = ButtonForKey(actionKey);
    
    UnicodeString fullText = button->GetText();
    button->SetText(fullText + appendage);
}

void Menu::HandleInput(Input& input)
{
    // Don't handle any input if there are no buttons or action triggers
    if (mButtons.empty() && mActionTriggers.empty()) return;

    // Retrieve the screen coordinates of the mouse cursor
    int pixelMouseX = input.mouseX();
    int pixelMouseY = input.mouseY();

    // Convert the screen coordinates of the cursor into ASCII cell coordinates
    int cellMouseX = mpGame->graphics()->pixelToCellX(pixelMouseX);
    int cellMouseY = mpGame->graphics()->pixelToCellY(pixelMouseY);

    // Preserve the selected index from this frame so we can compare to
    // identify changes
    mLastSelectedIndex = mSelectedIndex;

    if (!mButtons.empty())
    {
        // Handle mouse input to select buttons, but only if the mouse moved in the
        // last frame or this is the menu's first frame
        if ((abs(input.mouseChangeX()) + abs(input.mouseChangeY()) > 0
                || (mFirstFrame && mpTriggerSelectionAction->IsMouseHeld(input))) && !mpGame->FirstInputFrame())
        {
            // The mouse now has menu control
            mUsingMouse = true;

            // Check each button to see if it's selected by the cursor
            int i = 0;
            for (auto it = mButtons.begin(); it != mButtons.end(); ++it, ++i)
            {
                Button button = *it;

                if (button.IsSelected(cellMouseX, cellMouseY))
                {
                    // Update the menu to select the button
                    mSelectedIndex = i;
                }
            }
        }
    }

    // Handle keyboard input to select buttons
    if (input.anyKeyPressed())
    {
        // The keyboard now has control of the menu
        mUsingMouse = false;

        // Use the arrow keys to change the selected index
        if (mpSelectUpAction->IsTriggered(input))
        {
            SelectUp();
        }
        else if (mpSelectDownAction->IsTriggered(input))
        {
            SelectDown();
        }
    }

    // Test whether the currently selected button must be triggered
    Button button = mButtons.at(mSelectedIndex);

    // Trigger the button currently selected by the mouse if it was pressed,
    // and if the mouse is active
    if (IsMouseOverSelectedButton(input))
    {
        // And if it was clicked with a mouse button mapped to
        // Continue/Select
        if (mpTriggerSelectionAction->IsMouseTriggered(input))
        {
            // Then trigger its action
            mpNextState = mpGame->PerformAction(button.ActionKey());
            return;
        }
    }
    // Trigger the button currently selected if a Continue/Select key was
    // pressed, and if the keyboard is active
    if (!mUsingMouse)
    {
        // If a Continue/Select key was pressed
        if (mpTriggerSelectionAction->IsKeyTriggered(input))
        {
            // Then trigger its action
            mpNextState = mpGame->PerformAction(button.ActionKey());
            return;
        }
    }

    // Trigger any InputActions whose inputs have been fired
    for (auto it = mActionTriggers.begin(); it != mActionTriggers.end(); ++it)
    {
        InputAction* action = it->first;

        if (action->IsTriggered(input))
        {
            mpNextState = mpGame->PerformAction(it->second);
            return;
        }
    }

    mFirstFrame = false;
}

void Menu::Draw(Graphics& graphics)
{
    // Make sure the menu background is opaque
    graphics.clearOpaque();

    // Clear the screen to background color
    graphics.fill(' ', Color::Black, mBackgroundColor);

    // Draw every label in the menu
    for (auto it = mLabels.begin(); it != mLabels.end(); ++it)
    {
        it->Draw(graphics);
    }

    // Draw every button in the menu
    for (int i = 0; i < mButtons.size(); ++i)
    {
        // Draw the currently selected button highlighted
        bool selected = (i == mSelectedIndex);

        mButtons.at(i).Draw(graphics, selected);
    }
}

int Menu::AddButton(Button button)
{
    // Check what index the button is added at
    int buttonIndex = mButtons.size();
    // Add the button
    mButtons.push_back(button);
    // Return the button's index
    return buttonIndex;
}

bool Menu::IsMouseOverSelectedButton(Input& input)
{
    // Retrieve the screen coordinates of the mouse cursor
    int pixelMouseX = input.mouseX();
    int pixelMouseY = input.mouseY();

    // Convert the screen coordinates of the cursor into ASCII cell coordinates
    int cellMouseX = mpGame->graphics()->pixelToCellX(pixelMouseX);
    int cellMouseY = mpGame->graphics()->pixelToCellY(pixelMouseY);

    Button button = mButtons[mSelectedIndex];

    // If the mouse is above the currently selected button
    return button.IsSelected(cellMouseX, cellMouseY);
}

void Menu::AddActionTrigger(InputAction* action, string actionKey)
{
    mActionTriggers[action] = actionKey;
}

void Menu::SelectDown()
{
    MoveSelectionDown();
    SeekDown();
}

void Menu::SelectUp()
{
    MoveSelectionUp();
    SeekUp();
}

void Menu::MoveSelectionDown()
{
    // Don't do this without buttons
    if (mButtons.empty()) return;

    // Move the selection to the next index up.
    ++mSelectedIndex;

    // Wrap to the top if down is pressed from the bottom button
    mSelectedIndex = mSelectedIndex % mButtons.size();
}

void Menu::MoveSelectionUp()
{
    // Don't do this without buttons
    if (mButtons.empty()) return;

    // Move selection to the next index up.
    --mSelectedIndex;

    // If the next index doesn't exist, wrap to the last index in the list
    if (mSelectedIndex < 0)
    {
        mSelectedIndex = mButtons.size() - 1;
    }
}

void Menu::SeekDown()
{
    // Don't do this without buttons
    if (mButtons.empty()) return;

    while (!mButtons.at(mSelectedIndex).KeyboardEnabled())
    {
        MoveSelectionDown();
    }
}

void Menu::SeekUp()
{
    // Don't do this without buttons
    if (mButtons.empty()) return;

    while (!mButtons.at(mSelectedIndex).KeyboardEnabled())
    {
        MoveSelectionUp();
    }
}

void Menu::SeekInitialSelection()
{
    SeekDown();

    mLastSelectedIndex = mSelectedIndex;
}
