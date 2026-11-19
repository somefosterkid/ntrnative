#ifndef POKEHEARTGOLD_OVERLAY_07_VITAL_THROW_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_VITAL_THROW_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct BattleAnimSystem;
struct Pokepic;

// sizeof == 0x64 on the DS
typedef struct VitalThrowContext {
    int state;
    struct BattleAnimSystem *battleAnimSys;
    int dir;
    struct Pokepic *attackerSprite;
    s16 attackerX;
    s16 attackerY;
    XYTransformContext attackerPos;
    struct Pokepic *defenderSprite;
    s16 defenderX;
    s16 defenderY;
    XYTransformContext defenderPos;
} VitalThrowContext;

void ov07_0222EBB0(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_VITAL_THROW_CONTEXT_H
