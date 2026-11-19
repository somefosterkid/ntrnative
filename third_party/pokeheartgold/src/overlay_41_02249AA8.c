#include "global.h"

#include "touchscreen.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "pokepic.h"

struct Pokemon;

struct Pokepic;

struct PokepicDrawParam;

struct SoftwareSprite;


typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;

BOOL ov41_02245F14(UnkStruct_ov22_02255040 *param0, int *param1, int *param2, NNSG2dCharacterData *param3);

BOOL ov41_022497A8(UnkStruct_020298D8 *param0, int *param1, int *param2, NNSG2dCharacterData *param3);

BOOL ov41_02249AA8(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, NNSG2dCharacterData **param3);

void ov41_02245FA8(UnkStruct_ov22_02255040 *param0, int *param1, int *param2);
void ov41_02249780(UnkStruct_020298D8 *param0, int *xCenter, int *yCenter);
void ov41_02249B44(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);

BOOL ov41_02249AA8(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, NNSG2dCharacterData **param3) {
    NNSG2dCharacterData *v0;

    if (param0->unk_04 == 0) {
        UnkStruct_ov22_02255040 *v1 = param0->unk_00;

        v0 = param3[v1->unk_00];
        return ov41_02245F14(v1, param1, param2, v0);
    }

    if (param0->unk_04 == 1) {
        UnkStruct_ov22_02255040 *v2 = param0->unk_00;
        int v3 = v2->unk_00 + 100;

        v0 = param3[v3];
        return ov41_02245F14(v2, param1, param2, v0);
    }

    if (param0->unk_04 == 2) {
        UnkStruct_ov22_02255040 *v4 = param0->unk_00;
        int v5 = v4->unk_00 + 100;

        v0 = param3[v5];
        return ov41_02245F14(v4, param1, param2, v0);
    }

    {
        UnkStruct_020298D8 *v6 = param0->unk_00;

        v0 = param3[(100 + 18)];
        return ov41_022497A8(v6, param1, param2, v0);
    }
}

void ov41_02245F9C(UnkStruct_ov22_02255040 *param0, s16 param1, s16 param2);

void ov41_0224971C(UnkStruct_020298D8 *param0, int param1, int param2);

void ov41_022497A0(UnkStruct_020298D8 *param0, int *param1, int *param2);

void ov41_02249AF4(UnkStruct_ov22_02259560 *param0, int param1, int param2);

void ov41_02249AF4(UnkStruct_ov22_02259560 *param0, int param1, int param2) {
    if (param0->unk_04 < 3) {
        UnkStruct_ov22_02255040 *v0 = param0->unk_00;

        ov41_02245F9C(v0, param1, param2);
    } else {
        UnkStruct_020298D8 *v1 = param0->unk_00;
        int v2, v3;

        ov41_022497A0(v1, &v2, &v3);
        v2 /= 2;
        v3 /= 2;
        param1 += v2;
        param2 += v3;
        ov41_0224971C(v1, param1, param2);
    }
}

void ov41_02249B44(UnkStruct_ov22_02259560 *param0, int *param1, int *param2) {
    if (param0->unk_04 < 3) {
        UnkStruct_ov22_02255040 *v0 = param0->unk_00;
        ov41_02245FA8(v0, param1, param2);
    } else {
        UnkStruct_020298D8 *v1 = param0->unk_00;
        int v2, v3;

        ov41_02249780(v1, param1, param2);
        ov41_022497A0(v1, &v2, &v3);

        v2 /= 2;
        v3 /= 2;
        *param1 -= v2;
        *param2 -= v3;
    }
}
