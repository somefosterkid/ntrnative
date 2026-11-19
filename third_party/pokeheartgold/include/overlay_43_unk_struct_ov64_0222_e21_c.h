#ifndef POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_E21_C_H
#define POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_E21_C_H
#include "sprite_resources_header.h"

#include "g2d_renderer.h"

// UnkStruct_ov64_0222E21C: merged by tools/hg/promote.py from 23 m2c local copies (1 variants, base
// src/overlay_43_0222A154_0222A154.c) and 21 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct MsgData;
struct NARC;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct NNSG2dCharacterData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dMultiCellDataBank;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct NNSG2dScreenData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct StringTemplate;

typedef struct UnkStruct_ov64_0222E21C {
    struct BgConfig *unk_00;
    struct SpriteList *unk_04;
    NNSG2dRenderSurface unk_08;
    G2dRenderer unk_78;
    struct SpriteResourceCollection *unk_204[4];
    struct StringTemplate *unk_214;
    struct MsgData *unk_218;
    struct NARC *unk_21C;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_220;
    struct String *unk_230;
    struct String *unk_234;
    u32 unk_238;
    SpriteResourcesHeader unk_23C;
    struct SpriteResource *unk_260[4];
    struct Sprite *unk_270[2];
    struct { void *unk_00[2]; struct NNSG2dScreenData *unk_08[2]; } unk_278;
    u32 unk_1FC[4];
    u32 unk_20C;
} UnkStruct_ov64_0222E21C;

// sizeof(UnkStruct_ov64_0222E21C) == 0x210

#endif // POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_E21_C_H
