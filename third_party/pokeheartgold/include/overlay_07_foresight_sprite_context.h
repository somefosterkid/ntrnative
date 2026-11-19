#ifndef POKEHEARTGOLD_OVERLAY_07_FORESIGHT_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_FORESIGHT_SPRITE_CONTEXT_H

// ForesightSpriteContext: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_07_0222DEEC.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct BattleAnimSystem;
struct ManagedSprite;
struct Pokepic;
struct SpriteManager;
struct SpriteSystem;

typedef struct ForesightSpriteContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    int state;
    struct Pokepic *defenderSprite;
    struct ManagedSprite *sprite;
    struct { s16 x; s16 y; s32 data[8]; } pos;
    struct { struct { s16 x; s16 y; s32 data[8]; } lerp; int done; } alpha;
    s16 baseX;
    s16 baseY;
    int segment;
    int delay;
} ForesightSpriteContext;

// sizeof(ForesightSpriteContext) == 0x70

#endif // POKEHEARTGOLD_OVERLAY_07_FORESIGHT_SPRITE_CONTEXT_H
