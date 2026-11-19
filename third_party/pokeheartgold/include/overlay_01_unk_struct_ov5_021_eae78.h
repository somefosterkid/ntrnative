#ifndef POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021_EAE78_H
#define POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021_EAE78_H

// UnkStruct_ov5_021EAE78: merged by tools/hg/promote.py from 3 m2c local copies (2 variants, base
// src/overlay_01_021F6CFC_021F71C4.c) and 86 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct FieldSystem;
struct ListMenu;
struct Menu;
struct MsgData;
struct SaveData;
struct MessageFormat;
struct MsgData;
struct String;
struct StringList;

typedef struct UnkStruct_ov5_021EAE78 {
    struct StringList *unk_00;
    struct ListMenu *unk_04;
    struct String *unk_08;
    struct String *unk_0C;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_10;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_20;
    struct FieldSystem *fieldSystem;
    struct SaveData *saveData;
    struct MessageFormat *unk_38;
    struct MsgData *unk_3C;
    int unk_40;
    int unk_44;
    int unk_48;
    int unk_4C[8];
    int unk_6C[8];
    int unk_8C;
    int unk_90;
    u32 unk_94;
    u16 unk_98;
    u16 unk_9A;
} UnkStruct_ov5_021EAE78;

// sizeof(UnkStruct_ov5_021EAE78) == 0x9c

#endif // POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021_EAE78_H
