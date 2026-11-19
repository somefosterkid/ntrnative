#ifndef POKEHEARTGOLD_OVERLAY_07_MOVE_BATTLER_X_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_MOVE_BATTLER_X_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct BattleAnimSystem;

// sizeof == 0x68 on the DS
typedef struct MoveBattlerXContext {
    int unused0;
    int sLotoId;
    int frames;
    s16 offset;
    BattleAnimSpriteInfo spriteInfo;
    BattleAnimScriptFuncCommon common;
    XYTransformContext pos;
    struct BattleAnimSystem *battleAnimSys;
} MoveBattlerXContext;

void ov07_02224998(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_MOVE_BATTLER_X_CONTEXT_H
