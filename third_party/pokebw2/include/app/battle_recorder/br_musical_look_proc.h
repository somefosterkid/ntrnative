#ifndef POKEBW2_APP_BATTLE_RECORDER_BR_MUSICAL_LOOK_PROC_H
#define POKEBW2_APP_BATTLE_RECORDER_BR_MUSICAL_LOOK_PROC_H

#include "types.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// The Battle Recorder's musical photo viewing screen (br_musical_look_proc.c, overlay 270), which br_core.c runs as
// BR_PROCID_MUSICAL_LOOK. The name is a guess: the ROM gives none

typedef struct {
    BrFade *fade;
    BrSidebar *sidebar;
    BrGraphic *graphic;
    BrRes *res;
    BrProcSys *procSys;
    BrNet *net;
    GameData *gameData;
} BrMusicalLookProcParam;

extern const GameProcFunctions BR_MUSICAL_LOOK_PROC_FUNCTIONS;

#endif // POKEBW2_APP_BATTLE_RECORDER_BR_MUSICAL_LOOK_PROC_H
