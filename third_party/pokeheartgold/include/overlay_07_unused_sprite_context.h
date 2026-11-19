#ifndef POKEHEARTGOLD_OVERLAY_07_UNUSED_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_UNUSED_SPRITE_CONTEXT_H

#include "overlay_07_afterimage_context.h"
#include "overlay_07_battle_anim_common.h"

struct ManagedSprite;
struct Pokepic;

// sizeof == 0x11C on the DS
typedef struct UnusedSpriteContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    int state;
    struct Pokepic *defenderSprite;
    XYTransformContext shake;
    s16 defenderX;
    s16 defenderY;
    struct ManagedSprite *sprites[3];
    AfterimageContext afterImage;
    XYTransformContext scale;
} UnusedSpriteContext;

#endif // POKEHEARTGOLD_OVERLAY_07_UNUSED_SPRITE_CONTEXT_H
