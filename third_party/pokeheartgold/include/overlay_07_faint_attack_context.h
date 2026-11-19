#ifndef POKEHEARTGOLD_OVERLAY_07_FAINT_ATTACK_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_FAINT_ATTACK_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"
#include "overlay_07_alpha_fade_context.h"
struct ManagedSprite;
struct Pokepic;
struct SpriteManager;

// sizeof == 0x6C on the DS
typedef struct FaintAttackContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    int state;
    int delay;
    struct Pokepic *attackerSprite;
    struct ManagedSprite *sprite;
    s16 attackerX;
    s16 attackerY;
    XYTransformContext revs;
    AlphaFadeContext alpha;
    s16 attackerShadowHeight;
} FaintAttackContext;

void ov07_0222DA60(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_FAINT_ATTACK_CONTEXT_H
