#ifndef POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_F038_H
#define POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_F038_H

// UnkStruct_ov64_0222F038: merged by tools/hg/promote.py from 10 m2c local copies (1 variants, base
// src/overlay_43_0222A154_0222A154.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Menu;
struct NNSG2dScreenData;
struct String;

typedef struct UnkStruct_ov64_0222F038 {
    u32 unk_00;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_04;
    void *unk_14;
    struct NNSG2dScreenData *unk_18;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_1C;
    struct Menu *unk_2C;
    u32 unk_30;
    u32 unk_34;
    struct String *unk_38;
} UnkStruct_ov64_0222F038;

// sizeof(UnkStruct_ov64_0222F038) == 0x3c

#endif // POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_F038_H
