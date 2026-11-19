#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225_A0_E4_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225_A0_E4_H

#include "g2d_renderer.h"

// UnkStruct_ov22_0225A0E4: merged by tools/hg/promote.py from 39 m2c local copies (1 variants, base
// src/overlay_41_02246130_02246170.c) and 65 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct NARC;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct PokemonSpriteManager;
struct ResourceCollection;
struct SoftwareSpriteManager;
struct SpriteList;
struct SpriteResourceCollection;
struct UnkStruct_ov22_02254DE0;

typedef struct UnkStruct_ov22_0225A0E4 {
    struct SoftwareSpriteManager *unk_00;
    void **unk_04;
    int unk_08;
    int unk_0C;
    void **unk_10;
    int unk_14;
    int unk_18;
    BOOL unk_1C;
    struct PokemonSpriteManager *unk_20;
    u32 unk_24;
    u32 unk_28;
    BOOL unk_2C;
    u8 *unk_30;
    struct ResourceCollection *unk_34;
    void **unk_38;
    int unk_3C;
    struct BgConfig *unk_40;
    struct SpriteList *unk_44;
    struct SpriteResourceCollection *unk_48[4];
    G2dRenderer unk_58;
    struct NARC *unk_5C;
    u8 pad_184[0x1e0]; // 0x184..0x364 only exists in UnkStruct_ov22_0225B1BC (the 0x268-byte portrait object)
    struct UnkStruct_ov22_02254DE0 *unk_364;
    u8 pad_368[0x348];
    u32 unk_6B0;
    u8 pad_6B4[0x4];
    u32 unk_6B8;
    u32 unk_6BC;
    u32 unk_6C0;
    u8 pad_6C4[0x18];
    u32 unk_6DC;
    u8 pad_6E0[0x8];
    u32 unk_6E8;
    u32 unk_6EC;
} UnkStruct_ov22_0225A0E4;

// sizeof(UnkStruct_ov22_0225A0E4) == 0x6f0

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225_A0_E4_H
