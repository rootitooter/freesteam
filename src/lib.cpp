#include "steam/steam_api.h"
#include "steam/steamtypes.h"
#include <stdio.h>
#define _GNU_SOURCE
#include <dlfcn.h>

typedef int (*orig_steamapi_init_type)();

S_API bool S_CALLTYPE SteamAPI_Init() {
    // The only circumstances in which this would normally return false would be
    // if the user a) doesn't own the game or b) steam isn't running. We don't
    // need any of that, so we're just going to return true.

    // Calling the original SteamAPI_Init. We still want it to run, we just
    // don't want our program to know it failed.
    orig_steamapi_init_type orig_init;
    orig_init = (orig_steamapi_init_type)dlsym(RTLD_NEXT,"SteamAPI_Init");
    orig_init();

    printf("Called SteamAPI_Init.");
    return true;
}

S_API bool S_CALLTYPE SteamAPI_RestartAppIfNecessary(uint32 AppID) {
    // Normally, this function would restart the app using steam if steam's not
    // running. We want none of that. Just return false.
    return false;
}