// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/gtsnego.h, the lines in use so far.
// The proc table's name, GTSNEGO_PROC_FUNCTIONS, is pokebw2's guess.
#ifndef BWPORT_APP_GTSNEGO_H
#define BWPORT_APP_GTSNEGO_H

#include "types.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "struct_decls.h"

#define OVERLAY_GTSNEGO OVERLAY_ID(120)

#define GTSNEGO_RESULT_RETRY_LOGIN 0
#define GTSNEGO_RESULT_EXIT 1

typedef struct {
    u32 unk0;
    u32 unk4;
} GtsNegoUnk4;

typedef struct {
    GameData *gameData;
    // For each of the two players
    GtsNegoUnk4 unk4[2];
    // Copies of the player's info
    PlayerInfo *playerInfo;
    PlayerInfo *playerInfo2;
    u32 unk1C[100];
    s32 unk1AC;
    u32 result;
} GtsNegoParam;

extern const GameProcFunctions data_ov120_021d890c;

// Black 2's, not checked against Black
// A partner is being searched for or was found: anyone, or one of the people of the Pal Pad
#define GTSNEGO_RESULT_ANYONE 2

// Black 2's, not checked against Black
#define GTSNEGO_RESULT_RENDEZVOUS 3
#define GTSNEGO_MET_COUNT 100

#endif // BWPORT_APP_GTSNEGO_H
