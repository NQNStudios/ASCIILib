#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;

// Unix includes
#if defined( MAC ) || defined( LINUX )
#include <dirent.h>
#include <sys/stat.h>
#endif

// Windows includes
#ifdef WINDOWS
#include <windows.h>
#include <shlobj.h>
#endif

#include "Log.h"
using namespace ascii;

namespace
{
    // TODO will there be a problem on any system with the spaces in this name?
    //  * Not Mac
    const string FILE_DIRECTORY_NAME = "The Whisperer in Darkness";
#ifdef WINDOWS
    // Go home Windows, your drunk
    const string PATH_SEPARATOR = "\\";
#else
    const string PATH_SEPARATOR = "/";
#endif
}


namespace ascii
{

#if defined (MAC) || defined(LINUX)
inline string UserHome()
{
    const char* pHome = getenv("HOME");

    if (!pHome)
    {
        Log::Error("Environment variable HOME is undefined");
    }

    return string(pHome);
}
#endif

#ifdef MAC

// Retrieve the path to the Application Support directory in the current user's
// Library
inline string ApplicationSupportDirectory()
{
    stringstream path;
    path << UserHome() << "/Library/Application Support/";

    // Check if the directory exists before returning it
    ifstream file(path.str().c_str());
    if (!file.good())
    {
        Log::Error("User's HOME directory does not contain 'Library/Application Support' folder");
    }

    return path.str();
}

#endif

#ifdef WINDOWS

// Return the path to the user's "My Documents" folder on Windows
inline string UserDocumentsDirectory()
{
    char* documents_path = new char[MAX_PATH];
    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL,
            SHGFP_TYPE_CURRENT, documents_path);

    if (result != S_OK)
    {
        Log::Error("Failed to determine path to the user's 'Documents' folder.");
    }

	string pathResult(documents_path);
	delete documents_path;
    return pathResult + PATH_SEPARATOR;
}

#endif

// Return the path to the proper directory to store game files
inline string FileDirectory()
{
#ifdef MAC
    return ApplicationSupportDirectory() + FILE_DIRECTORY_NAME + "/";
#endif
#ifdef WINDOWS
    return UserDocumentsDirectory() + FILE_DIRECTORY_NAME + PATH_SEPARATOR;
#endif
#ifdef LINUX
    stringstream path;
    path << UserHome() << "/.whisperer-in-darkness/";
    return path.str();
#endif
}

// Check whether the proper directory for outputting game files exists yet
inline bool FileDirectoryExists()
{
    //Log::Print("Checking if game file directory exists: " + FileDirectory());
#ifdef WINDOWS
    DWORD ftyp = GetFileAttributesA(FileDirectory().c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    {
        return true;
    }

    Log::Error("Path to game files directory doesn't reference a directory: " + FileDirectory());
    return false;
#else
    ifstream file(FileDirectory().c_str());
    return file.good();
#endif
}

// Create the proper directory for writing game files on the current operating
// system
inline void CreateFileDirectory()
{
#if defined( MAC ) || defined( LINUX )
    mkdir(FileDirectory().c_str(), 0777);
#endif
#ifdef WINDOWS
    if (!CreateDirectory(FileDirectory().c_str(), NULL))
    {
        stringstream error;
        error << "Failed to create game file directory on Windows: ";
        DWORD error_code = GetLastError();

        if (error_code == ERROR_ALREADY_EXISTS)
        {
            error << "The folder already exists.";
        }
        else if (error_code == ERROR_PATH_NOT_FOUND)
        {
            error << "Intermediate directories were missing in the attempted creation path.";
        }

        Log::Error(error.str());
    }
#endif

    // If it still doesn't exist...
    if (!FileDirectoryExists())
    {
        Log::Error("Failed to create game files folder: " + FileDirectory());
    }
}

// Retrieve the path of the simulated working directory on Mac
#ifdef MAC
string GetWorkingDirectory();
#endif

// Convert the given path (relative to the assumed working directory) into an
// appropriate path for accessing the file on the current operating system
string FileAccessPath(string path);

}
