#ifndef POKEHEARTGOLD_OVERLAY_07_MEMENTO_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_MEMENTO_CONTEXT_H

// MementoContext: merged by tools/hg/promote.py from 4 m2c local copies (1 variants, base
// src/overlay_07_0222EF58_0222F600_0222F600.c) and 29 accesses; fields at every accessed offset, pads elsewhere.

#include "overlay_07_alpha_fade_context.h"

struct BattleAnimSystem;
struct ManagedSprite;
struct PaletteData;
struct SpriteManager;

typedef struct MementoContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    int state;
    int delay;
    int attackerHeight;
    int attackerY;
    struct ManagedSprite *attackerSprite;
    struct ManagedSprite *defenderSprite;
    struct ManagedSprite *attackerPartnerSprite;
    struct ManagedSprite *defenderPartnerSprite;
    struct { s16 x; s16 y; s32 data[8]; } scale;
    struct { s16 x; s16 y; s32 data[8]; } scaleX;
    struct { s16 x; s16 y; s32 data[8]; } defenderPos;
    int spriteState;
    int spriteDelay;
    AlphaFadeContext alpha;
    struct PaletteData *paletteData;
    BOOL attackerFlipped;
    BOOL defenderFlipped;
} MementoContext;

// sizeof(MementoContext) == 0xd0

#endif // POKEHEARTGOLD_OVERLAY_07_MEMENTO_CONTEXT_H
