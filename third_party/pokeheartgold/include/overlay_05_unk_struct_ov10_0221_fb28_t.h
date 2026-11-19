#ifndef POKEHEARTGOLD_OVERLAY_05_UNK_STRUCT_OV10_0221_FB28_T_H
#define POKEHEARTGOLD_OVERLAY_05_UNK_STRUCT_OV10_0221_FB28_T_H

#include "bg_window.h"

// UnkStruct_ov10_0221FB28_t: merged by tools/hg/promote.py from 20 m2c local copies (1 variants, base
// src/overlay_05_0221BA70.c) and 148 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct GF3DVramMan;
struct ListMenu2D;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct PaletteData;
struct ParticleSystem;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct TextOBJ;
struct WaitingIcon;
struct TrainerIntroData;
struct UnkStruct_02013534;

struct UnkStruct_ov10_0221FB28_t;
typedef struct UnkStruct_ov10_0221FB28_t UnkStruct_ov10_0221FB28;
typedef struct UnkStruct_ov10_0221FB28_t UnkStruct_ov10_0221FB28_t;
typedef u8 (*UnkFuncPtr_ov10_02222AD0)(UnkStruct_ov10_0221FB28 *);

struct UnkStruct_ov10_0221FB28_t {
    struct TrainerIntroData *trainerIntroData;
    const UnkFuncPtr_ov10_02222AD0 *unk_04;
    struct PaletteData *unk_08;
    struct BgConfig *unk_0C;
    Window unk_10[24];
    struct SpriteSystem *unk_190;
    struct SpriteManager *unk_194;
    struct ManagedSprite *unk_198[30];
    u32 unk_210;
    struct { u32 unk_00; u16 unk_04; u16 unk_06; u16 unk_08; u16 unk_0A; u8 unk_0C; u8 unk_0D; u8 unk_0E; u8 unk_0F; u8 unk_10; u8 unk_11; u8 unk_14_pad[0x2]; u32 unk_14; } unk_214[6];
    u16 unk_2A4[96];
    u16 unk_364[96];
    u16 unk_424[96];
    u16 unk_4E4[48];
    u16 unk_544[2][384];
    struct UnkStruct_02013534 *unk_B44;
    struct TextOBJ *unk_B48[4];
    struct GF3DVramMan *unk_B58;
    struct ParticleSystem *unk_B5C;
    void *unk_B60;
    u8 unk_B64[4];
    u8 unk_B68[4];
    u8 unk_B6C[4];
    u8 unk_B70[4];
    s32 unk_B74;
    s16 unk_B78;
    s16 unk_B7A;
    s8 unk_B7C;
    s8 unk_B7D;
    u8 unk_B7E;
    u8 unk_B7F;
    u8 unk_B80;
    u8 unk_B81;
    u8 unk_B82;
    u8 pad_B83[0x1];
    u16 unk_B84;
    u16 unk_B86;
    Window unk_B88;
    Window unk_B98;
    u32 unk_BA8;
    struct MsgData *unk_BAC;
    struct MessageFormat *unk_BB0;
    struct String *unk_BB4;
    int unk_BB8;
    int unk_BBC;
    struct ListMenu2D *unk_BC0;
    struct WaitingIcon *unk_BC4;
    int unk_BC8;
    int unk_BCC;
    BOOL unk_BD0;
};

// sizeof(UnkStruct_ov10_0221FB28_t) == 0xbd4

#endif // POKEHEARTGOLD_OVERLAY_05_UNK_STRUCT_OV10_0221_FB28_T_H
