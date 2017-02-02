#pragma once

#include <string>
#include <map>
using namespace std;

#include "unicode/utypes.h"

#include "SDL.h"

#include "Color.h"
#include "Point.h"
using namespace ascii;

#include "json.h"

#include "json-util.h"
#include "Preferences.h"

#include "content.h"
#include "TextManager.h"
#include "Game.h"

#include "Label.h"
#include "Button.h"
#include "Menu.h"
#include "Alignment.h"


namespace
{
    const string MENU_DIR("content/menus/");

    const string LABELS_KEY("labels");
    const string BUTTONS_KEY("buttons");
    const string INPUT_ACTIONS_KEY("input-mappings");

    const string TEXT_KEY("text");
    const string POSITION_KEY("position");
    const string BOUNDS_KEY("bounds");
    const string ALIGNMENT_KEY("alignment");
    const string CENTER_ALIGNED("center");
    const string CONDITIONS_KEY("conditions");

    const string BACKGROUND_COLOR_KEY("background-color");

    const string TEXT_COLOR_KEY("text-color");
    const string TEXT_COLOR_SELECTED_KEY("text-color-selected");

    const string BOX_COLOR_KEY("box-color");
    const string BOX_COLOR_SELECTED_KEY("box-color-selected");

    const string ACTION_KEY("action");
    const string KEYBOARD_ENABLED_KEY("keyboard-enabled");
}


namespace ascii
{

// Check if every condition defined by a parseable element is met
inline bool AllConditionsMet(Json::Value elementJson, Game* game)
{
    // A "conditions" child must be defined in order to check any conditions
    if (Json::ElementExists(elementJson, CONDITIONS_KEY))
    {
        Json::Value conditionsList = Json::GetValue(elementJson, CONDITIONS_KEY);

        // Iterate through the list of necessary conditions
        for (int i = 0; i < conditionsList.size(); ++i)
        {
            string condition = conditionsList[i].asString();

            // Conditions starting with '!' are negated 
            bool negated = false;

            if (condition[0] == '!')
            {
                negated = true;
                //Log::Print("Negating position ", false);
                //Log::Print(condition);
                condition = condition.substr(1);
                //Log::Print(condition);
            }

            // If any of them is not provided, or a negated condition IS
            // provied, return false
            if (negated == game->ConditionIsActive(condition))
            {
                return false;
            }
        }
    }

    // Otherwise they all must be provided
    return true;
}

// Parse an integer from a parsed JSON value.
// The integer can either be represented directly, or by the key corresponding
// to a constant value defined in the menu json inside the member "values"
inline int ParseInt(Json::Value intJson, map<string, int> menuValues)
{
    if (intJson.isInt())
    {
        return intJson.asInt();
    }
    else if (intJson.isString())
    {
        string valueKey = intJson.asString();
        if (menuValues.find(valueKey) == menuValues.end())
        {
            Log::Error("Tried to retrieve nonexistent menu constant value " + valueKey);
        }
        return menuValues[valueKey];
    }
    else
    {
        Log::Error("Tried to parse an integer from JSON non-integer, non-string value");
        return -1;
    }
}

// Parse a point from a parsed JSON value
inline Point ParsePoint(Json::Value pointJson, map<string, int> menuValues)
{
    // Expected JSON syntax:
    // [ x, y ]

    return Point(
            ParseInt(pointJson[0], menuValues),
            ParseInt(pointJson[1], menuValues));
}

// Parse an alignment from a parsed JSON value
inline Alignment ParseAlignment(Json::Value alignmentJson)
{
    // Expected JSON syntax:
    // "[alignment]"
    //   Possible values: "left", "right", "center"

    map<string, Alignment> alignments;
    alignments["left"] = ALIGN_LEFT;
    alignments["right"] = ALIGN_RIGHT;
    alignments["center"] = ALIGN_CENTER;

    return alignments[alignmentJson.asString()];
}

// Parse a color from a parsed Json value
inline Color ParseColor(Json::Value colorJson)
{
    // Expected JSON syntax:
    // [ r, g, b ]

    return Color(colorJson[0].asInt(), colorJson[1].asInt(), colorJson[2].asInt());
}

// Retrieve the default background color for a menu from the config JSON file
inline Color DefaultBackgroundColor(Preferences* config)
{
    return ParseColor(config->GetValue("menu-background-color-default"));
}

// Retrieve the default label text color from the config JSON file
inline Color DefaultTextColor(Preferences* config)
{
    return ParseColor(config->GetValue("menu-text-color-default"));
}

// Retrieve the default selected label text color from the config JSON file
inline Color DefaultTextColorSelected(Preferences* config)
{
    return ParseColor(config->GetValue("menu-text-color-selected-default"));
}

// Retrieve the default button box color from the config JSON file
inline Color DefaultBoxColor(Preferences* config)
{
    return ParseColor(config->GetValue("menu-box-color-default"));
}

// Retrieve the default button box color from the config JSON file
inline Color DefaultBoxColorSelected(Preferences* config)
{
    return ParseColor(config->GetValue("menu-box-color-selected-default"));
}

// Retrieve the background color used for tutorials
inline Color TutorialBackgroundColor(Preferences* config)
{
    return ParseColor(config->GetValue("tutorial-background-color"));
}

// Retrieve the highlight color used for tutorials
inline Color HighlightColor(Preferences* config)
{
    return ParseColor(config->GetValue("highlight-color"));
}

// Retrieve the activated highlight color used for tutorials
inline Color HighlightColorActivated(Preferences* config)
{
    return ParseColor(config->GetValue("highlight-color-activated"));
}

// Parse a rectangle from a parsed JSON value
inline Rectangle ParseRectangle(Json::Value rectJson, map<string, int> menuValues)
{
    // Expected JSON syntax:
    // [ x, y, width, height ]
    
    return Rectangle(
            ParseInt(rectJson[0], menuValues),
            ParseInt(rectJson[1], menuValues),
            ParseInt(rectJson[2], menuValues),
            ParseInt(rectJson[3], menuValues));
}

// Retrieve the text associated with the key from the given JSON element
inline UnicodeString RetrieveText(Json::Value elementJson, Game* game)
{
    // Retrieve the message key of the element's assigned text
    string textKey = GetString(elementJson, TEXT_KEY);

    // If the key is blank, return a blank string
    if (textKey.empty())
    {
        return "";
    }

    // Return the message associated with the assigned message key, in the
    // proper language
    return game->textManager()->GetText(textKey);
}

// Retrieve the position of the given JSON element (either label or button)
inline Point RetrievePosition(Json::Value elementJson, map<string, int> menuValues)
{
    // Parse and return the point that defines the given element's position
    Point position = ParsePoint(elementJson[POSITION_KEY], menuValues);
    return position;
}

// Parse a label from the given JSON element
inline Label ParseLabel(Json::Value labelJson, Game* game,
        map<string, int> menuValues)
{
    /* Expected JSON syntax:
     *   {
     *      "text": "[text-manager-text-key]",
     *      "position": [ x, y ],
     *      "alignment": "center", // Optional, defaults to non-centered
     *      "text-color": [ r, g, b ] // Optional, default defined in config
     *          JSON
     *   }
     *
     * OR for a multiline label:
     *   {
     *      "text": "[text-manager-text-key]",
     *      "bounds": [ x, y, width, height ],
     *      "text-color": [ r, g, b ] // Optional, default defined in config
     *          JSON
     *   }
     */

    // Parse the key for the label's text
    UnicodeString text = RetrieveText(labelJson, game);

    // Parse the color of the label's text
    Color textColor = DefaultTextColor(game->config());
    if (ElementExists(labelJson, TEXT_COLOR_KEY))
        textColor = ParseColor(labelJson[TEXT_COLOR_KEY]);

    // Construct a single-line label if a simple position is defined
    if (ElementExists(labelJson, POSITION_KEY))
    {
        Point position = RetrievePosition(labelJson, menuValues);
        Alignment alignment = ParseAlignment(labelJson[ALIGNMENT_KEY]);
        return Label(text, position, alignment, textColor);
    }
    // Construct a multiline label if a bounding rectangle is defined
    else 
    {
        Rectangle bounds = ParseRectangle(labelJson[BOUNDS_KEY], menuValues);
        return Label(text, bounds, textColor);
    }
}

// Create a label using the default appearance. Specify the text key of the
// label's message, the label's position and the label's alignment
inline Label DefaultLabel(string textKey, Point position,
        Alignment alignment, Game* game)
{
    UnicodeString text = game->textManager()->GetText(textKey);

    return Label(text, position, alignment, DefaultTextColor(game->config()));
}

// Create a label using the default appearance. Specify the label's message
// directly.
inline Label DefaultLabelDirect(UnicodeString text, Point position,
        Alignment alignment, Game* game)
{
    return Label(text, position, alignment, DefaultTextColor(game->config()));
}

// Parse a button from the given JSON element
inline Button ParseButton(Json::Value buttonJson, Game* game,
        map<string, int> menuValues)
{
    /* Expected JSON syntax:
     *   {
     *      "text": "[text-manager-text-key]",
     *      "bounds": [ x, y, width, height ],
     *      "alignment": "center", // Optional, defaults to non-centered
     *      "box-color": [ r, g, b ], // Optional, default defined in config
     *                                // JSON
     *      "box-color-selected": [ r, g, b ], // Optional, default defined in
     *                                         // config json
     *
     *      "text-color": [ r, g, b ], 
     *      "text-color-selected": [ r, g, b ], // Optional, default defined in
     *                                          // config json
     *
     *      "action": "action-key", // Key for the action to perform when triggered
     *      "keyboard-enabled": [boolean] // Optional, defaults to true
     *   }
     */

    // Parse the key for the button's text
    UnicodeString text = RetrieveText(buttonJson, game);
    // Parse the bounding rectangle for the button
    Rectangle bounds = ParseRectangle(buttonJson[BOUNDS_KEY], menuValues);
    // Parse the alignment of the button
    Alignment alignment = ParseAlignment(buttonJson[ALIGNMENT_KEY]);

    // Parse the color of the button's bounding box, or use the default
    Color boxColor = DefaultBoxColor(game->config());
    if (ElementExists(buttonJson, BOX_COLOR_KEY))
        boxColor = ParseColor(buttonJson[BOX_COLOR_KEY]);

    // Parse the color of the button's bounding box when selected,
    // or use the default
    Color boxColorSelected = DefaultBoxColorSelected(game->config());
    if (ElementExists(buttonJson, BOX_COLOR_SELECTED_KEY))
        boxColorSelected = ParseColor(buttonJson[BOX_COLOR_SELECTED_KEY]);

    // Parse the color of the button's text, or use the default
    Color textColor = DefaultTextColor(game->config());
    if (ElementExists(buttonJson, TEXT_COLOR_KEY))
        textColor = ParseColor(buttonJson[TEXT_COLOR_KEY]);

    // Parse the color of the button's text when selected, or use the default
    Color textColorSelected = DefaultTextColorSelected(game->config());
    if (ElementExists(buttonJson, TEXT_COLOR_SELECTED_KEY))
        textColorSelected = ParseColor(buttonJson[TEXT_COLOR_SELECTED_KEY]);

    // Parse the key of the action performed when the button is selected
    string actionKey = GetString(buttonJson, ACTION_KEY);

    // Parse the whether the button can be selected by the keyboard, defaulting
    // to true
    bool keyboardEnabled = true;
    if (ElementExists(buttonJson, KEYBOARD_ENABLED_KEY))
        keyboardEnabled = buttonJson[KEYBOARD_ENABLED_KEY].asBool();

    // Construct the button
    return Button(text, bounds, alignment, textColor, textColorSelected,
            boxColor, boxColorSelected, actionKey, keyboardEnabled);
}

// Construct a button using the default appearance. Specify text directly (not
// by key) along with bounds, alignment, UIAction key, and keyboard
// selectability
inline Button DefaultButton(Preferences* config, UnicodeString text, Rectangle bounds,
        Alignment alignment, string actionKey, bool keyboardEnabled)
{
    return Button(text, bounds, alignment, DefaultTextColor(config),
            DefaultTextColorSelected(config), DefaultBoxColor(config),
            DefaultBoxColorSelected(config), actionKey,keyboardEnabled);
}

// Parse a menu from a file to fill the menu passed as a parameter. The menu
// passed can be an instance of a subclass of menu. Items defined in JSON will
// be added to the menu in the order presented. This means if you want buttons
// from the subclass to come first, they must be added before the menu is
// parsed. If you want them added after, they must be added after parsing.
inline void ParseMenu(Menu* menu, Json::Value* menuJsonPtr, Game* game)
{
    Json::Value menuJson = *menuJsonPtr;

    // Parse out menu's background color using a default if undefined
    Color backgroundColor = DefaultBackgroundColor(game->config());
    if (ElementExists(menuJson, BACKGROUND_COLOR_KEY))
        backgroundColor = ParseColor(menuJson[BACKGROUND_COLOR_KEY]);

    // Parse out menu's constant values
    map<string, int> menuValues;
    if (ElementExists(menuJson, "values"))
    {
        Json::Value valuesJson = menuJson["values"];

        // Read the values set for the currently selected language, or ALL
        string currentPackDirectory = game->languageManager()->CurrentPack().directory;
        if (ElementExists(valuesJson, currentPackDirectory))
        {
            valuesJson = valuesJson[currentPackDirectory];
        }
        else if (ElementExists(valuesJson, "ALL"))
        {
            valuesJson = valuesJson["ALL"];
        }

        Json::Value::Members valueMembers = valuesJson.getMemberNames();

        for (auto it = valueMembers.begin(); it != valueMembers.end(); ++it)
        {
            menuValues[*it] = valuesJson[*it].asInt();
        }
    }

    // Parse out all of the menu's labels from the label array
    if (ElementExists(menuJson, LABELS_KEY))
    {
        Json::Value labelsJson = menuJson[LABELS_KEY];

        for (Json::ArrayIndex i = 0; i < labelsJson.size(); ++i)
        {
            Json::Value labelJson = labelsJson[i];

            // Parse out a label for every member of "labels" that has its
            // conditions met
            if (AllConditionsMet(labelJson, game))
            {
                menu->AddLabel(ParseLabel(labelsJson[i], game, menuValues));
            }
        }
    }

    // Parse out all of the menu's buttons from the button array
    if (ElementExists(menuJson, BUTTONS_KEY))
    {
        Json::Value buttonsJson = menuJson[BUTTONS_KEY];

        for (Json::ArrayIndex i = 0; i < buttonsJson.size(); ++i)
        {
            Json::Value buttonJson = buttonsJson[i];

            // Parse out a button for every member of "buttons" that has its
            // conditions met
            if (AllConditionsMet(buttonJson, game))
            {
                menu->AddButton(ParseButton(buttonsJson[i], game, menuValues));
            }
        }
    }

    // Seek the first keyboard-selectable button in the menu
    menu->SeekInitialSelection();

    // Parse all of the menu's InputActions from the input action array
    if (ElementExists(menuJson, INPUT_ACTIONS_KEY))
    {
        Json::Value inputActionsJson = menuJson[INPUT_ACTIONS_KEY];

        for (Json::ArrayIndex i = 0; i < inputActionsJson.size(); ++i)
        {
            // Parse out an InputAction for every member of "input-actions"
            Json::Value actionJson = inputActionsJson[i];

            if (AllConditionsMet(actionJson, game))
            {
                string inputActionKey = GetString(actionJson, "input-action");
                string uiActionKey = GetString(actionJson, "ui-action");

                InputAction* action = game->inputMappings()->GetAction(inputActionKey);
                menu->AddActionTrigger(action, uiActionKey);
            }
        }
    }
}

// Parse a menu JSON file. All menu elements defined in the JSON will be added
// in order to the menu passed as a parameter
inline void ParseMenu(Menu* menu, Game* game, Handle fileHandle)
{
    // Parse out the entire JSON file
    Json::Value* menuJson = Json::Load(FileAccessPath(MENU_DIR + fileHandle));

    menu->Filename = fileHandle;

    // Pass this JSON to the other ParseMenu() function to create a menu
    ParseMenu(menu, menuJson, game);

    // Delete the JSON we loaded
    delete menuJson;
}
    
}
