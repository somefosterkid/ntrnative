#ifndef POKEHEARTGOLD_OVERLAY_07_SHAKE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_SHAKE_CONTEXT_H

#include "global.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_common.h"

typedef struct SpriteShakeInfo {
    s16 extentX;
    s16 extentY;
    s16 interval;
    s16 amount;
    int targets;
} SpriteShakeInfo;

// Shake the sprites of the selected battlers (task ov07_02223240). Heap object ov07_022324D8(system, sizeof(ShakeContext)).
// sizeof == 0xA0 on the DS
typedef struct ShakeContext {
    int spriteCount;
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo shakeInfo;
    BattleAnimSpriteInfo sprites[4];
    XYTransformContext shake;
} ShakeContext;

void ov07_02223240(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_SHAKE_CONTEXT_H
