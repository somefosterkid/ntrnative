// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wificlub.h: the lines in use so far.
#ifndef BWPORT_APP_WIFICLUB_H
#define BWPORT_APP_WIFICLUB_H

#include "types.h"
#include "gfl/overlay.h"
#include "struct_decls.h"

// Black's (wifi_note.c loads 0x7f); Black 2's is 203
#define OVERLAY_WIFICLUB OVERLAY_ID(127)

// Black 2's, not checked against Black
#define OVERLAY_WIFICLUB_MAIN OVERLAY_ID(173)

// Black 2's; twinned code confirms the offsets of buffer, parties and regulation in Black
struct WifiClubData {
    void *buffer;
    GameData *gameData;
    SaveControl *save;
    // What the player chose in the Wi-Fi Club
    u32 mode;
    u32 unk10;
    PokeParty *parties[2];
    Regulation *regulation;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    // One for each friend in the friend list
    u8 unk24[0x20];
    // One for each of the two players, by net ID
    u8 unk44[2];
    u8 unk46;
    // The friend's index in the friend list, plus 1
    u8 friendIndex;
    u8 unk48;
    u8 unk49;
    u8 unk4A[2];
};

#endif // BWPORT_APP_WIFICLUB_H
