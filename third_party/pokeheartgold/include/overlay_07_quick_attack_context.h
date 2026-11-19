#ifndef POKEHEARTGOLD_OVERLAY_07_QUICK_ATTACK_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_QUICK_ATTACK_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_afterimage_context.h"

struct BattleAnimSystem;
struct ManagedSprite;
struct Pokepic;
struct SpriteManager;

// sizeof == 0xF0 on the DS
typedef struct QuickAttackContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    struct Pokepic *attackerSprite;
    XYTransformContext rev;
    struct ManagedSprite *afterimageSprites[2];
    AfterimageContext afterImageCtx;
    s16 attackerX;
    s16 attackerY;
    s16 attackerShadowHeight;
} QuickAttackContext;

#endif // POKEHEARTGOLD_OVERLAY_07_QUICK_ATTACK_CONTEXT_H
