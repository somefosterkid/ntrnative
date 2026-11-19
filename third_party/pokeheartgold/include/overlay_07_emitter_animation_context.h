#ifndef POKEHEARTGOLD_OVERLAY_07_EMITTER_ANIMATION_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_EMITTER_ANIMATION_CONTEXT_H

// EmitterAnimationContext: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_07_022260F8_022260F8.c) and 10 accesses; fields at every accessed offset, pads elsewhere.

struct BattleAnimSystem;
struct BgConfig;
struct ManagedSprite;
struct PaletteData;
struct ParticleSystem;
struct PokemonSprite;
struct SPLEmitter;
struct SpriteManager;
struct SpriteSystem;

typedef struct EmitterAnimationContext {
    int emitterID;
    s16 startDelay;
    s16 frames;
    int radius;
    struct { s16 x; s16 y; } offset;
    int mode;
    int startBattler;
    int endBattler;
    int frame;
    int skipFrames;
    int maxFrames;
    int timer;
    int angle;
    BOOL curve;
    struct ParticleSystem *particleSys;
    struct SPLEmitter *emitter;
    struct { u8 state; u8 counter1; u8 counter2; u8 unused_03; struct BattleAnimSystem *battleAnimSys; struct SpriteSystem *spriteSystem; struct SpriteManager *pokemonSpriteManager; struct SpriteManager *primarySpriteManager; struct BgConfig *bgConfig; struct PaletteData *paletteData; } common;
    struct { struct { s16 x; s16 y; } basePos; struct { s16 x; s16 y; } pos; struct PokemonSprite *monSprite; struct ManagedSprite *hwSprite; int battler; } unk_58[4];
    struct { s16 x; s16 y; s32 data[8]; } transforms[2];
    struct { s32 value; s32 data[4]; } unused;
} EmitterAnimationContext;

// sizeof(EmitterAnimationContext) == 0x104

#endif // POKEHEARTGOLD_OVERLAY_07_EMITTER_ANIMATION_CONTEXT_H
