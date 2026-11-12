#ifndef POKEBW2_SYSTEM_CTVT_BEACON_H
#define POKEBW2_SYSTEM_CTVT_BEACON_H

#include "types.h"
#include "struct_decls.h"

// What the main module reads of an Xtransceiver's beacon (app/comm_tvt/ctvt_comm.h's CtvtCommBeacon). The file's
// name and the functions' are ours

// Whether the beacon invites the machine of the MAC address and its player is among the friends
BOOL CtvtBeacon_IsFriendInvite(CtvtCommBeacon *beacon, WifiList *wifiList, const u8 *mac);
u16 *CtvtBeacon_GetPlayerName(CtvtCommBeacon *beacon);
u16 CtvtBeacon_GetTrainerID(CtvtCommBeacon *beacon);

#endif // POKEBW2_SYSTEM_CTVT_BEACON_H
