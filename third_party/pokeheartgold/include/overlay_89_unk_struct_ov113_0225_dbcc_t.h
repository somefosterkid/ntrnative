#ifndef POKEHEARTGOLD_OVERLAY_89_UNK_STRUCT_OV113_0225_DBCC_T_H
#define POKEHEARTGOLD_OVERLAY_89_UNK_STRUCT_OV113_0225_DBCC_T_H

// UnkStruct_ov113_0225DBCC_t: merged by tools/hg/promote.py from 23 m2c local copies (1 variants, base
// src/overlay_89_02258800.c) and 47 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Camera;
struct FontOAM;
struct G3DPipelineBuffers;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct NARC;
struct NNSG3dAnmObj_;
struct NNSG3dJntAnmResult_;
struct NNSG3dMatAnmResult_;
struct NNSG3dResMdlSet_;
struct NNSG3dResMdl_;
struct NNSG3dResTex_;
struct PaletteData;
struct SaveData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct SysTask;
struct UnkStruct_02012744_t;
struct UnkStruct_ov113_0225E6B8_t;
struct YesNoPrompt;

typedef struct UnkStruct_ov113_0225DBCC_t {
    void *unk_00;
    struct SaveData *saveData;
    struct BgConfig *unk_08;
    struct PaletteData *unk_0C;
    struct UnkStruct_02012744_t *unk_10;
    struct G3DPipelineBuffers *unk_14;
    struct SysTask *unk_18;
    struct SpriteSystem *unk_1C;
    struct SpriteManager *unk_20;
    struct YesNoPrompt *unk_24;
    u8 unk_28;
    u8 unk_29;
    u8 pad_2A[0x2];
    struct MessageFormat *unk_2C;
    struct MsgData *unk_30;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_34[8];
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_B4;
    struct String *unk_C4;
    u8 unk_C8;
    u8 pad_C9[0x3];
    struct Camera *camera;
    fx32 unk_D0;
    fx32 unk_D4;
    struct { struct { void *data; struct NNSG3dResMdlSet_ *set; struct NNSG3dResMdl_ *model; struct NNSG3dResTex_ *texture; } unk_00; struct { NNSG3dRenderObj renderObj; struct { fx32 x; fx32 y; fx32 z; } position; struct { fx32 x; fx32 y; fx32 z; } scale; BOOL visible; u16 rotation[3]; u8 padding_76[2]; } unk_10; } unk_D8;
    struct NARC *unk_160;
    struct NARC *unk_164;
    struct { BOOL unk_00; s32 unk_04; } unk_168;
    s32 unk_170[8];
    int unk_190;
    struct { struct { struct { fx32 unk_00; s16 unk_04; u8 unk_06; u8 unk_07; } unk_00; struct { struct { fx32 x; fx32 y; fx32 z; } unk_00; u8 unk_0C; u8 unk_0D; u8 unk_0E; u8 unk_0F; } unk_08; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; s32 unk_08; s16 unk_0C; u8 unk_0E; u8 unk_0F; } unk_18; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; s32 unk_08; s16 unk_0C; u8 unk_0E; u8 unk_0F; } unk_28; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; s32 unk_08; s16 unk_0C; u8 unk_0E; u8 unk_0F; } unk_38; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; fx32 unk_08; u8 unk_0C; u8 unk_0D; u8 unk_0E[2]; } unk_48; struct { fx32 unk_00; s16 unk_04; u8 unk_06; u8 unk_07; } unk_58; struct { struct { fx32 x; fx32 y; fx32 z; } unk_00; struct { fx32 x; fx32 y; fx32 z; } unk_0C; fx32 unk_18; u8 unk_1C; u8 unk_1D; u8 unk_1E[2]; } unk_60; struct { struct { fx32 x; fx32 y; fx32 z; } unk_00; struct { fx32 x; fx32 y; fx32 z; } unk_0C; fx32 unk_18; u8 unk_1C; u8 unk_1D; u8 unk_1E[2]; } unk_80; struct { struct { u16 x; u16 y; u16 z; u16 padding_06; } cameraAngle; s32 unk_08; s16 unk_0C; u8 unk_0E; u8 unk_0F; } unk_A0; } unk_00; struct { u8 unk_00; u8 unk_01; s16 unk_02; } unk_B0; struct UnkStruct_ov113_0225E6B8_t *unk_B4[128]; struct UnkStruct_ov113_0225E6B8_t *unk_2B4[128]; struct NNSG3dResTex_ *unk_4B4[32]; u32 unk_534[2]; struct { u8 unk_00; u8 unk_01; u16 unk_02; } unk_53C[128]; u8 unk_73C; u8 unk_73D; u8 unk_73E; u8 unk_73F; u8 unk_740; u8 unk_742_pad[0x1]; s16 unk_742; } unk_194;
    struct { u16 unk_00; u16 unk_02; u32 unk_04; u8 unk_08; u8 unk_09; u8 unk_0A; u8 unk_0B; } unk_8D8[6];
    s8 unk_920;
    u8 unk_921;
    u8 pad_922[0x2];
    struct ManagedSprite *unk_924[6];
    struct ManagedSprite *unk_93C[6];
    struct ManagedSprite *unk_954[6];
    struct ManagedSprite *unk_96C;
    struct ManagedSprite *unk_970[8];
    struct ManagedSprite *unk_990[3];
    u16 unk_99C[8];
    u16 unk_9AC[8];
    int unk_9BC;
    u16 unk_9C0[2048];
    struct { struct FontOAM *unk_00; struct { u32 size; u32 offset; u16 vramType; u16 atEnd; } unk_04; u16 unk_10; u8 unk_12_tail[0x2]; } unk_19C0;
    int unk_19D4;
    int unk_19D8;
    int unk_19DC;
    BOOL unk_19E0;
} UnkStruct_ov113_0225DBCC_t;

// sizeof(UnkStruct_ov113_0225DBCC_t) == 0x19e4

#endif // POKEHEARTGOLD_OVERLAY_89_UNK_STRUCT_OV113_0225_DBCC_T_H
