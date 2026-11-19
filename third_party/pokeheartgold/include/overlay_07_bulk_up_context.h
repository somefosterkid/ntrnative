#ifndef POKEHEARTGOLD_OVERLAY_07_BULK_UP_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_BULK_UP_CONTEXT_H

#include "global.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;
struct Pokepic;

// sizeof(BulkUpContext) == 0x58 on the DS (heap alloc in ov07_02227AEC)
typedef struct BulkUpContext {
    u8 state;
    s16 baseY;
    int spriteHeight;
    struct BattleAnimSystem *battleAnimSys;
    struct Pokepic *sprite;
    XYTransformContext scale;
    XYTransformContext unused;
} BulkUpContext;

void ov07_022278B0(SysTask *task, void *param);
void ov07_02227AEC(struct BattleAnimSystem *system);

#endif // POKEHEARTGOLD_OVERLAY_07_BULK_UP_CONTEXT_H
