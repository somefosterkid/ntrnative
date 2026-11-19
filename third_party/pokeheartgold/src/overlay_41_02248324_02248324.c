#include "global.h"

#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "pokemon_types_def.h"
#include "pokepic.h"

struct Pokemon;

struct Pokepic;

struct PokepicManager;

struct PokemonSpriteTransforms;

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
} UnkStruct_ov22_02259484;

typedef struct {
    UnkStruct_ov22_02259484 *unk_00;
    UnkStruct_ov22_02259560 unk_04;
    UnkStruct_ov22_02259560 unk_14;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_02257964_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;

typedef struct {
    UnkStruct_ov22_02255040 *unk_00;
    int unk_04;
} UnkStruct_ov22_02254DE0;

typedef struct {
    BgConfig *unk_00;
    enum NarcId narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_022597BC;

typedef struct {
    SoftwareSpriteManager *unk_00;
    SoftwareSpriteChars **unk_04;
    SoftwareSpritePalette **unk_08;
    u8 *unk_0C;
    PokepicManager *unk_10;
    BgConfig *unk_14;
    UnkStruct_ov22_02254DE0 *unk_18;
    UnkStruct_ov22_022597BC unk_1C;
    int unk_48;
    UnkStruct_020298D8 unk_4C;
} UnkStruct_ov22_02257964_sub2;

typedef struct {
    UnkStruct_ov22_02257964_sub1 unk_00;
    UnkStruct_ov22_02257964_sub2 unk_2C;
} UnkStruct_ov22_02257964;

void ov41_02247E34(u32 param0, u32 param1, u32 param2, u32 param3, u32 param4, int *param5, int *param6);

BOOL ov41_02249820(UnkStruct_020298D8 *param0, int param1, int param2, NNSG2dCharacterData *param3);

void ov41_02249A50(UnkStruct_ov22_02259560 *param0, UnkStruct_ov22_02259560 *param1);

void ov41_02249B44(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);

void ov41_02249B94(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);

void ov41_02249BAC(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4);

void ov41_02248324(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_02259560 *param1, BOOL param2, NNSG2dCharacterData **param3);

void ov41_02248324(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_02259560 *param1, BOOL param2, NNSG2dCharacterData **param3) {
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
    int v10;
    int v11, v12;
    BOOL v13;
    NNSG2dCharacterData *v14;
    int v15, v16;

    ov41_02249B44(param1, &v1, &v2);
    ov41_02249B94(param1, &v3, &v4);
    ov41_02249BAC(param1, &v5, &v15, &v6, &v16);

    v7 = v2 + v6;
    v8 = v2 + v4 - v16;
    v9 = v1 + v5;
    v10 = v1 + v3 - v15;
    v14 = param3[(100 + 18)];

    v13 = 0;

    for (v0 = 0; v0 < 16; v0++) {
        ov41_02247E34(v7, v8, v9, v10, v0, &v11, &v12);
        v13 |= ov41_02249820(&param0->unk_2C.unk_4C, v11, v12, v14);
    }

    if (v13) {
        if (param2) {
            ov41_02249A50(param1, &param0->unk_00.unk_14);
        } else {
            ov41_02249A50(param1, param0->unk_00.unk_14.unk_0C);
        }
    } else {
        if (param2) {
            ov41_02249A50(param1, &param0->unk_00.unk_04);
        } else {
            ov41_02249A50(param1, param0->unk_00.unk_04.unk_0C);
        }
    }
}
