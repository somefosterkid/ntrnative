#ifndef POKEHEARTGOLD_OVERLAY_46_UNK_STRUCT_OV67_0225D154_H
#define POKEHEARTGOLD_OVERLAY_46_UNK_STRUCT_OV67_0225D154_H

// UnkStruct_ov67_0225D154: merged by tools/hg/promote.py from 3 m2c local copies (1 variants, base
// src/overlay_46_02258CB4_02258DA8.c) and 12 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Menu;
struct MsgData;
struct SaveData;
struct String;
struct StringTemplate;
struct SysTask;

typedef struct UnkStruct_ov67_0225D154 {
    struct SaveData *saveData;
    u32 unk_04;
    u32 unk_08;
    struct BgConfig *unk_0C;
    struct { struct StringTemplate *unk_00; struct MsgData *unk_04; struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_08; struct String *unk_18; struct String *unk_1C; u32 unk_20; void *unk_24; u32 unk_28; u32 unk_2C; } unk_10;
    struct { struct StringTemplate *unk_00; struct MsgData *unk_04; struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_08; struct String *unk_18; struct String *unk_1C; u32 unk_20; void *unk_24; u32 unk_28; u32 unk_2C; } unk_40;
    struct { struct StringTemplate *unk_00; struct MsgData *unk_04; struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_08; struct String *unk_18; struct String *unk_1C; u32 unk_20; void *unk_24; u32 unk_28; u32 unk_2C; } unk_70;
    struct { struct StringTemplate *unk_00; struct MsgData *unk_04; struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_08; struct String *unk_18; struct String *unk_1C; u32 unk_20; void *unk_24; u32 unk_28; u32 unk_2C; } unk_A0;
    struct Menu *unk_D0;
    struct { struct SysTask *unk_00; BOOL unk_04; u16 unk_08[16][4]; u16 unk_88[16][21]; s16 unk_328; s8 unk_32A; u8 unk_32B; } unk_D4;
} UnkStruct_ov67_0225D154;

// sizeof(UnkStruct_ov67_0225D154) == 0x400

#endif // POKEHEARTGOLD_OVERLAY_46_UNK_STRUCT_OV67_0225D154_H
