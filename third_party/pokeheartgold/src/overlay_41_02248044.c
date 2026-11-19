#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "overlay_41_unk_struct_ov22_0225500c.h"

typedef struct UnkStruct_ov41_022480E0 {
    u8 unk_0[0x4];
    u8 unk_4;
    u8 unk_5[0xF];
    u8 unk_14;
    u8 unk_15[0xF];
    u32 unk_24;
} UnkStruct_ov41_022480E0;

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

UnkStruct_ov22_02255040 *ov41_02245EE0(UnkStruct_ov22_0225500C *param0);

void ov41_02246014(UnkStruct_ov22_02255040 *param0, int param1);

UnkStruct_ov22_02259560 *ov41_022499F0(UnkStruct_ov22_02259484 *param0, void *param1, int param2);

void ov41_02249A50(UnkStruct_ov22_02259560 *param0, UnkStruct_ov22_02259560 *param1);

BOOL ov41_02248044(UnkStruct_ov22_02257964 *param0, int param1, int param2, int param3, int param4);

void ov41_02248324(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_02259560 *param1, BOOL param2, NNSG2dCharacterData **param3);
BOOL ov41_022480A4(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_02259560 *param1, NNSG2dCharacterData **param2);

void ov41_02249A60(UnkStruct_ov22_02259560 *);
void ov41_022480C8(void *arg0, void *arg1);

void ov41_02249A70(void *);
void ov41_022480E0(UnkStruct_ov41_022480E0 *param_1);

BOOL ov41_02248044(UnkStruct_ov22_02257964 *param0, int param1, int param2, int param3, int param4) {
    UnkStruct_ov22_0225500C v0;
    UnkStruct_ov22_02255040 *v1;

    if (param0->unk_00.unk_24 < param0->unk_00.unk_28) {
        v0.unk_00 = param0->unk_2C.unk_18;
        v0.unk_04 = param0->unk_2C.unk_00;
        v0.unk_08 = param0->unk_2C.unk_04[param1];
        v0.unk_0C = param0->unk_2C.unk_08[0];
        v0.unk_10 = param2;
        v0.unk_14 = param3;
        v0.unk_18 = param1;
        v0.unk_1C = param0->unk_2C.unk_0C[param1];

        v1 = ov41_02245EE0(&v0);
        ov41_02246014(v1, param4);

        {
            UnkStruct_ov22_02259560 *v2;

            v2 = ov41_022499F0(param0->unk_00.unk_00, v1, 0);
            ov41_02249A50(v2, &param0->unk_00.unk_04);
        }

        param0->unk_00.unk_24++;

        return 1;
    }

    return 0;
}

BOOL ov41_022480A4(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_02259560 *param1, NNSG2dCharacterData **param2) {
    if (param0->unk_00.unk_24 < param0->unk_00.unk_28) {
        ov41_02248324(param0, param1, 1, param2);
        param0->unk_00.unk_24++;

        return 1;
    }

    return 0;
}

void ov41_022480C8(void *arg0, void *arg1) {
    s32 v0;

    ov41_02249A60(arg1);
    v0 = *((s32 *)&((UnkStruct_ov22_02257964 *)arg0)->unk_00.unk_24) - 1;
    *((s32 *)&((UnkStruct_ov22_02257964 *)arg0)->unk_00.unk_24) = v0;
    if (v0 < 0) {
        GF_AssertFail();
    }
}

void ov41_022480E0(UnkStruct_ov41_022480E0 *param_1) {
    ov41_02249A70((u8 *)&param_1->unk_4);
    ov41_02249A70((u8 *)&param_1->unk_14);
    param_1->unk_24 = 0;
    return;
}
