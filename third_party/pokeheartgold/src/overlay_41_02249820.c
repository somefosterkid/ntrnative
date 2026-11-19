#include "global.h"

#include "touchscreen.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "pokepic.h"

struct Pokemon;

struct Pokepic;

struct PokepicDrawParam;

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

BOOL ov41_02249774(UnkStruct_020298D8 *param0, int param1, int param2);

void ov41_02249780(UnkStruct_020298D8 *param0, int *xCenter, int *yCenter);

void ov41_022497A0(UnkStruct_020298D8 *param0, int *param1, int *param2);

int ov41_022464BC(NNSG2dCharacterData *param0, int param1, int param2, int param3);

BOOL ov41_02249820(UnkStruct_020298D8 *param0, int param1, int param2, NNSG2dCharacterData *param3);

struct UnkStruct_ov22_02259560_t;
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
} UnkStruct_ov22_02259484;
void ov41_022499B4(UnkStruct_ov22_02259484 *param0, int param1, enum HeapID heapID);
void ov41_02249888(UnkStruct_020298D8 *param0, UnkStruct_ov22_022596B0 *param1);
int ov41_0224989C(char *param0, int param1);
void ov41_022498E8(char *param0, int param1, int param2, UnkStruct_ov22_022596B0 *param3);
void ov41_02249978(TouchscreenHitbox *rect, int param1, int param2, int param3, int param4);
void ov41_022499B4(UnkStruct_ov22_02259484 *param0, int param1, enum HeapID heapID);
void ov41_022499DC(UnkStruct_ov22_02259484 *param0);
UnkStruct_ov22_02259560 *ov41_022499F0(UnkStruct_ov22_02259484 *param0, void *param1, int param2);

BOOL ov41_02249820(UnkStruct_020298D8 *param0, int param1, int param2, NNSG2dCharacterData *param3) {
    int v0;
    int v1, v2;
    int v3, v4;

    v0 = ov41_02249774(param0, param1, param2);

    if (v0 == 0) {
        return 0;
    }

    ov41_02249780(param0, &v3, &v4);
    ov41_022497A0(param0, &v1, &v2);

    v3 -= (v1 / 2);
    v4 -= (v2 / 2);

    param1 -= v3;
    param2 -= v4;

    v0 = ov41_022464BC(param3, param1, param2, 0);

    if (v0 == 0) {
        return 1;
    }

    return 0;
}

void ov41_02249888(UnkStruct_020298D8 *param0, UnkStruct_ov22_022596B0 *param1) {
    *param1 = param0->unk_08;
}

int ov41_0224989C(char *param0, int param1) {
    int v0, v1;
    int v2;
    int v3;
    u8 v4;

    for (v0 = 0; v0 < 80; v0++) {
        for (v1 = 0; v1 < 80; v1++) {
            v2 = ((v1 * param1) + v0) / 2;
            v3 = ((v1 * param1) + v0) % 2;
            v4 = 0xf << (4 * v3);

            if ((param0[v2] & v4) != 0) {
                return v0;
            }
        }
    }

    return 80;
}

void ov41_022498E8(char *param0, int param1, int param2, UnkStruct_ov22_022596B0 *param3) {
    int v0, v1;
    int v2;
    int v3;
    u8 v4;

    param3->unk_00 = 80 / 2;
    param3->unk_01 = 80 / 2;
    param3->unk_02 = 80 / 2;
    param3->unk_03 = 80 / 2;

    for (v0 = 0; v0 < 80; v0++) {
        for (v1 = 0; v1 < 80; v1++) {
            v2 = ((v1 * param1) + v0) / 2;
            v3 = ((v1 * param1) + v0) % 2;
            v4 = 0xf << (4 * v3);

            if ((param0[v2] & v4) != 0) {
                if (param3->unk_00 > v0) {
                    param3->unk_00 = v0;
                }

                if (param3->unk_01 > 80 - v0) {
                    param3->unk_01 = 80 - v0;
                }

                if (param3->unk_02 > v1) {
                    param3->unk_02 = v1;
                }

                if (param3->unk_03 > 80 - v1) {
                    param3->unk_03 = 80 - v1;
                }
            }
        }
    }
}

void ov41_02249978(TouchscreenHitbox *rect, int param1, int param2, int param3, int param4) {
    if ((param2 - param4) >= 0) {
        rect->rect.top = param2 - param4;
    } else {
        rect->rect.top = 0;
    }

    if ((param2 + param4) <= 191) {
        rect->rect.bottom = param2 + param4;
    } else {
        rect->rect.bottom = 191;
    }

    if ((param1 - param3) >= 0) {
        rect->rect.left = param1 - param3;
    } else {
        rect->rect.left = 0;
    }

    if ((param1 + param3) <= 255) {
        rect->rect.right = param1 + param3;
    } else {
        rect->rect.right = 255;
    }
}

void ov41_022499B4(UnkStruct_ov22_02259484 *param0, int param1, enum HeapID heapID) {
    param0->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_ov22_02259560) * param1);
    ((param0->unk_00) ? (void)0 : GF_AssertFail());
    memset(param0->unk_00, 0, sizeof(UnkStruct_ov22_02259560) * param1);
    param0->unk_04 = param1;
}

void ov41_022499DC(UnkStruct_ov22_02259484 *param0) {
    Heap_Free(param0->unk_00);
    param0->unk_00 = 0L;
    param0->unk_04 = 0;
}

UnkStruct_ov22_02259560 *ov41_022499F0(UnkStruct_ov22_02259484 *param0, void *param1, int param2) {
    int v0;

    ((param0->unk_00) ? (void)0 : GF_AssertFail());
    ((param0->unk_04) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        if (param0->unk_00[v0].unk_00 == 0L) {
            break;
        }
    }

    ((param0->unk_04 > v0) ? (void)0 : GF_AssertFail());

    param0->unk_00[v0].unk_00 = param1;
    param0->unk_00[v0].unk_04 = param2;

    return param0->unk_00 + v0;
}
