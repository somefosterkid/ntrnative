#ifndef POKEHEARTGOLD_OVERLAY_47_UNK_STRUCT_OV68_0225_D128_H
#define POKEHEARTGOLD_OVERLAY_47_UNK_STRUCT_OV68_0225_D128_H

// UnkStruct_ov68_0225D128: merged by tools/hg/promote.py from 14 m2c local copies (1 variants, base
// src/overlay_47_02258800.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Menu;
struct String;

typedef struct UnkStruct_ov68_0225D128 {
    u32 unk_00;
    s32 unk_04;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_08;
    struct String *unk_18;
    void *unk_1C;
    struct Menu *unk_20;
} UnkStruct_ov68_0225D128;

// sizeof(UnkStruct_ov68_0225D128) == 0x24

#endif // POKEHEARTGOLD_OVERLAY_47_UNK_STRUCT_OV68_0225_D128_H
