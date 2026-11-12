// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/bag.h: the lines in use so far.
#ifndef BWPORT_APP_BAG_H
#define BWPORT_APP_BAG_H

#include "types.h"
#include "field/player_action.h"
#include "gfl/overlay.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define OVERLAY_BAG OVERLAY_ID(142)

// Black 2's, minus the Free Space's filter and the Dowsing MCHN flag; item is at 0x38 in Black
// What the bag is started with
struct BagProcessData {
    GameData *gameData;
    TrainerDataSave *trainerData;
    PlayerInfo *playerInfo;
    // The bag's cursor in each pocket, which func_0200887c and the functions after it read and set
    void *cursor;
    BagSave *bag;
    PlayerActionPerms perms;
    u32 mode;
    BOOL isCycling;
    // What the player did, 0 for nothing
    u32 result;
    u32 item;
};

#include "gfl/heap.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BagParam_Create
// Creates the bag's data for a mode, in the main module, with a copy of the perms when they are given
BagProcessData *func_0202e474(GameData *gameData, const PlayerActionPerms *perms, u32 mode, HeapID heapId);

#endif // BWPORT_APP_BAG_H

