#ifndef POKEHEARTGOLD_UNK_02097B78_H
#define POKEHEARTGOLD_UNK_02097B78_H

#include "overlay_manager.h"
#include "save.h"
#include "legendary_cinematic_task.h"

typedef struct LegendaryCinematicArgs {
    SaveData *saveData;
    ModelAttributes unk4;
    u16 unk4C;
    u16 unk4E;
} LegendaryCinematicArgs;

// Environment of the launcher task (sub_02097CB4 creates it, sub_02097CD8 runs it). DS 0x0C.
typedef struct LegendaryCinematicLauncher {
    LegendaryCinematicTask *task; // 0x00
    LegendaryCinematicArgs *args; // 0x04
    u32 state;                    // 0x08
} LegendaryCinematicLauncher;

BOOL LegendaryCinematic_Init(OverlayManager *man, int *state);
BOOL LegendaryCinematic_Main(OverlayManager *man, int *state);
BOOL LegendaryCinematic_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_UNK_02097B78_H
