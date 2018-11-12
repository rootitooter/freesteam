#include "steam/steam_api.h"
#include <iostream>

int main() {
    if (SteamAPI_RestartAppIfNecessary(488400)) {
        return 1;
    }
    
    if(!SteamAPI_Init()) {
        std::cerr << "Failed to initialize Steamworks API." << std::endl;
        return 1;
    }

    std::cout << "Steam install path: " << SteamAPI_GetSteamInstallPath() << std::endl;
    return 0;
}