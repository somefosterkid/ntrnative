#ifndef POKEHEARTGOLD_OVERLAY_07_SPLASH_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_SPLASH_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct BattleAnimSystem;
struct Pokepic;

// sizeof == 0x58 on the DS
typedef struct SplashContext {
    u8 state;
    u8 iteration;
    u8 cycles;
    s16 attackerY;
    s16 attackerHeight;
    struct BattleAnimSystem *battleAnimSys;
    struct Pokepic *attackerSprite;
    XYTransformContext scale;
    XYTransformContext pos;
} SplashContext;

void ov07_02228510(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_SPLASH_CONTEXT_H
