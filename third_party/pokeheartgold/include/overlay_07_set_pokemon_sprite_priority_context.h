#ifndef POKEHEARTGOLD_OVERLAY_07_SET_POKEMON_SPRITE_PRIORITY_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_SET_POKEMON_SPRITE_PRIORITY_CONTEXT_H

// SetPokemonSpritePriorityContext: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/overlay_07_02226CB0.c) and 6 accesses; fields at every accessed offset, pads elsewhere.

struct BattleAnimSystem;
struct BgConfig;
struct ManagedSprite;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;

typedef struct SetPokemonSpritePriorityContext {
    struct { u8 state; u8 counter1; u8 counter2; u8 unused_03; struct BattleAnimSystem *battleAnimSys; struct SpriteSystem *spriteSystem; struct SpriteManager *pokemonSpriteManager; struct SpriteManager *primarySpriteManager; struct BgConfig *bgConfig; struct PaletteData *paletteData; } common;
    struct ManagedSprite *sprite;
    int spriteID;
    int maxFrames;
    int bg;
    int spritePriority;
    int mode;
    int windowType;
    int stepCount;
    int sinkFullyStartFrame;
} SetPokemonSpritePriorityContext;

// sizeof(SetPokemonSpritePriorityContext) == 0x40

#endif // POKEHEARTGOLD_OVERLAY_07_SET_POKEMON_SPRITE_PRIORITY_CONTEXT_H
