#include "global.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "pokemon_types_def.h"
#include "pokepic.h"

typedef struct {
    u32 unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_02095B28;

typedef struct {
    UnkStruct_02095B28 unk_00;
    UnkStruct_02095B28 unk_08;
    u8 unk_10[4];
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
} UnkStruct_02095C60;

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov22_0225AD68;

typedef struct {
    BOOL unk_00;
    Sprite *unk_04[2];
    VecFx32 unk_0C[2];
    UnkStruct_ov22_0225AD68 unk_24;
    UnkStruct_ov22_0225AD68 unk_38;
    UnkStruct_ov22_0225AD68 unk_4C;
} UnkStruct_ov22_0225AE9C;

typedef struct {
    SpriteResource *unk_00[4];
    Sprite *unk_10[2];
    Window *unk_18;
    int unk_1C;
    int unk_20;
    SysTask *unk_24;
    SysTask *unk_28;
    UnkStruct_02095C60 *unk_2C;
    UnkStruct_ov22_0225AE9C unk_30;
    BOOL unk_90;
} UnkStruct_ov22_0225AB54;

void ov41_0224B31C(UnkStruct_ov22_0225AD68 *param0, fx32 param1, fx32 param2, int param3);

void ov41_0224B374(UnkStruct_ov22_0225AB54 *param0, UnkStruct_ov22_0225AE9C *param1);

struct Pokemon;
struct Pokepic;
struct PokepicManager;
struct PokemonSpriteTransforms;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
BOOL ov41_0224B32C(UnkStruct_ov22_0225AD68 *param0);
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
typedef struct {
    const UnkStruct_ov22_022597BC *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov22_02259820;
typedef struct {
    const UnkStruct_ov22_02257964 *unk_00;
    UnkStruct_ov22_02259820 unk_04;
} UnkStruct_ov22_0225AF68;
typedef struct {
    const UnkStruct_ov22_022597BC *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov22_0225AF44;
void ov41_02249CE0(UnkStruct_ov22_02259820 *param0, UnkStruct_ov22_0225AF44 *param1);
void ov41_02249CF8(UnkStruct_ov22_02259820 *param0, int param1);
void ov41_0224B450(UnkStruct_ov22_0225AE9C *param0);
void ov41_0224B4E8(UnkStruct_ov22_0225AF68 *param0, const UnkStruct_ov22_02257964 *param1, int param2);
void ov41_0224B50C(UnkStruct_ov22_0225AF68 *param0);

void ov41_02249D60(s8 *arg0, s32);
void ov41_0224B518(s8 *arg0, s32 arg1);

void ov41_0224B374(UnkStruct_ov22_0225AB54 *param0, UnkStruct_ov22_0225AE9C *param1) {
    int v0;
    fx32 v1;
    fx32 v2, v3;
    VecFx32 v4;
    int v5;

    v4.y = 58 * FX32_ONE;
    v4.y += (512 * FX32_ONE);

    for (v0 = 0; v0 < 2; v0++) {
        param1->unk_04[v0] = param0->unk_10[v0];
        Sprite_SetAffineOverwriteMode(param1->unk_04[v0], 2);
        v4.x = 103 + (v0 * 24);
        v4.x *= FX32_ONE;
        param1->unk_0C[v0] = v4;
    }

    v5 = 10 - param0->unk_1C;

    if (v5 > 0) {
        v1 = (FX32_CONST(1.0f)) + FX_Mul(v5 << FX32_SHIFT, (FX32_CONST(0.150f)));
    } else {
        v1 = (FX32_CONST(1.0f));
    }

    ov41_0224B31C(&param1->unk_24, v1, (FX32_CONST(1.0f)), 16);

    v2 = FX_Mul((FX32_CONST(24)), v1);
    v3 = FX_Mul((FX32_CONST(24)), v1);
    v2 = v2 - (FX32_CONST(24));
    v3 = v3 - (FX32_CONST(24));

    ov41_0224B31C(&param1->unk_38, v2, 0, 16);
    ov41_0224B31C(&param1->unk_4C, v3, 0, 16);

    param1->unk_00 = 1;
}

void ov41_0224B450(UnkStruct_ov22_0225AE9C *param0) {
    VecFx32 v0;
    VecFx32 v1;
    BOOL v2;

    if (param0->unk_00 == 0) {
        return;
    }

    v2 = ov41_0224B32C(&param0->unk_24);

    ov41_0224B32C(&param0->unk_38);
    ov41_0224B32C(&param0->unk_4C);

    v0.x = param0->unk_24.unk_00;
    v0.y = param0->unk_24.unk_00;
    v0.z = param0->unk_24.unk_00;

    Sprite_SetAffineScale(param0->unk_04[0], &v0);
    Sprite_SetAffineScale(param0->unk_04[1], &v0);

    v1 = param0->unk_0C[0];
    v1.x -= param0->unk_38.unk_00;
    v1.y -= param0->unk_4C.unk_00;

    Sprite_SetMatrix(param0->unk_04[0], &v1);

    v1 = param0->unk_0C[1];
    v1.y -= param0->unk_4C.unk_00;

    Sprite_SetMatrix(param0->unk_04[1], &v1);

    if (v2) {
        param0->unk_00 = 0;
    }
}

void ov41_0224B4E8(UnkStruct_ov22_0225AF68 *param0, const UnkStruct_ov22_02257964 *param1, int param2) {
    UnkStruct_ov22_0225AF44 v0;

    param0->unk_00 = param1;

    v0.unk_00 = &param1->unk_2C.unk_1C;
    v0.unk_04 = (param1->unk_2C.unk_48) * 4 + 134 + 3;
    v0.unk_08 = 16;
    v0.unk_0C = param2;

    ov41_02249CE0(&param0->unk_04, &v0);
}

void ov41_0224B50C(UnkStruct_ov22_0225AF68 *param0) {
    ov41_02249CF8(&param0->unk_04, 1);
}

void ov41_0224B518(s8 *arg0, s32 arg1) {
    u32 v0;
    s8 *v1;

    v1 = arg0;
    ov41_02249D60(v1 + 4, arg1);
    v0 = 0x1C;
    do {
        *v1 = 0;
        v1 += 1;
        v0 -= 1;
    } while (v0 != 0);
}
