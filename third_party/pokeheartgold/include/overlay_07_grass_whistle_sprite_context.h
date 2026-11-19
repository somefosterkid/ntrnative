#ifndef POKEHEARTGOLD_OVERLAY_07_GRASS_WHISTLE_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_GRASS_WHISTLE_SPRITE_CONTEXT_H

// GrassWhistleSpriteContext: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/overlay_07_0222EF58_0222FEB0_0222FFD0.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct BattleAnimSystem;
struct ManagedSprite;
struct SpriteManager;
struct SpriteSystem;

typedef struct GrassWhistleSpriteContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    int unused;
    int dirX;
    int dirY;
    struct ManagedSprite *sprites[15];
    struct { s16 x; s16 y; s32 data[8]; } notePos[15];
    int noteDelay;
    int noteIndex;
} GrassWhistleSpriteContext;

// sizeof(GrassWhistleSpriteContext) == 0x278

#endif // POKEHEARTGOLD_OVERLAY_07_GRASS_WHISTLE_SPRITE_CONTEXT_H
