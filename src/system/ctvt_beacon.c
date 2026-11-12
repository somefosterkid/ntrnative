// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/ctvt_beacon.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/player_info.h"
#include "save/wifi_list.h"

#include "battle/btl_event.h"

typedef struct CtvtCommBeacon {
    PlayerInfo player;
    u8 memberCount;
    u8 cameraEnabled : 1;
    u8 inviteOnly : 1;
    u8 inviteMacs[3][6];
} CtvtCommBeacon;

// What the main module reads of an Xtransceiver's beacon, as it finds the machines calling this one. The file's
// name is a guess

// pokebw2's CtvtBeacon_IsFriendInvite
BOOL func_0202e8d8(CtvtCommBeacon *beacon, WifiList *wifiList, const u8 *mac) {
    u8 i;
    u8 j;

    if (wifiList == NULL) {
        return FALSE;
    }
    for (i = 0; i < 3; i++) {
        BOOL match = TRUE;

        for (j = 0; j < 6; j++) {
            if (beacon->inviteMacs[i][j] != mac[j]) {
                match = FALSE;
                break;
            }
        }
        if (match == TRUE) {
            return func_02009d1c(wifiList, &beacon->player, NULL);
        }
    }
    return FALSE;
}

