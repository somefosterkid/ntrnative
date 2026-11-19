#include "global.h"

#include "constants/heap.h"
#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_funcs.h"

BOOL ov07_0221BFC0(BattleAnimSystem *system);

BOOL ov07_0221BFC0(BattleAnimSystem *system) {
    if (system == NULL) {
        GF_AssertFail();
    }
    return system->isContest;
}

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system) {
    if (system == NULL) {
        GF_AssertFail();
    }
    return (enum HeapID)system->heapID;
}

BOOL ov07_0221C3DC(BattleAnimSystem *system);

s32 ov07_0221BFE0(BattleAnimSystem *system);

s32 ov07_0221BFE0(BattleAnimSystem *system) {
    s32 i;

    if (ov07_0221C3DC(system) == 0) {
        return 0;
    }
    i = 0;
    do {
        NARC_Delete(system->arcs[i]);
        i += 1;
    } while (i < 6);
    Heap_Free(system->context);
    Heap_Free(system);
    return 1;
}
