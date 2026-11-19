#ifndef POKEHEARTGOLD_OVERLAY_07_GROWTH_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_GROWTH_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

// sizeof == 0x30 on the DS
typedef struct GrowthContext {
    u8 state;
    u8 iterations;
    u8 scaleState;
    struct BattleAnimSystem *battleAnimSys;
    struct Pokepic *sprite;
    XYTransformContext scale;
} GrowthContext;

void ov07_02227FA4(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_GROWTH_CONTEXT_H
