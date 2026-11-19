#ifndef POKEHEARTGOLD_OVERLAY_07_MEDITATE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_MEDITATE_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct Pokepic;

// sizeof == 0x34 on the DS
typedef struct MeditateContext {
    u8 state;
    u8 iterations;
    s16 attackerY;
    s16 attackerHeight;
    struct BattleAnimSystem *battleAnimSys;
    struct Pokepic *attackerSprite;
    XYTransformContext scale;
} MeditateContext;

void ov07_022280A8(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_MEDITATE_CONTEXT_H
