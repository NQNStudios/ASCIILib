#include "FilePaths.h"

#ifdef MAC
namespace mac
{
#include "CoreFoundation/CoreFoundation.h"
}
#endif

#ifdef MAC
string ascii::GetWorkingDirectory()
{
    //Log::Print("Working directory needs to be simulated for Mac app.");

    mac::CFBundleRef mainBundle = mac::CFBundleGetMainBundle();
    mac::CFURLRef resourcesURL = mac::CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!mac::CFURLGetFileSystemRepresentation(resourcesURL, true, (mac::UInt8 *)path, PATH_MAX))
    {
        Log::Error("Failed to properly set working directory for mac .app launch");
    }
    mac::CFRelease(resourcesURL);

    //Log::Print("Current working directory: ", false);
    //Log::Print(path);

    return path;
}
#endif

string ascii::FileAccessPath(string path)
{
#ifdef MAC
    return ascii::GetWorkingDirectory() + "/" + path;
#endif
	return path;
}
