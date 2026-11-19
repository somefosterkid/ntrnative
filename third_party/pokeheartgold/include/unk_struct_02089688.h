#ifndef POKEHEARTGOLD_UNK_STRUCT_02089688_H
#define POKEHEARTGOLD_UNK_STRUCT_02089688_H

// UnkStruct_02089688: merged by tools/hg/promote.py from 14 m2c local copies (1 variants, base
// src/unk_02085604_02085688.c) and 43 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct FontOAM;
struct ManagedSprite;
struct NARC;
struct Options;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct TouchScreenActions;
struct UnkStruct_02012744_t;

typedef struct UnkStruct_02089688 {
    struct { int unk_00; int unk_04; BOOL unk_08; struct ManagedSprite *unk_0C; void *unk_10; struct { s16 unk_00; s16 unk_02; u8 unk_04; u8 unk_05; } unk_14; u8 unk_1A_tail[0x2]; } unk_00[16];
    struct { int unk_00; int unk_04; BOOL unk_08; struct ManagedSprite *unk_0C; void *unk_10; struct { s16 unk_00; s16 unk_02; u8 unk_04; u8 unk_05; } unk_14; u8 unk_1A_tail[0x2]; } unk_1C0[3];
    struct { int unk_00; int unk_04; BOOL unk_08; struct ManagedSprite *unk_0C; void *unk_10; struct { s16 unk_00; s16 unk_02; u8 unk_04; u8 unk_05; } unk_14; u8 unk_1A_tail[0x2]; } unk_214[3];
    struct { int unk_00; int unk_04; BOOL unk_08; struct ManagedSprite *unk_0C; void *unk_10; struct { s16 unk_00; s16 unk_02; u8 unk_04; u8 unk_05; } unk_14; u8 unk_1A_tail[0x2]; } unk_268[2];
    s16 unk_2A0[5];
    u16 unk_2AA[2][5];
    u8 pad_2BE[0x2];
    int unk_2C0;
    int unk_2C4;
    int unk_2C8;
    int unk_2CC;
    int unk_2D0;
    int unk_2D4;
    int unk_2D8;
    int unk_2DC;
    int unk_2E0;
    int unk_2E4;
    int unk_2E8;
    struct { struct NARC *unk_00; struct SpriteSystem *unk_04; struct SpriteManager *unk_08; struct BgConfig *unk_0C; struct PaletteData *unk_10; struct TouchScreenActions *unk_14; u8 unk_18[0x70]; BOOL unk_88; struct UnkStruct_02012744_t *unk_8C; struct FontOAM *unk_90[2]; struct { u32 size; u32 offset; u16 vram; u16 isAtEnd; } unk_98[2]; struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } unk_B0; } unk_2EC;
    struct { int unk_00; int unk_04; int unk_08; } unk_3AC;
    struct { int unk_00; int unk_04[5]; int unk_18; struct String *unk_1C; struct Options *options; int unk_24; u32 unk_28; u32 unk_2C; u32 unk_30; } unk_3B8;
    int unk_3EC;
    int unk_3F0;
} UnkStruct_02089688;

// sizeof(UnkStruct_02089688) == 0x3f4

#endif // POKEHEARTGOLD_UNK_STRUCT_02089688_H
