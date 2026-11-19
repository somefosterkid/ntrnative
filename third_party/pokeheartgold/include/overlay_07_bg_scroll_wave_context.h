#ifndef POKEHEARTGOLD_OVERLAY_07_BG_SCROLL_WAVE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_BG_SCROLL_WAVE_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct BgScrollContext;

// sizeof == 0x28 on the DS
typedef struct BgScrollWaveContext {
    int counter;
    int frames;
    BattleAnimScriptFuncCommon common;
    struct BgScrollContext *bgScroll;
} BgScrollWaveContext;

void ov07_02227248(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_BG_SCROLL_WAVE_CONTEXT_H
