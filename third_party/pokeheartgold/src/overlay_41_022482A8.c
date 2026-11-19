#include "global.h"

#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"

struct Pokemon;
struct Pokepic;
struct PokepicManager;
struct PokemonSpriteTransforms;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
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
void ov41_02249CC4(UnkStruct_ov22_022597BC *param0);
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;
typedef struct UnkStruct_ov66_02234798_t UnkStruct_ov66_02234798;
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0225D220;
typedef union {
    struct {
        s16 unk_00;
        s16 unk_02;
    } val1;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
    } val2;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
        fx32 unk_14;
    } val3;
    struct {
        s32 unk_00;
        VecFx32 unk_04;
    } val4;
} UnkStruct_ov70_0225CC54_sub1;
struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
};
void ov41_02248400(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);

void ov41_022482A8(UnkStruct_ov22_02257964 *param0);
UnkStruct_ov70_0225CC54 *ov41_022482B4(UnkStruct_ov70_0225CA20 *param0);
void ov41_022482B8(UnkStruct_ov22_02257964 *param0, int *param1, int *param2);

void ov41_022482A8(UnkStruct_ov22_02257964 *param0) {
    ov41_02249CC4(&param0->unk_2C.unk_1C);
}

UnkStruct_ov70_0225CC54 *ov41_022482B4(UnkStruct_ov70_0225CA20 *param0) {
    return param0->unk_24;
}

void ov41_022482B8(UnkStruct_ov22_02257964 *param0, int *param1, int *param2) {
    int v0;
    int v1;
    int v2;
    int v3;
    UnkStruct_ov22_02259560 *v4 = param0->unk_00.unk_14.unk_08;
    v0 = 0;
    v1 = 0;

    while (v4 != &param0->unk_00.unk_14) {
        ov41_02248400(v4, &v2, &v3);

        if ((((v2) < 0) ? (-(v2)) : (v2)) > (((v0) < 0) ? (-(v0)) : (v0))) {
            v0 = v2;
        }

        if ((((v3) < 0) ? (-(v3)) : (v3)) > (((v1) < 0) ? (-(v1)) : (v1))) {
            v1 = v3;
        }

        v4 = v4->unk_08;
    }

    *param1 = v0;
    *param2 = v1;
}
