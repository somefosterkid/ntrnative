#ifndef POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_0208_C06_C_H
#define POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_0208_C06_C_H

#include "touchscreen.h"

// UnkStruct_0208C06C: the ov40 application work (allocated 0x4170 bytes in sub_02087A8C). Layout regenerated from the m2c
// copy in src/unk_02087E70_02087E70.c (hk398); nested records are inlined as anonymous structs, pointers to types of
// other files stay `struct Tag *` / `void *`.

struct BgConfig;
struct GF3DVramMan;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct NARC;
struct PaletteData;
struct PokemonSpriteManager;
struct SaveData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct SysTask;
struct TextOBJ;
struct TouchHitboxController;
struct UnkStruct_02012744_t;
struct UnkStruct_02012B20_t;
struct UnkStruct_0202F41C_t;
struct UnkStruct_02030A80_t;
struct UnkStruct_0208B878;
struct UnkStruct_020F3DCC_t;
struct UnkStruct_ov61_0222BE20;
struct UnkStruct_ov61_0222BED8;
struct UnkStruct_ov62_02248CDC;

struct UnkStruct_0208BA84 {
    BOOL unk_00;
    int unk_04;
    u8 padding_08[8];
};

typedef struct UnkStruct_0208C06C {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int *unk_10;
    struct { struct NARC *unk_00; struct SpriteSystem *unk_04; struct SpriteManager *unk_08; u8 padding_0C[4]; struct BgConfig *unk_10; struct PaletteData *unk_14; struct TouchHitboxController *unk_18; TouchscreenHitbox unk_1C[5]; BOOL unk_30; struct MsgData *unk_34; struct MsgData *unk_38; struct UnkStruct_02012744_t *unk_3C; int unk_40; int unk_44; u8 unk_48; u8 padding_49[3]; struct GF3DVramMan *unk_4C; struct PokemonSpriteManager *unk_50; struct { struct SaveData *saveData; void *unk_04; void *unk_08; struct { s32 unk_00; u8 unk_04; u8 language; char unk_06[301]; u8 padding[5]; } unk_0C; u32 heapID; int unk_148; int unk_14C; struct { void *unk_00; void *unk_04; void *unk_08; void *unk_0C; void *unk_10; void *unk_14; void *unk_18; void *unk_1C; void *unk_20; void *unk_24; void *unk_28; } unk_150; struct { BOOL unk_00; int unk_04; int unk_08; int unk_0C; } unk_17C; void *unk_18C; struct { u8 unk_0_anon[0x21C]; union { struct { u8 unk_00; u8 unk_01; u8 unk_02[2]; } val1; struct { u16 unk_00; u16 unk_02; } val2; struct { u64 unk_00; } val3; } unk_21C; } unk_190; void *unk_3B4; int unk_3B8; int unk_3BC; int unk_3C0; BOOL unk_3C4; BOOL unk_3C8; struct { int unk_00; int unk_04; int unk_08; } unk_3CC; int unk_3D8; int unk_3DC; int unk_3E0; int unk_3E4; int unk_3E8; int unk_3EC; int unk_3F0; struct MsgData *unk_3F4; struct MessageFormat *unk_3F8; struct String *unk_3FC; struct { void *unk_00; u16 unk_04; u16 unk_06; u8 unk_08; u8 padding_9[3]; void *unk_0C; } unk_400; u8 unk_40D; u8 unk_40E; u8 padding_412[2]; } unk_54; struct { int unk_00; BOOL unk_04; u32 unk_08; u32 unk_0C; s16 unk_10; u8 padding_12[2]; BOOL unk_14; struct ManagedSprite * unk_18[2]; } unk_46C; struct { int unk_00; int unk_04; s16 unk_08; u8 padding_0A[2]; int unk_0C; int unk_10; int unk_14; struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } unk_18; struct UnkStruct_ov62_02248CDC *unk_28; struct ManagedSprite * unk_2C[2]; struct MsgData *unk_34; struct { int unk_00; int unk_04; int unk_08; int unk_0C; } unk_38; BOOL unk_48; u8 padding_4C[4]; } unk_48C; u8 padding_4D8[24]; struct { int unk_00; int unk_04; int unk_08; int unk_0C; int unk_10; int unk_14; BOOL unk_18; u8 padding_1C[4]; } unk_4F0; BOOL unk_510; BOOL unk_514; } unk_14;
    struct { int unk_00; BOOL unk_04; } unk_52C;
    struct { struct { struct ManagedSprite *unk_00; u8 padding_04[8]; BOOL unk_0C; TouchscreenHitbox *unk_10; struct TextOBJ *unk_14; struct UnkStruct_02012B20_t *unk_18; struct { u32 size; u32 offset; u16 vram; u16 isAtEnd; } unk_1C; } unk_00[5]; struct { struct ManagedSprite *unk_00; u8 padding_04[8]; BOOL unk_0C; TouchscreenHitbox *unk_10; struct TextOBJ *unk_14; struct UnkStruct_02012B20_t *unk_18; struct { u32 size; u32 offset; u16 vram; u16 isAtEnd; } unk_1C; } unk_C8[5]; struct UnkStruct_020F3DCC_t * unk_190[5]; int unk_1A4; u8 padding_1A8[4]; int unk_1AC; int unk_1B0; u8 padding_1B4[8]; } unk_534;
    struct UnkStruct_0208B878 *unk_6F0;
    struct UnkStruct_0208B878 *unk_6F4;
    struct { struct ManagedSprite * unk_00[2]; struct SysTask *unk_08; fx32 unk_0C; f32 unk_10; s16 unk_14; s16 unk_16; s8 unk_18; u8 unk_19; u8 padding_1A[2]; int unk_1C; int unk_20; int unk_24; int unk_28; int unk_2C; } unk_6F8[6];
    struct UnkStruct_020F3DCC_t *unk_818;
    struct UnkStruct_020F3DCC_t * unk_81C[5];
    struct SaveData *saveData;
    u16 unk_834;
    u16 unk_836;
    struct { BOOL unk_00; int unk_04; int unk_08[8]; } unk_838;
    void *unk_860;
    u8 padding_864[4];
    struct UnkStruct_0208BA84 *unk_868;
    int unk_86C;
    BOOL unk_870;
    u32 unk_874;
    u8 *unk_878;
    struct UnkStruct_0202F41C_t * unk_87C[4];
    struct UnkStruct_02030A80_t * unk_88C[4];
    BOOL unk_89C;
    BOOL unk_8A0;
    struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } unk_8A4;
    struct { struct UnkStruct_ov61_0222BED8 *unk_00; struct { struct { u16 unk_00[8]; u32 unk_10; u8 unk_14; u8 unk_15; u8 unk_16; u8 unk_17; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B_0 : 1; u8 unk_1B_1 : 7; u16 unk_1C; u8 unk_1E; u8 unk_1F; u8 unk_20_anon[0x50]; u8 unk_70[12]; struct { u16 unk_00; u16 unk_02; } unk_7C; } unk_00; struct { u16 unk_00[12]; u8 unk_18[12]; u16 unk_24; u8 unk_26; u8 unk_27; struct { u16 unk_00[12]; u16 unk_18; u8 unk_1A; u8 unk_1B; s8 unk_1C; s8 unk_1D; u8 unk_1E_0 : 1; u8 unk_1E_1 : 1; u8 unk_1E_2 : 1; u8 unk_1E_3 : 1; u8 unk_1E_4 : 1; u8 : 3; u8 unk_1F; } unk_28; u16 unk_48; u8 unk_4A[14]; u64 unk_58; struct { u16 unk_00; u16 unk_02; } unk_60; } unk_80; struct { struct { u32 unk_00; int unk_04; int unk_08[4]; struct { u8 unk_00; u8 unk_01; u8 unk_02; u8 unk_03; u16 unk_04[4]; u32 unk_0C; u32 unk_10; u16 unk_14[8]; struct { u16 msg_bank; u16 msg_no; u16 fields[2]; } unk_24; struct { u16 msg_bank; u16 msg_no; u16 fields[2]; } unk_2C; } unk_18[4]; int unk_E8; int unk_EC; int unk_F0; int unk_F4; int unk_F8; int unk_FC; int unk_100; int unk_104; int unk_108; int unk_10C; int unk_110; u32 unk_114[4]; u32 unk_124; int unk_128; u32 unk_12C; u32 unk_130; int unk_134[4]; u16 unk_144; u16 unk_146; int unk_148; u8 unk_14C[4]; } unk_00; struct { u8 unk_00[4][1024]; } unk_150; struct { u16 unk_00; u16 unk_02; struct { u32 unk_00; u16 unk_04_0 : 1; u16 unk_04_1 : 1; u16 unk_04_2 : 1; u16 unk_04_3 : 13; u16 unk_06; u16 unk_08; u8 padding_A[2]; u32 unk_0C; u32 unk_10; u8 unk_14; u8 unk_15; u8 unk_16; u8 unk_17; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B; u16 unk_1C[4]; u8 unk_24[4]; u8 unk_28[4]; u32 unk_2C_0 : 5; u32 unk_2C_5 : 5; u32 unk_2C_10 : 5; u32 unk_2C_15 : 5; u32 unk_2C_20 : 5; u32 unk_2C_25 : 5; u32 unk_2C_30 : 1; u32 unk_2C_31 : 1; u8 unk_30_0 : 1; u8 unk_30_1 : 2; u8 unk_30_3 : 5; u8 padding_31[1]; u16 unk_32[11]; u16 unk_48[8]; u8 unk_58; u8 unk_59; u8 padding_5A[2]; u32 unk_5C; u8 unk_60; u8 unk_61; u16 unk_62; u16 unk_64; u16 unk_66; u16 unk_68; u16 unk_6A; u16 unk_6C; u16 unk_6E; } unk_04[6]; } unk_1150[4]; struct { u16 unk_00[8]; u32 unk_10; u32 unk_14; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B; u8 unk_1C; u8 unk_1D_0 : 1; u8 unk_1D_1 : 1; u8 unk_1D_2 : 6; u8 unk_1E; u8 unk_1F; } unk_1B68[4]; struct { u16 unk_00_0 : 4; u16 unk_00_4 : 2; u16 unk_00_6 : 1; u16 unk_00_7 : 1; u16 unk_00_8 : 2; u16 unk_00_10 : 5; u16 unk_00_15 : 1; } unk_1BE8; u16 unk_1BEA; struct { u16 unk_00; u16 unk_02; } unk_1BEC; } unk_E4; } unk_04; u8 unk_1D54; u8 padding_1D55[3]; struct UnkStruct_ov61_0222BE20 * unk_1D58[30]; struct { struct { u16 unk_00[8]; u32 unk_10; u8 unk_14; u8 unk_15; u8 unk_16; u8 unk_17; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B_0 : 1; u8 unk_1B_1 : 7; u16 unk_1C; u8 unk_1E; u8 unk_1F; u8 unk_20_anon[0x50]; u8 unk_70[12]; struct { u16 unk_00; u16 unk_02; } unk_7C; } unk_00; struct { u16 unk_00[12]; u8 unk_18[12]; u16 unk_24; u8 unk_26; u8 unk_27; struct { u16 unk_00[12]; u16 unk_18; u8 unk_1A; u8 unk_1B; s8 unk_1C; s8 unk_1D; u8 unk_1E_0 : 1; u8 unk_1E_1 : 1; u8 unk_1E_2 : 1; u8 unk_1E_3 : 1; u8 unk_1E_4 : 1; u8 : 3; u8 unk_1F; } unk_28; u16 unk_48; u8 unk_4A[14]; u64 unk_58; struct { u16 unk_00; u16 unk_02; } unk_60; } unk_80; } unk_1DD0[30]; int unk_3900; u8 unk_3904[32]; } unk_8B4;
    struct { BOOL unk_00; int unk_04; struct UnkStruct_0208B878 *unk_08; struct SysTask *unk_0C; } unk_41D8;
    struct SysTask *unk_41E8;
} UnkStruct_0208C06C;

// sizeof(UnkStruct_0208C06C) == 0x4170 on DS

#endif // POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_0208_C06_C_H
