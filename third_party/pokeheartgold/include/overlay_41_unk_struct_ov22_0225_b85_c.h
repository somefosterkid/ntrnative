#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225_B85_C_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225_B85_C_H

#include "g2d_renderer.h"

// UnkStruct_ov22_0225B85C: merged by tools/hg/promote.py from 3 m2c local copies (1 variants, base
// src/overlay_41_0224BA10_0224BC04_0224BC04.c) and 12 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct DressUpPhoto;
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
struct Sprite;
struct SpriteList;
struct SpriteResourceCollection;
struct UnkStruct_02029C88_t;
struct UnkStruct_ov22_0225B1BC_t;
struct Window;

typedef struct UnkStruct_ov22_0225B85C {
    struct DressUpPhoto *photo;
    struct UnkStruct_02029C88_t *unk_04;
    u32 unk_08;
    u32 unk_0C;
    struct UnkStruct_ov22_0225B1BC_t *unk_10;
    struct { struct SoftwareSpriteManager *unk_00; void **unk_04; int unk_08; int unk_0C; void **unk_10; int unk_14; int unk_18; BOOL unk_1C; struct PokemonSpriteManager *unk_20; u32 unk_24; u32 unk_28; BOOL unk_2C; u8 *unk_30; struct ResourceCollection *unk_34; void **unk_38; int unk_3C; struct BgConfig *unk_40; struct SpriteList *unk_44; struct SpriteResourceCollection *unk_48[4]; G2dRenderer unk_58; struct NARC *unk_5C; } unk_14;
    struct Sprite *unk_1FC;
    struct Window *unk_200;
} UnkStruct_ov22_0225B85C;

// sizeof(UnkStruct_ov22_0225B85C) == 0x1a0

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225_B85_C_H
