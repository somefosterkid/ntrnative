#ifndef POKEHEARTGOLD_BATTLE_CASTLE_APP_SPRITE_MANAGER_H
#define POKEHEARTGOLD_BATTLE_CASTLE_APP_SPRITE_MANAGER_H

// BattleCastleAppSpriteManager: the sprite/resource state embedded in both Battle Castle apps (SelfApp at 0x518,
// OpponentApp at 0x2C8). ov83_022471FC walks BATTLE_CASTLE_APP_SPRITE_RESOURCE_COUNT (14) character resources.
// Kept free of other includes so any header can embed it by value.

#include "g2d_renderer.h"

struct GF_2DGfxResMan;
struct SpriteList;
struct SpriteResource;

#define BATTLE_CASTLE_APP_SPRITE_RESOURCE_COUNT 14

typedef struct BattleCastleAppSpriteManager {
    struct SpriteList *spriteList;
    G2dRenderer renderer;
    struct GF_2DGfxResMan *resourceCollection[4]; // char, pltt, cell, anim
    struct SpriteResource *resources[BATTLE_CASTLE_APP_SPRITE_RESOURCE_COUNT][4]; // [resource ID][char, pltt, cell, anim]
} BattleCastleAppSpriteManager;

// sizeof(BattleCastleAppSpriteManager) == 0x21C

#endif // POKEHEARTGOLD_BATTLE_CASTLE_APP_SPRITE_MANAGER_H
