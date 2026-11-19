#ifndef POKEHEARTGOLD_SPRITE_RESOURCE_MANAGER_H
#define POKEHEARTGOLD_SPRITE_RESOURCE_MANAGER_H

#include "g2d_renderer.h"

// SpriteResourceManager: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/render_window_0200F54C_0200F600_0200F600.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct SpriteList;
struct SpriteResourceCollection;
struct SpriteResourceList;
struct SpriteResourcesHeaderList;

typedef struct SpriteResourceManager {
    struct SpriteList *spriteList;
    G2dRenderer renderer;
    struct SpriteResourcesHeaderList *resourceHeaderList;
    struct SpriteResourceCollection *resourceCollections[6];
    struct SpriteResourceList *resourceLists[6];
    u16 resourceTypeCount;
    u16 heapID;
} SpriteResourceManager;

// sizeof(SpriteResourceManager) == 0x164

#endif // POKEHEARTGOLD_SPRITE_RESOURCE_MANAGER_H
