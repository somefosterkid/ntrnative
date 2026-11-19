#ifndef POKEHEARTGOLD_OVERLAY_07_BLINK_ATTACKER_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_BLINK_ATTACKER_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct BattleAnimSystem;

// sizeof == 0x40 on the DS
typedef struct BlinkAttackerContext {
    int count;
    int timer;
    int interval;
    BattleAnimSpriteInfo spriteInfo;
    BattleAnimScriptFuncCommon common;
    struct BattleAnimSystem *battleAnimSys;
} BlinkAttackerContext;

void ov07_022248F4(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_BLINK_ATTACKER_CONTEXT_H
