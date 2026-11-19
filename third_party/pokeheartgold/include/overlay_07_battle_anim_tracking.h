#ifndef POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_TRACKING_H
#define POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_TRACKING_H

#include "global.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"

struct BgConfig;

// The per-slot tracking tasks of BattleAnimSystem (spriteTrackingTasks[], bgTrackingTask): a sys task that copies a
// Pokepic's position onto a ManagedSprite (or a BG layer). Sizes: 0x10 on the DS, 0x20 on the PC (pointer slots).
typedef struct PokemonSpriteTrackingTask {
    u8 frameCount;
    u8 interval;
    Pokepic *sprite;
} PokemonSpriteTrackingData;

typedef struct SpriteTrackingTask {
    ManagedSprite *sprite;
    PokemonSpriteTrackingData data;
    SysTask *task;
} SpriteTrackingTask;

typedef struct BgTrackingTask {
    struct BgConfig *bg;
    PokemonSpriteTrackingData data;
    SysTask *task;
} BgTrackingTask;

#endif // POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_TRACKING_H
