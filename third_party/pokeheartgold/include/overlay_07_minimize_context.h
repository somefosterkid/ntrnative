#ifndef POKEHEARTGOLD_OVERLAY_07_MINIMIZE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_MINIMIZE_CONTEXT_H

#include "overlay_07_battle_anim_common.h"

struct ManagedSprite;

// sizeof == 0x2C on the DS
typedef struct MinimizeSprite {
    u8 state;
    u8 frameCounter;
    struct ManagedSprite *sprite;
    XYTransformContext scale;
} MinimizeSprite;

// sizeof == 0x9C on the DS
typedef struct MinimizeContext {
    u8 state;
    u8 cycles;
    s16 attackerY;
    int attackerSpriteHeight;
    s8 spriteDir;
    struct Pokepic *attackerSprite;
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    MinimizeSprite sprites[3];
} MinimizeContext;

#endif // POKEHEARTGOLD_OVERLAY_07_MINIMIZE_CONTEXT_H
