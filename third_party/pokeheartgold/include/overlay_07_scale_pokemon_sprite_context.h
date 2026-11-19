#ifndef POKEHEARTGOLD_OVERLAY_07_SCALE_POKEMON_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_SCALE_POKEMON_SPRITE_CONTEXT_H

#include "overlay_07_battle_anim_common.h"

struct ManagedSprite;

// sizeof == 0x54 on the DS
typedef struct ScalePokemonSpriteContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    int state;
    s16 baseY;
    s16 battlerHeight;
    struct ManagedSprite *sprite;
    XYTransformContext scale;
    int spriteAlpha;
    int startScale;
    int endScale;
    int refScale;
    int cycles;
    int scaleFrames;
    BOOL flip;
} ScalePokemonSpriteContext;

#endif // POKEHEARTGOLD_OVERLAY_07_SCALE_POKEMON_SPRITE_CONTEXT_H
