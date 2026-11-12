// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/beacon_detail.h: the lines in use so
// far.
#ifndef BWPORT_APP_BEACON_DETAIL_H
#define BWPORT_APP_BEACON_DETAIL_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; matching code confirms the offsets of gsys and unk09 in Black
struct BeaconDetailParam {
    GameSystem *gsys;
    GameData *gameData;
    u8 subscreen;
    // Set when the field is to return to the default subscreen
    u8 unk09;
    u8 unk0A;
};

// The process functions of the beacon detail screen
extern const GameProcFunctions data_ov216_021bdb40;

#endif // BWPORT_APP_BEACON_DETAIL_H
