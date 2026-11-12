// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/box2.h, the lines in use so far.
#ifndef BWPORT_APP_BOX2_H
#define BWPORT_APP_BOX2_H

#include "types.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// The PC Box, overlay 255 in Black 2
#define OVERLAY_BOX2 OVERLAY_ID(170)

#define BOX2_MODE_DREAM_WORLD 5

typedef struct {
    GameData *gameData;
    BoxSaveAccessor *boxes;
    PokeParty *party;
    BagSave *bag;
    PlayerInfo *playerInfo;
    u32 unk14;
    TrainerDataSave *trainerData;
    u32 unk1C;
    // Flags by species
    u8 *unk20;
    u32 mode;
    u16 unk28;
    // Where the chosen Pokemon is, 0xff for both if none was chosen
    u8 tray;
    u8 position;
} Box2Param;

// pokebw2's BOX2_PROC_FUNCTIONS
extern const GameProcFunctions data_ov170_021ef78c;

#endif // BWPORT_APP_BOX2_H
