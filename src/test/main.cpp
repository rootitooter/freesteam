#include "steam/steam_api.h"
#include "steam/isteamappticket.h"
#include <iostream>

int main() {
    if (SteamAPI_RestartAppIfNecessary(480)) {
        return 1;
    }
    
    if(!SteamAPI_Init()) {
        std::cerr << "Failed to initialize Steamworks API." << std::endl;
        return 1;
    }

    std::cout << "Steam install path: " << SteamAPI_GetSteamInstallPath() << std::endl;

    // Check ownership
    ISteamAppTicket *SteamAppTicket = (ISteamAppTicket*)SteamClient()
                                        ->GetISteamGenericInterface(
                                            SteamAPI_GetHSteamUser(),
                                            SteamAPI_GetHSteamPipe(),
                                            STEAMAPPTICKET_INTERFACE_VERSION
                                        );

    const int k_bufferLength = 256;
    const int k_signatureLength = 128;
    char buffer[k_bufferLength];
    uint32 iAppID;
    uint32 iSteamID;
    uint32 iSignature;
    uint32 cbSignature;
    uint32 ret = SteamAppTicket->GetAppOwnershipTicketData(4800, buffer,
                                    k_bufferLength, &iAppID, &iSteamID,
                                    &iSignature, &cbSignature);
    
    printf("AppTicket ret: %d\n", ret);
    if (ret > 0) {
        printf( "iAppID offset: %d\n", iAppID );
		printf( "iSteamID offset: %d\n", iSteamID );
		printf( "iSignature offset: %d\n", iSignature );
		printf( "cbSignature length: %d\n\n", cbSignature );

		AppId_t appID;
		memcpy( &appID, &buffer[iAppID], sizeof(AppId_t) );
		printf( "App ID: %d\n", appID);

		CSteamID steamID;
		memcpy( &steamID, &buffer[iSteamID], sizeof(CSteamID) );
		printf( "Steam ID: %lld\n", steamID.ConvertToUint64() );
    }

    return 0;
}