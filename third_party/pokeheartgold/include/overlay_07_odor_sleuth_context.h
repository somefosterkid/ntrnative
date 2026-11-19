#ifndef POKEHEARTGOLD_OVERLAY_07_ODOR_SLEUTH_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_ODOR_SLEUTH_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct ManagedSprite;
struct Pokepic;

// sizeof == 0xC on the DS
typedef struct SpriteShakeInfo {
    s16 extentX;
    s16 extentY;
    s16 interval;
    s16 amount;
    int targets;
} SpriteShakeInfo;

// sizeof == 0x88 on the DS
typedef struct OdorSleuthContext {
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo unused;
    Point2D spritePositions[2];
    struct Pokepic *battlerSprites[2];
    struct ManagedSprite *monSprites[2];
    XYTransformContext shake[2];
} OdorSleuthContext;

void ov07_0222915C(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_ODOR_SLEUTH_CONTEXT_H
