#include "steam/steam_api.h"
#include "steam/steamtypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <strings.h>
#define _GNU_SOURCE

typedef int (*orig_steamapi_init_type)();

const char *USER;
const char *APPID;
const char *INSTALLPATH;

S_API bool S_CALLTYPE SteamAPI_Init() {
    // Now, we want to do some initialization of our own!
    // Get username!
    char *username;
    username = getenv("USER");
    if (username == NULL || strcmp(username, ""))
        username = "freesteam";
    
    // Get Install path!
    INSTALLPATH = getenv("FS_INSTALLPATH");
    if (INSTALLPATH == NULL || strcmp(INSTALLPATH, "") == 0) {
        char *xdg_home;
        xdg_home = getenv("XDG_DATA_HOME");
        if (xdg_home == NULL || strcmp(xdg_home, ""))
            xdg_home = strcat(getenv("HOME"), "/.local/share");
        // TODO: Use getpwdid to get home directory if HOME environment variable not set
        INSTALLPATH = strcat(xdg_home, "/freesteam");
    }

    APPID = getenv("SteamAppId");
    // TODO: Get APPID from file

    // Calling the original SteamAPI_Init. We still want it to run, we just
    // don't want our program to know it failed.
    //orig_steamapi_init_type orig_init;
    //orig_init = (orig_steamapi_init_type)dlsym(RTLD_NEXT,"SteamAPI_Init");
    //orig_init();

    // The only circumstances in which this would normally return false would be
    // if the user a) doesn't own the game or b) steam isn't running. We don't
    // need any of that, so we're just going to return true.
    return true;
}

S_API bool S_CALLTYPE SteamAPI_RestartAppIfNecessary(uint32 AppID) {
    // Normally, this function would restart the app using steam if steam's not
    // running. We want none of that. Just return false.
    // Even though IsSteamRunning is always going to return true, this will
    // still return false if it doesn't detect a valid appID, so we need to
    // provide one somehow. Best way is to through the steam_appid.txt file.
    return false;
}

S_API bool S_CALLTYPE SteamAPI_IsSteamRunning() {
    // This needs to return true, otherwise, SteamAPI_Init will fail.
    return true;
}

S_API const char *SteamAPI_GetSteamInstallPath() {
    return INSTALLPATH;
}