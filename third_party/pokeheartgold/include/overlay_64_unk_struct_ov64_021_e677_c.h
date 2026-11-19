#ifndef POKEHEARTGOLD_OVERLAY_64_UNK_STRUCT_OV64_021_E677_C_H
#define POKEHEARTGOLD_OVERLAY_64_UNK_STRUCT_OV64_021_E677_C_H

// UnkStruct_ov64_021E677C: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_64_021E677C_021E677C.c) and 3 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct HallOfFame;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct NARC;
struct String;

typedef struct UnkStruct_ov64_021E677C {
    struct HallOfFame *hof;
    u32 unk_4;
    struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } windows[16];
    u8 filler_108[16];
    struct MsgData *msgUi;
    struct MsgData *msgSpecies;
    struct MsgData *msgMoves;
    struct MessageFormat *msgFmt;
    struct String *str1;
    struct String *str2;
    u8 filler_130[8];
    struct ManagedSprite *sprites[2];
    u8 filler_140[68];
    struct NARC *narc;
    struct { struct String *nickname; struct String *otname; u32 personality; u32 otid; u16 species; u8 level; u8 form; u16 moves[4]; } mon;
    u8 filler_1A4[8];
    u32 count;
    u8 filler_1B0[4];
    int team;
    u8 filler_1B8[12];
    u16 page;
    u8 pad_1C6[0x2];
} UnkStruct_ov64_021E677C;

// sizeof(UnkStruct_ov64_021E677C) == 0x1c8

#endif // POKEHEARTGOLD_OVERLAY_64_UNK_STRUCT_OV64_021_E677_C_H
