#ifndef POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_0225_E4_EC_H
#define POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_0225_E4_EC_H

#include "g2d_renderer.h"

// UnkStruct_ov70_0225E4EC: merged by tools/hg/promote.py from 34 m2c local copies (1 variants, base
// src/overlay_49_02259AA4_02259AA4.c) and 4 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct G3DPipelineBuffers;
struct NNSG2dCellTransferState;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct SpriteList;
struct SpriteResourceCollection;

typedef struct UnkStruct_ov70_0225E4EC {
    struct BgConfig *unk_00;
    struct SpriteList *unk_04;
    G2dRenderer unk_08;
    struct SpriteResourceCollection *unk_194[4];
    struct NNSG2dCellTransferState *unk_1A4;
    struct G3DPipelineBuffers *unk_1A8;
} UnkStruct_ov70_0225E4EC;

// sizeof(UnkStruct_ov70_0225E4EC) == 0x148

#endif // POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_0225_E4_EC_H
