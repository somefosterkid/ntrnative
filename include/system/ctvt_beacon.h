// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/ctvt_beacon.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_CTVT_BEACON_H
#define BWPORT_SYSTEM_CTVT_BEACON_H

#include "types.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CtvtBeacon_IsFriendInvite
// Whether the beacon invites the machine of the MAC address and its player is among the friends
BOOL func_0202e8d8(CtvtCommBeacon *beacon, WifiList *wifiList, const u8 *mac);
// pokebw2's CtvtBeacon_GetPlayerName
u16 *func_0202e930(CtvtCommBeacon *beacon);
// pokebw2's CtvtBeacon_GetTrainerID
u16 func_0202e938(CtvtCommBeacon *beacon);

#endif // BWPORT_SYSTEM_CTVT_BEACON_H
