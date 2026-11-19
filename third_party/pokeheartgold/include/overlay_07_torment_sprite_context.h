#ifndef POKEHEARTGOLD_OVERLAY_07_TORMENT_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_TORMENT_SPRITE_CONTEXT_H

// TormentSpriteContext: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/overlay_07_0222EF58_0222EF88.c) and 25 accesses; fields at every accessed offset, pads elsewhere.

struct BattleAnimSystem;
struct ManagedSprite;
struct Pokepic;
struct SpriteManager;
struct SpriteSystem;

typedef struct TormentSpriteContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    int state;
    int curSpeechBubble;
    int unused;
    struct Pokepic *attackerSprite;
    struct { s16 x; s16 y; s32 data[8]; } scale;
    int attackerScaleState;
    s16 attackerY;
    s16 attackerOffset;
    struct ManagedSprite *speechBubbleSprites[6];
    int speechBubbleStates[6];
    int speechBubbleEndDelays[6];
} TormentSpriteContext;

// sizeof(TormentSpriteContext) == 0x90

#endif // POKEHEARTGOLD_OVERLAY_07_TORMENT_SPRITE_CONTEXT_H
