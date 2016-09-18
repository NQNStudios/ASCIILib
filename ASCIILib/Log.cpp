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
	FILE* overwriteFile = fopen(filename.c_str(), "w");

    if (overwriteFile == NULL)
    {
        string filename = sOutputFilename;
        sOutputFilename = "";
        Error("Log file could not be opened for writing: " + filename);
    }

	fclose(overwriteFile);
}
