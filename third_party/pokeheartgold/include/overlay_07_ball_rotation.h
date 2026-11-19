#ifndef POKEHEARTGOLD_OVERLAY_07_BALL_ROTATION_H
#define POKEHEARTGOLD_OVERLAY_07_BALL_ROTATION_H

// BallRotation: merged by tools/hg/promote.py from 18 m2c local copies (1 variants, base
// src/overlay_07_02232E18_02232F58.c) and 82 accesses; fields at every accessed offset, pads elsewhere.

struct BattleMonOBJData;
struct BattleSystem;
struct ManagedSprite;
struct PaletteData;
struct PaletteFadeContext;
struct SpriteManager;
struct SpriteSystem;
struct SysTask;
struct UnkStruct_ov12_02236648_t;

typedef struct BallRotation {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    BOOL unk_1C;
    u8 unk_20;
    u8 unk_21;
    s8 unk_22;
    u8 pad_23[0x1];
    BOOL unk_24;
    BOOL unk_28;
    struct SpriteManager *unk_2C;
    struct ManagedSprite *unk_30;
    struct { s32 value; s32 data[4]; } unk_34;
    struct { s16 x; s16 y; s32 data[8]; } unk_48[2];
    struct { s32 type; u32 heapID; s32 mode; s32 target; int ball; s32 bgPrio; s32 surface; struct SpriteSystem *spriteSystem; struct PaletteData *paletteData; struct BattleSystem *battleSystem; } unk_90;
    struct { s16 unk_00; s16 unk_02; s16 unk_04; s16 unk_06; int unk_08; int unk_0C; int unk_10; } unk_B8;
    struct SysTask *unk_CC;
    struct UnkStruct_ov12_02236648_t *unk_D0;
    struct PaletteFadeContext *unk_D4;
    struct BattleMonOBJData *unk_D8;
    int unk_DC;
} BallRotation;

// sizeof(BallRotation) == 0xe0

#endif // POKEHEARTGOLD_OVERLAY_07_BALL_ROTATION_H
