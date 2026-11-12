// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_video.h. pokebw2's
// BATTLE_VIDEO_PROC_FUNCTIONS is Black's data_0209defc.
#ifndef BWPORT_APP_BATTLE_VIDEO_H
#define BWPORT_APP_BATTLE_VIDEO_H

#include "types.h"
#include "struct_decls.h"

// The Battle Video proc is in the main program
typedef struct {
    GameData *gameData;
    u32 mode;
} BattleVideoParam;

extern const GameProcFunctions data_0209defc;

#endif // BWPORT_APP_BATTLE_VIDEO_H
