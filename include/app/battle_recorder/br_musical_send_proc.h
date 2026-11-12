// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_musical_send_proc.h,
// the lines in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_MUSICAL_SEND_PROC_H
#define BWPORT_APP_BATTLE_RECORDER_BR_MUSICAL_SEND_PROC_H

#include "types.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// The Battle Recorder's musical photo sending screen (br_musical_send_proc.c, overlay 184; Black 2's overlay 269). The
// name is pokebw2's guess: the ROM gives none

typedef struct {
    BrFade *fade;
    BrSidebar *sidebar;
    BrGraphic *graphic;
    BrRes *res;
    BrProcSys *procSys;
    BrNet *net;
    GameData *gameData;
    // TRUE if the player chose to send the photo, FALSE if they went back, which makes the menu fade in differently
    BOOL isSend;
} BrMusicalSendProcParam;

// pokebw2's data_ov184_021f8a04
extern const GameProcFunctions data_ov184_021f8a04;

#endif // BWPORT_APP_BATTLE_RECORDER_BR_MUSICAL_SEND_PROC_H
