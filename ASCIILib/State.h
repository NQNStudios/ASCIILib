#pragma once

#include "Input.h"
#include "Graphics.h"

using namespace ascii;

namespace ascii
{
    class Game;

    // Interface for game states
    // Some states will contain other states. For example, a chapter state
    // will execute a script while pausing to wait for states invoked
    // by the script to finish their execution.
    class State
    {
        public:
            // Virtual destructor to avoid memory leakage
            virtual ~State() { }

            // Functioning loop of the state
            virtual void Update(int deltaMS)=0;
            virtual void HandleInput(Input& input)=0;
            virtual void Draw(Graphics& graphics)=0;

            // Called by the game to monitor whether the state
            // has been fully processed
            virtual bool IsFinished()=0;

            // Whether this state requires the screen be refreshed after it draws
            virtual bool MustRefreshScreen() { return false; }

            // Complete this state's execution immediately if possible
            virtual void Skip() { }

            // If the state is finished, this method will return the next state for
            // the game to run
            virtual State* NextState(Game* game)=0;
    };
}
