#ifndef POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_F0_C4_H
#define POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_F0_C4_H

// UnkStruct_ov64_0222F0C4: merged by tools/hg/promote.py from 13 m2c local copies (3 variants, base
// src/overlay_43_0222B0A0_0222B5D0_0222B5D0.c) and 49 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct ListMenu;
struct Menu;
struct Sprite;
struct String;
struct StringList;
struct UnkStruct_ov63_0222AE60_t;
struct UnkStruct_ov63_0222BB38_t;

typedef struct UnkStruct_ov64_0222F0C4 {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    struct { struct { s16 unk_00; s16 unk_02; u8 unk_04; u8 unk_05; } unk_00; u8 unk_8_pad[0x2]; struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_08; struct UnkStruct_ov63_0222BB38_t *unk_18[8]; } unk_08[3];
    s16 unk_AA;
    u16 unk_AC;
    struct UnkStruct_ov63_0222AE60_t *unk_B0;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_B4;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_C4;
    struct String *unk_D4;
    u32 unk_D8;
    u32 unk_DC;
    struct StringList *unk_F0[2];
    struct TouchscreenListMenu *unk_F8;
    struct Menu *unk_FC;
    s32 unk_100;
    struct Sprite *unk_104;
    u32 unk_108;
    struct Menu *unk_100_;
    u8 pad_104[0x4];
    struct Sprite *unk_108_;
    u8 pad_10C[0x4];
} UnkStruct_ov64_0222F0C4;

// sizeof(UnkStruct_ov64_0222F0C4) == 0x110

#endif // POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_F0_C4_H
