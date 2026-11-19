#ifndef POKEHEARTGOLD_OVERLAY_07_STRING_SHOT_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_STRING_SHOT_SPRITE_CONTEXT_H

#include "overlay_07_battle_anim_common.h"

struct ManagedSprite;

// sizeof == 0x44 on the DS
typedef struct StringShotSpriteContext {
    u8 state;
    u8 spriteCounters[3];
    u8 delay;
    u8 spriteAlpha;
    u8 bgAlpha;
    struct BattleAnimSystem *battleAnimSystem;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    struct ManagedSprite *sprites[3];
    XYTransformContext scale;
} StringShotSpriteContext;

#endif // POKEHEARTGOLD_OVERLAY_07_STRING_SHOT_SPRITE_CONTEXT_H
