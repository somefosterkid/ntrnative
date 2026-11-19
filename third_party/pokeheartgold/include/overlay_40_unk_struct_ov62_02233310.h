#ifndef POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_02233310_H
#define POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_02233310_H

// UnkStruct_ov62_02233310: merged by tools/hg/promote.py from 220 m2c local copies (1 variants, base
// src/overlay_40_0222B6E0.c) and 44 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct ManagedSprite;
struct MsgData;

typedef struct UnkStruct_ov62_02233310 {
    int unk_00;
    int unk_04;
    s16 unk_08;
    u8 padding_0A[2];
    int unk_0C;
    int unk_10;
    int unk_14;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_18;
    void *unk_28;
    struct ManagedSprite *unk_2C[2];
    struct MsgData *unk_34;
    struct { int unk_00; int unk_04; int unk_08; int unk_0C; } unk_38;
    BOOL unk_48;
    u8 padding_4C[4];
} UnkStruct_ov62_02233310;

// sizeof(UnkStruct_ov62_02233310) == 0x50

#endif // POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_02233310_H
