// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_musical_look_proc.h:
// the lines in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_MUSICAL_LOOK_PROC_H
#define BWPORT_APP_BATTLE_RECORDER_BR_MUSICAL_LOOK_PROC_H

#include "types.h"
#include "struct_decls.h"

struct BrMusicalLookProcParam {
    BrFade *fade;
    BrSidebar *sidebar;
    BrGraphic *graphic;
    BrRes *res;
    BrProcSys *procSys;
    BrNet *net;
    GameData *gameData;
};

#endif // BWPORT_APP_BATTLE_RECORDER_BR_MUSICAL_LOOK_PROC_H
