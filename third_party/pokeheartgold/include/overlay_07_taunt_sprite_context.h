#ifndef POKEHEARTGOLD_OVERLAY_07_TAUNT_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_TAUNT_SPRITE_CONTEXT_H

#include "overlay_07_battle_anim_common.h"

struct ManagedSprite;

// sizeof == 0x44 on the DS
typedef struct TauntSpriteContext {
    u8 state;
    u8 fadeInCycles[3];
    u8 fadeInFrames;
    u8 handCount;
    u8 spriteAlpha;
    u8 bgAlpha;
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    struct ManagedSprite *handSprites[3];
    XYTransformContext unused;
} TauntSpriteContext;

#endif // POKEHEARTGOLD_OVERLAY_07_TAUNT_SPRITE_CONTEXT_H
