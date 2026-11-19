#ifndef POKEHEARTGOLD_OVERLAY_98_SPRITE_CTX_H
#define POKEHEARTGOLD_OVERLAY_98_SPRITE_CTX_H

#include "global.h"
#include "constants/heap.h"

#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"

// The sprite system context ov98_0221E5E0 allocates and the pokeathlon apps (overlay 99) hold.
typedef struct PokeathlonSpriteContext {
    enum HeapID heapId;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *paletteData;
    NARC *narc;
    s8 paletteOffset;
} PokeathlonSpriteContext;

// sizeof(PokeathlonSpriteContext) == 0x18

PokeathlonSpriteContext *ov98_0221E5E0(enum HeapID heapId, SpriteResourceCountsListUnion *counts, int maxSprites);
SpriteSystem *ov98_0221E5C0(PokeathlonSpriteContext *ctx);
SpriteManager *ov98_0221E5D0(PokeathlonSpriteContext *ctx);
void ov98_0221E684(PokeathlonSpriteContext *ctx, ManagedSprite **sprites, u32 count);
void ov98_0221E6CC(PokeathlonSpriteContext *ctx);
ManagedSprite *ov98_0221E6E0(PokeathlonSpriteContext *ctx, ManagedSpriteTemplate *tmpl);
void ov98_0221E6F0(PokeathlonSpriteContext *ctx, ManagedSprite **sprites, const int *resIds, u32 count, int start, s16 x, s16 y, BOOL isMain);

#endif // POKEHEARTGOLD_OVERLAY_98_SPRITE_CTX_H
