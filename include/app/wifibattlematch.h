// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifibattlematch.h. The proc table's
// name, WIFIBATTLEMATCH_PROC_FUNCTIONS, is pokebw2's guess.
#ifndef BWPORT_APP_WIFIBATTLEMATCH_H
#define BWPORT_APP_WIFIBATTLEMATCH_H

#include "types.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "struct_decls.h"

#define OVERLAY_WIFIBATTLEMATCH OVERLAY_ID(199)

typedef struct {
    GameData *gameData;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
} WifiBattleMatchParam;

extern const GameProcFunctions data_ov199_02213c4c;

#endif // BWPORT_APP_WIFIBATTLEMATCH_H
