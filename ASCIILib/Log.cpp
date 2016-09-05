#include "Log.h"

#include "SDL.h"

#include "GlobalArgs.h"


string ascii::Log::sOutputFilename;

void ascii::Log::SDLError()
{
    Log::Error(SDL_GetError());
}

bool ascii::Log::Enabled()
{
    return ascii::GlobalArgs::Enabled("log");
}

void ascii::Log::SetOutputFilename(string filename)
{
    sOutputFilename = filename;

    // If the file already exists, overwrite it
    ofstream overwriteStream(filename.c_str(), ios_base::trunc);

    // If the file can't be opened, print another error
    if (!overwriteStream.good())
    {
        string filename = sOutputFilename;
        sOutputFilename = "";
        Error("Log file could not be opened for writing: " + filename);
    }
}
