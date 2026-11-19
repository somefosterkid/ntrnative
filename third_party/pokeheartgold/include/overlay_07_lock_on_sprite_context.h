#ifndef POKEHEARTGOLD_OVERLAY_07_LOCK_ON_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_LOCK_ON_SPRITE_CONTEXT_H

// LockOnSpriteContext: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_07_0222D578_0222E108_0222E108.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct BattleAnimSystem;
struct ManagedSprite;
struct Pokepic;
struct SpriteManager;
struct SpriteSystem;

typedef struct LockOnSpriteContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    int state;
    struct Pokepic *defenderSprite;
    struct Pokepic *battlerSprites[4];
    struct ManagedSprite *sprite;
    struct { s16 x; s16 y; s32 data[8]; } pos;
    s16 baseX;
    s16 baseY;
    int segment;
    int delay;
    int blinkVisible;
} LockOnSpriteContext;

// sizeof(LockOnSpriteContext) == 0x5c

#endif // POKEHEARTGOLD_OVERLAY_07_LOCK_ON_SPRITE_CONTEXT_H
