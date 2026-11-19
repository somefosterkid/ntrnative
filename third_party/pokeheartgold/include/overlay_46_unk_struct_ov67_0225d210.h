#ifndef POKEHEARTGOLD_OVERLAY_46_UNK_STRUCT_OV67_0225D210_H
#define POKEHEARTGOLD_OVERLAY_46_UNK_STRUCT_OV67_0225D210_H

// UnkStruct_ov67_0225D210: merged by tools/hg/promote.py from 5 m2c local copies (1 variants, base
// src/overlay_46_02258CB4_02258DA8.c) and 8 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct MsgData;
struct String;
struct StringTemplate;

typedef struct UnkStruct_ov67_0225D210 {
    struct StringTemplate *unk_00;
    struct MsgData *unk_04;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_08;
    struct String *unk_18;
    struct String *unk_1C;
    u32 unk_20;
    void *unk_24;
    u32 unk_28;
    u32 unk_2C;
} UnkStruct_ov67_0225D210;

// sizeof(UnkStruct_ov67_0225D210) == 0x30

#endif // POKEHEARTGOLD_OVERLAY_46_UNK_STRUCT_OV67_0225D210_H
