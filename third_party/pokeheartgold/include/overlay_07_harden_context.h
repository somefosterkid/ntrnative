#ifndef POKEHEARTGOLD_OVERLAY_07_HARDEN_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_HARDEN_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct BattleAnimSystem;
struct ManagedSprite;
struct SpriteManager;

// sizeof == 0x14 on the DS
typedef struct HardenContext {
    u8 state;
    u8 timer;
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    struct ManagedSprite *attackerSprite1;
    struct ManagedSprite *attackerSprite2;
} HardenContext;

void ov07_02228684(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_HARDEN_CONTEXT_H
