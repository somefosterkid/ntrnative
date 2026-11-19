#ifndef POKEHEARTGOLD_OVERLAY_91_UNK_STRUCT_OV115_02261_ADC_H
#define POKEHEARTGOLD_OVERLAY_91_UNK_STRUCT_OV115_02261_ADC_H
#include "sprite_resources_header.h"

#include "g2d_renderer.h"

// UnkStruct_ov115_02261ADC: merged by tools/hg/promote.py from 47 m2c local copies (1 variants, base
// src/overlay_91_0225CDF4.c) and 19 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Camera;
struct FontOAM;
struct G3DPipelineBuffers;
struct MsgData;
struct NNSFndAllocatorFunc;
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
struct NNSG3dAnmObj_;
struct NNSG3dJntAnmResult_;
struct NNSG3dMatAnmResult_;
struct NNSG3dResMdlSet_;
struct NNSG3dResMdl_;
struct NNSG3dResTex_;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct StringTemplate;
struct UnkStruct_02012744_t;
struct UnkStruct_02012B20_t;
struct UnkStruct_ov114_02025CAD4_t;

typedef struct UnkStruct_ov115_02261ADC {
    struct BgConfig *unk_00;
    struct StringTemplate *unk_04;
    struct MsgData *unk_08;
    struct String *unk_0C;
    struct String *unk_10;
    u16 unk_14;
    u16 unk_16;
    struct G3DPipelineBuffers *unk_18;
    struct SpriteList *unk_1C;
    G2dRenderer unk_20;
    struct SpriteResourceCollection *unk_1AC[4];
    SpriteResourcesHeader unk_1BC;
    struct SpriteResource *unk_1E0[4];
    struct UnkStruct_02012744_t *unk_1F0;
    struct Camera *camera;
    struct { fx32 x; fx32 y; fx32 z; } unk_1F8;
    struct { struct { NNSG3dRenderObj renderObj; struct { fx32 x; fx32 y; fx32 z; } position; struct { fx32 x; fx32 y; fx32 z; } scale; BOOL visible; u16 rotation[3]; u8 padding_76[2]; } unk_00[3]; struct { void *dataRaw; struct NNSG3dResMdlSet_ *modelSet; struct NNSG3dResMdl_ *model; struct NNSG3dResTex_ *texture; } unk_168[3]; struct { void *fileDataRaw; void *animRaw; struct NNSG3dAnmObj_ *animObj; fx32 frame; BOOL shouldRetainData; } unk_198[3]; fx32 unk_1D4; u8 unk_1D8; u8 unk_1D9; s16 unk_1DA; fx32 unk_1DC; fx32 unk_1E0; } unk_204;
    struct { struct { struct Sprite *unk_00[3]; u32 unk_0C[3]; struct { fx32 x; fx32 y; fx32 z; } unk_18[3]; BOOL unk_3C; } unk_00[4]; } unk_3E8;
    struct { u32 unk_00; s32 unk_04; s32 unk_08; struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } unk_0C; } unk_4E8;
    struct { u32 unk_00; u32 unk_04; struct { fx32 unk_00; fx32 unk_04; fx32 unk_08; fx32 unk_0C; int unk_10; int unk_14; } unk_08; } unk_504;
    struct { struct { NNSG3dRenderObj renderObj; struct { fx32 x; fx32 y; fx32 z; } position; struct { fx32 x; fx32 y; fx32 z; } scale; BOOL visible; u16 rotation[3]; u8 padding_76[2]; } unk_00[3]; struct { void *dataRaw; struct NNSG3dResMdlSet_ *modelSet; struct NNSG3dResMdl_ *model; struct NNSG3dResTex_ *texture; } unk_168[3]; struct { void *fileDataRaw; void *animRaw; struct NNSG3dAnmObj_ *animObj; fx32 frame; BOOL shouldRetainData; } unk_198[6]; u16 unk_210; u16 unk_212; struct { void *fileDataRaw; void *animRaw; struct NNSG3dAnmObj_ *animObj; fx32 frame; BOOL shouldRetainData; } unk_214[1]; fx32 unk_228; u16 unk_22C; u16 unk_22E; } unk_524;
    struct UnkStruct_ov114_02025CAD4_t *unk_754;
    struct { u16 unk_00; s16 unk_02; s16 unk_04; u8 unk_06; u8 unk_07; } unk_758;
    struct { struct { void *dataRaw; struct NNSG3dResMdlSet_ *modelSet; struct NNSG3dResMdl_ *model; struct NNSG3dResTex_ *texture; } unk_00[5]; struct { void *dataRaw; struct NNSG3dResMdlSet_ *modelSet; struct NNSG3dResMdl_ *model; struct NNSG3dResTex_ *texture; } unk_50; struct SpriteResource *unk_60[4]; SpriteResourcesHeader unk_70; } unk_760;
    struct { struct Sprite *unk_00; struct { NNSG3dRenderObj renderObj; struct { fx32 x; fx32 y; fx32 z; } position; struct { fx32 x; fx32 y; fx32 z; } scale; BOOL visible; u16 rotation[3]; u8 padding_76[2]; } unk_04; struct { NNSG3dRenderObj renderObj; struct { fx32 x; fx32 y; fx32 z; } position; struct { fx32 x; fx32 y; fx32 z; } scale; BOOL visible; u16 rotation[3]; u8 padding_76[2]; } unk_7C; BOOL unk_F4; void *unk_F8; u16 unk_FC; u16 unk_FE; u16 unk_100; u16 unk_102; s16 unk_104; u8 unk_106_tail[0x2]; } unk_7F4[96];
    struct { BOOL unk_00; struct Sprite *unk_04; } unk_6A34;
    struct { struct { BOOL unk_00; struct Sprite *unk_04; struct { fx32 x; fx32 y; fx32 z; } unk_08; s32 unk_14; BOOL unk_18; } unk_00[8]; } unk_6A3C;
    struct { struct SpriteResource *unk_00[4]; SpriteResourcesHeader unk_10; struct Sprite *unk_34; struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } unk_38; struct FontOAM *unk_48; struct UnkStruct_02012B20_t *unk_4C; struct { u32 size; u32 offset; u16 vram; u16 isAtEnd; } unk_50; struct String *unk_5C; struct SpriteResource *unk_60; struct { fx32 unk_00; fx32 unk_04; fx32 unk_08; fx32 unk_0C; int unk_10; int unk_14; } unk_64; struct { fx32 unk_00; fx32 unk_04; fx32 unk_08; fx32 unk_0C; int unk_10; int unk_14; } unk_7C; s16 unk_94; s16 unk_96; u16 unk_98; s16 unk_9A; u32 heapID; } unk_6B1C;
    struct { struct NNSFndAllocatorFunc *pFunc; void *pHeap; u32 heapParam1; u32 heapParam2; } unk_6BBC;
} UnkStruct_ov115_02261ADC;

// sizeof(UnkStruct_ov115_02261ADC) == 0x6c28

#endif // POKEHEARTGOLD_OVERLAY_91_UNK_STRUCT_OV115_02261_ADC_H
