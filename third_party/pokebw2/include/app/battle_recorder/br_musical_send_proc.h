#ifndef POKEBW2_APP_BATTLE_RECORDER_BR_MUSICAL_SEND_PROC_H
#define POKEBW2_APP_BATTLE_RECORDER_BR_MUSICAL_SEND_PROC_H

#include "types.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// The Battle Recorder's musical photo sending screen (br_musical_send_proc.c, overlay 269), which br_core.c runs as
// BR_PROCID_MUSICAL_SEND. The name is a guess: the ROM gives none

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

extern const GameProcFunctions BR_MUSICAL_SEND_PROC_FUNCTIONS;

#endif // POKEBW2_APP_BATTLE_RECORDER_BR_MUSICAL_SEND_PROC_H
