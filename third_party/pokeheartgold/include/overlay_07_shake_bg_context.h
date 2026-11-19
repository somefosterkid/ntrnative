#ifndef POKEHEARTGOLD_OVERLAY_07_SHAKE_BG_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_SHAKE_BG_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct BattleAnimSystem;
struct BgConfig;
struct SpriteManager;

// sizeof == 0xF8 on the DS
typedef struct ShakeBgContext {
    u8 state;
    u8 iteration;
    u8 unused0[2];
    int extentX;
    int extentY;
    int interval;
    int amount;
    int sLotoId;
    int cycles;
    int bgLayer;
    BOOL resetX;
    BOOL resetY;
    struct BgConfig *bgConfig;
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    BattleAnimSpriteInfo unused2[8];
    XYTransformContext shake;
} ShakeBgContext;

void ov07_02225DC4(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_SHAKE_BG_CONTEXT_H
