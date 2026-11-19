#ifndef POKEHEARTGOLD_OVERLAY_07_TELEPORT_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_TELEPORT_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct Pokepic;

// sizeof == 0x34 on the DS
typedef struct TeleportContext {
    u8 state;
    u8 scaleType;
    s16 attackerY;
    s16 baseY;
    s16 attackerHeight;
    struct BattleAnimSystem *battleAnimSys;
    struct Pokepic *attackerSprite;
    XYTransformContext scale;
} TeleportContext;

void ov07_022281C0(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_TELEPORT_CONTEXT_H
