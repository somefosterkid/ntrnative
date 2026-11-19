#ifndef POKEHEARTGOLD_OVERLAY_07_SWAGGER_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_SWAGGER_SPRITE_CONTEXT_H

// SwaggerSpriteContext: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/overlay_07_02223240_02223CE4_02223CE4.c) and 20 accesses; fields at every accessed offset, pads elsewhere.

struct BattleAnimSystem;
struct ManagedSprite;
struct SpriteManager;
struct SpriteSystem;

typedef struct SwaggerSpriteContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    int state;
    int unused;
    struct ManagedSprite *sprite;
    struct { s16 x; s16 y; s32 data[8]; } scale;
    int dir;
    int scaleStep;
    int delay;
    int popState;
    s16 defenderX;
    s16 defenderY;
} SwaggerSpriteContext;

// sizeof(SwaggerSpriteContext) == 0x50

#endif // POKEHEARTGOLD_OVERLAY_07_SWAGGER_SPRITE_CONTEXT_H
