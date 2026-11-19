#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_ICON_WORK_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_ICON_WORK_H

#include "constants/heap.h"

#include "g2d_renderer.h"
#include "sprite.h"
#include "sprite_resources_header.h"

// Pokeathlon course participant icon display (Heap_Alloc(heapID, 0x244) in ov96_021FC188), twelve icons.

typedef struct Ov96IconSpec {
    u16 species; // 0x0
    u16 form;    // 0x2
} Ov96IconSpec; // size = 0x4

typedef struct Ov96Icon {
    Sprite *monIcon;  // 0x0
    Sprite *teamMark; // 0x4
    Sprite *rank;     // 0x8
    u32 unkC;         // 0xC
} Ov96Icon; // size = 0x10

typedef struct Ov96IconWork {
    enum HeapID heapID;                   // 0x000
    SpriteList *spriteList;               // 0x004
    G2dRenderer renderer;                 // 0x008
    struct GF_2DGfxResMan *resMan[6];     // 0x130
    struct SpriteResource *charRes;       // 0x148
    struct SpriteResource *plttRes;       // 0x14C
    struct SpriteResource *cellRes;       // 0x150
    struct SpriteResource *animRes;       // 0x154
    u8 unk158[8];                         // 0x158
    SpriteResourcesHeader resHeader;      // 0x160
    Ov96Icon icons[12];                   // 0x184
} Ov96IconWork; // size = 0x244

Ov96IconWork *ov96_021FC188(enum HeapID heapID);
void ov96_021FC1A8(Ov96IconWork *work);
void ov96_021FC214(Ov96IconWork *work, Ov96IconSpec *specs);
void ov96_021FC248(Ov96IconWork *work, s32 idx, s32 x);
void ov96_021FC28C(Ov96IconWork *work, s32 idx, u32 rank);
void ov96_021FC2B4(Ov96IconWork *work, BOOL visible);
void ov96_021FC2E0(Ov96IconWork *work, s32 idx, BOOL visible);
void ov96_021FC314(Ov96IconWork *work);
void ov96_021FC320(Ov96IconWork *work);
void ov96_021FC404(Ov96IconWork *work);
void ov96_021FC450(Ov96IconWork *work, Ov96IconSpec *specs);
void ov96_021FC5E0(SpriteTemplate *template, Ov96IconWork *work, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE vram);

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_ICON_WORK_H
