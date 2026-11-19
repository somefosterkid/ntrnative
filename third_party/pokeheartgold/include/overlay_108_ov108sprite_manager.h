#ifndef POKEHEARTGOLD_OVERLAY_108_OV108SPRITE_MANAGER_H
#define POKEHEARTGOLD_OVERLAY_108_OV108SPRITE_MANAGER_H

#include "constants/heap.h"

#include "g2d_renderer.h"

// Ov108SpriteManager: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_108_021E867C_021E867C.c) and 4 accesses; fields at every accessed offset, pads elsewhere.

struct GF_2DGfxResMan;
struct GF_2DGfxResObjList;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct SpriteList;

typedef struct Ov108SpriteManager {
    enum HeapID heapID;
    u8 unk4[8];
    u16 maxSprites;
    u8 unkE[2];
    struct SpriteList *spriteList;
    u8 unk14[4];
    G2dRenderer renderer;
    u8 unk18_pad[4];
    struct GF_2DGfxResMan *resMan[4];
    struct GF_2DGfxResObjList *resList[4];
} Ov108SpriteManager;

// sizeof(Ov108SpriteManager) == 0x164

#endif // POKEHEARTGOLD_OVERLAY_108_OV108SPRITE_MANAGER_H
