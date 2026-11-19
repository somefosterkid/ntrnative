#ifndef POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_02246BF4_H
#define POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_02246BF4_H

// UnkStruct_ov62_02246BF4: merged by tools/hg/promote.py from 58 m2c local copies (1 variants, base
// src/overlay_40_0223D544_0223D5E8_0223D618.c) and 19 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct ManagedSprite;
struct NARC;
struct Options;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct TouchScreenActions;

typedef struct UnkStruct_ov62_02246BF4 {
    struct { int unk_00; int unk_04; BOOL unk_08; struct ManagedSprite *unk_0C; void *unk_10; struct { s16 unk_00; s16 unk_02; u8 unk_04; u8 unk_05; } unk_14; u8 padding_1A[2]; } unk_00[12];
    struct { int unk_00; int unk_04; BOOL unk_08; struct ManagedSprite *unk_0C; void *unk_10; struct { s16 unk_00; s16 unk_02; u8 unk_04; u8 unk_05; } unk_14; u8 padding_1A[2]; } unk_150[2];
    struct { int unk_00; int unk_04; BOOL unk_08; struct ManagedSprite *unk_0C; void *unk_10; struct { s16 unk_00; s16 unk_02; u8 unk_04; u8 unk_05; } unk_14; u8 padding_1A[2]; } unk_188[3];
    s16 unk_1DC[4];
    u16 unk_1E4[2][4];
    int unk_1F4;
    int unk_1F8;
    int unk_1FC;
    int unk_200;
    int unk_204;
    int unk_208;
    int unk_20C;
    int unk_210;
    int unk_214;
    int unk_218;
    int unk_21C;
    struct { struct NARC *unk_00; struct SpriteSystem *unk_04; struct SpriteManager *unk_08; struct BgConfig *unk_0C; struct PaletteData *unk_10; struct TouchScreenActions *unk_14; u8 unk_18[0x60]; BOOL unk_78; } unk_220;
    struct { int unk_00; int unk_04; int unk_08; } unk_29C;
    struct { int unk_00; int unk_04[4]; u8 padding_14[4]; struct String *unk_18; struct Options *options; } unk_2A8;
    BOOL unk_2C8;
    u64 unk_2CC;
    void *unk_2D4;
} UnkStruct_ov62_02246BF4;

// sizeof(UnkStruct_ov62_02246BF4) == 0x2d8

#endif // POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_02246BF4_H
