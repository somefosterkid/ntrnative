#ifndef POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_02241204_H
#define POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_02241204_H

#include "bg_window.h"
#include "touchscreen.h"
#include "overlay_40_unk_struct_ov62_022312b0.h"
#include "overlay_40_unk_struct_ov62_02246bf4.h"

// UnkStruct_ov62_02241204: merged by tools/hg/promote.py from 43 m2c local copies (1 variants, base
// src/overlay_40_0223D544_0223D5E8_0223D618.c) and 4 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct TextOBJ;
struct ManagedSprite;
struct MsgData;
struct NARC;
struct Options;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct TouchHitboxController;
struct UnkStruct_02012B20_t;
struct UnkStruct_0202F41C_t;
struct UnkStruct_02030A80_t;

typedef struct UnkStruct_ov62_022323B8 { struct UnkStruct_02030A80_t *unk_00; struct ManagedSprite *unk_04; struct ManagedSprite *unk_08; struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } unk_0C[8]; } UnkStruct_ov62_022323B8;

typedef struct UnkStruct_ov62_02233F74 { struct UnkStruct_02030A80_t *unk_00; struct UnkStruct_0202F41C_t *unk_04; u8 padding_08[4]; struct ManagedSprite *unk_0C[12]; struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } unk_3C[8]; } UnkStruct_ov62_02233F74;

typedef struct UnkStruct_ov62_0223E01C_sub1 { int unk_00; int unk_04; u16 *unk_08; struct MsgData *unk_0C; BOOL unk_10; } UnkStruct_ov62_0223E01C_sub1;

typedef struct UnkStruct_ov62_02249380 {
    int unk_00;
    int unk_04;
    u64 unk_08;
} UnkStruct_ov62_02249380;

typedef struct UnkStruct_ov62_02248CDC { const UnkStruct_ov62_02249380 *unk_00; int unk_04; int unk_08; int unk_0C; int unk_10; int unk_14; int unk_18; int unk_1C; int unk_20; u32 unk_24; const TouchscreenHitbox *unk_28; } UnkStruct_ov62_02248CDC;

typedef struct UnkStruct_ov62_0223E01C_sub1_sub1 {
    int unk_00;
    struct ManagedSprite *unk_04;
} UnkStruct_ov62_0223E01C_sub1_sub1;

typedef struct UnkStruct_ov62_0223E01C_sub2 {
    struct NARC *unk_00;
    int unk_04;
    int unk_08;
    UnkStruct_ov62_0223E01C_sub1_sub1 unk_0C[5][6];
} UnkStruct_ov62_0223E01C_sub2;

typedef struct UnkStruct_ov62_02241204 {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    UnkStruct_ov62_022312B0 unk_10[4];
    UnkStruct_ov62_022323B8 unk_80;
    UnkStruct_ov62_02233F74 unk_10C;
    int unk_1C8;
    int unk_1CC;
    struct UnkStruct_02030A80_t *unk_1D0;
    UnkStruct_ov62_02246BF4 unk_1D4;
    void *unk_4AC;
    u64 unk_4B0;
    BOOL unk_4B8;
    void *unk_4BC;
    u16 unk_4C0;
    u8 unk_4C2;
    u8 unk_4C3;
    u8 unk_4C4;
    u8 unk_4C5;
    u8 unk_4C6[2];
    UnkStruct_ov62_0223E01C_sub1 unk_4C8;
    UnkStruct_ov62_02249380 *unk_4DC;
    UnkStruct_ov62_02248CDC unk_4E0;
    UnkStruct_ov62_0223E01C_sub2 unk_50C;
    struct TouchHitboxController *unk_608[3];
    Window unk_614[12];
    Window unk_6D4[12];
    int unk_794;
    int unk_798;
    u8 unk_79C;
    u8 pad_79D[0x3];
} UnkStruct_ov62_02241204;

// sizeof(UnkStruct_ov62_02241204) == 0x7a0

#endif // POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_02241204_H
