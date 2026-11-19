#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021E9A78_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021E9A78_H

#include "constants/heap.h"

struct GF_2DGfxResMan;
struct SpriteResource;

// UnkStruct_ov96_021E9A78: the 0xb4-byte sprite resource source allocated by ov96_021E9A78 and freed by
// ov96_021E9C0C. flags selects which of the 12 char/cell/anim resource sets get loaded (bit order 1, 4, 2, 8, 0x10, ...).
typedef struct UnkStruct_ov96_021E9A78 {
    enum HeapID heapId;
    u32 flags;
    u32 vram; // NNS_G2D_VRAM_TYPE for the char resources and the first palette
    struct GF_2DGfxResMan *resMan[4]; // char, pltt, cell, anim
    struct SpriteResource *charRes[12];
    struct SpriteResource *cellRes[12];
    struct SpriteResource *animRes[12];
    struct SpriteResource *plttRes[2];
} UnkStruct_ov96_021E9A78;

// sizeof(UnkStruct_ov96_021E9A78) == 0xb4

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021E9A78_H
