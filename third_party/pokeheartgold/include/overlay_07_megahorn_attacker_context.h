#ifndef POKEHEARTGOLD_OVERLAY_07_MEGAHORN_ATTACKER_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_MEGAHORN_ATTACKER_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct Pokepic;

// sizeof == 0x64 on the DS
typedef struct MegahornAttackerContext {
    struct BattleAnimSystem *battleAnimSys;
    int state;
    int delay;
    int dirX;
    int dirY;
    struct Pokepic *sprite;
    s16 x;
    s16 y;
    XYTransformContext pos;
    XYTransformContext shake;
} MegahornAttackerContext;

void ov07_02230A70(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_MEGAHORN_ATTACKER_CONTEXT_H
