#ifndef POKEHEARTGOLD_OVERLAY_07_EARTHQUAKE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_EARTHQUAKE_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct BgConfig;
struct SpriteManager;

// sizeof == 0xD8 on the DS
typedef struct EarthquakeContext {
    u8 state;
    u8 shakeCount;
    u8 flashFlag;
    u8 flashDuration;
    int spriteCount;
    struct BgConfig *bgConfig;
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    BattleAnimSpriteInfo sprites[4];
    BattleAnimSpriteInfo unused[4];
    XYTransformContext shake;
} EarthquakeContext;

void ov07_02228B38(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_EARTHQUAKE_CONTEXT_H
