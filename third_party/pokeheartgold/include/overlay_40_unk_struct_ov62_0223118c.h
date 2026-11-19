#ifndef POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_0223118C_H
#define POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_0223118C_H

// UnkStruct_ov62_0223118C: merged by tools/hg/promote.py from 218 m2c local copies (2 variants, base
// src/overlay_40_0222B6E0.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct G3DPipelineBuffers;
struct ManagedSprite;
struct MsgData;
struct NARC;
struct PaletteData;
struct PokemonSpriteManager;
struct SaveData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct StringTemplate;
struct TouchScreenActions;
struct UnkStruct_02012744_t;

typedef struct UnkStruct_ov62_0223118C {
    struct NARC *unk_00;
    struct SpriteSystem *unk_04;
    struct SpriteManager *unk_08;
    u8 padding_0C[4];
    struct BgConfig *unk_10;
    struct PaletteData *unk_14;
    struct TouchScreenActions *unk_18;
    u8 pad_1C[0x14];
    BOOL unk_30;
    struct MsgData *unk_34;
    struct MsgData *unk_38;
    struct UnkStruct_02012744_t *unk_3C;
    int unk_40;
    int unk_44;
    u8 unk_48;
    u8 padding_49[3];
    struct G3DPipelineBuffers *unk_4C;
    struct PokemonSpriteManager *unk_50;
    struct { struct SaveData *saveData; void *unk_04; void *unk_08; struct { s32 unk_00; u8 unk_04; u8 language; s8 unk_06[301]; u8 padding[5]; } unk_0C; u32 heapID; int unk_148; int unk_14C; struct { void *unk_00; void *unk_04; void *unk_08; void *unk_0C; void *unk_10; void *unk_14; void *unk_18; void *unk_1C; void *unk_20; void *unk_24; void *unk_28; } unk_150; struct { BOOL unk_00; int unk_04; int unk_08; int unk_0C; } unk_17C; void *unk_18C; struct { u8 unk_0_anon[0x21c]; u8 unk_21C[0x8]; } unk_190; void *unk_3B4; int unk_3B8; int unk_3BC; int unk_3C0; BOOL unk_3C4; BOOL unk_3C8; struct { int unk_00; int unk_04; int unk_08; } unk_3CC; int unk_3D8; int unk_3DC; int unk_3E0; int unk_3E4; int unk_3E8; int unk_3EC; int unk_3F0; struct MsgData *unk_3F4; struct StringTemplate *unk_3F8; struct String *unk_3FC; struct { void *unk_00; u16 unk_04; u16 unk_06; u8 unk_08; u8 unk_C_pad[0x3]; void *unk_0C; } unk_400; u8 unk_40D; u8 unk_40E; u8 unk_412_tail[0x2]; } unk_54;
    struct { int unk_00; BOOL unk_04; u32 unk_08; u32 unk_0C; s16 unk_10; u8 padding_12[2]; BOOL unk_14; struct ManagedSprite *unk_18[2]; } unk_46C;
    struct { int unk_00; int unk_04; s16 unk_08; u8 padding_0A[2]; int unk_0C; int unk_10; int unk_14; struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_18; void *unk_28; struct ManagedSprite *unk_2C[2]; struct MsgData *unk_34; struct { int unk_00; int unk_04; int unk_08; int unk_0C; } unk_38; BOOL unk_48; u8 padding_4C[4]; } unk_48C;
    u8 padding_4D8[24];
    struct { int unk_00; int unk_04; int unk_08; int unk_0C; int unk_10; int unk_14; BOOL unk_18; u8 padding_1C[4]; } unk_4F0;
    BOOL unk_510;
    BOOL unk_514;
    u8 pad_518[0x4];
} UnkStruct_ov62_0223118C;

// sizeof(UnkStruct_ov62_0223118C) == 0x51c

#endif // POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_0223118C_H
