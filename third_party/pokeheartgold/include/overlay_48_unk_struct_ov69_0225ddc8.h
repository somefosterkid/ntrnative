#ifndef POKEHEARTGOLD_OVERLAY_48_UNK_STRUCT_OV69_0225DDC8_H
#define POKEHEARTGOLD_OVERLAY_48_UNK_STRUCT_OV69_0225DDC8_H

// UnkStruct_ov69_0225DDC8: merged by tools/hg/promote.py from 9 m2c local copies (1 variants, base
// src/overlay_48_02258800.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct String;
struct YesNoTouchMenu;

typedef struct UnkStruct_ov69_0225DDC8 {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    struct String *unk_0C;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_10;
    struct YesNoTouchMenu *unk_20;
} UnkStruct_ov69_0225DDC8;

// sizeof(UnkStruct_ov69_0225DDC8) == 0x24

#endif // POKEHEARTGOLD_OVERLAY_48_UNK_STRUCT_OV69_0225DDC8_H
