#include "Log.h"

#include "SDL.h"

#include "GlobalArgs.h"


void ascii::Log::SDLError()
{
    Log::Error(SDL_GetError());
}

bool ascii::Log::Enabled()
{
    return ascii::GlobalArgs::Enabled("log");
}
