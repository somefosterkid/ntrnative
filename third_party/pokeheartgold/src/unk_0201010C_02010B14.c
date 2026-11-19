#include "global.h"

#include "screen_fade_fx.h"

struct HardwareWindowSettings;
struct ScreenFade;
struct ScreenFadeHBlanks;
void SetMasterBrightness(enum DSScreen screen, int brightness);
int sub_02010A6C(int param0, int param1, int param2);
static inline void inline_0201035C_sub(int param0, int param1, int param2, int param3, int param4, enum DSScreen screen) {
    if (param4 == 0) {
        if (screen == DS_SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd0Position(param0, param1, param2, param3);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd0Position(param0, param1, param2, param3);
            }
        }
    } else {
        if (screen == DS_SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd1Position(param0, param1, param2, param3);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd1Position(param0, param1, param2, param3);
            }
        }
    }
}
void sub_02010F34(int param0, HardwareWindowSettings *param1, enum DSScreen screen);
void sub_02010F84(HardwareWindowSettings *param0, int param1, int param2, int param3, enum DSScreen screen, int param5, int param6, int param7, int param8, int param9);
void sub_02010710(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);
void sub_02010728(UnkStruct_0201076C *param0, enum DSScreen screen, int param2, int param3, int param4);
static inline void inline_020107D8_sub(int param0, BOOL param1, enum DSScreen screen) {
    if (screen == DS_SCREEN_MAIN) {
        if (GX_IsHBlank()) {
            G2_SetWndOutsidePlane(param0, param1);
        }
    } else {
        if (GX_IsHBlank()) {
            G2S_SetWndOutsidePlane(param0, param1);
        }
    }
}
static inline void inline_020107D8_sub_1(int param0, BOOL param1, int param4, enum DSScreen screen) {
    if (param4 == 0) {
        if (screen == DS_SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd0InsidePlane(param0, param1);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd0InsidePlane(param0, param1);
            }
        }
    } else {
        if (screen == DS_SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd1InsidePlane(param0, param1);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd1InsidePlane(param0, param1);
            }
        }
    }
}
void sub_02010E48(UnkStruct_02010EA4 *param0, const UnkStruct_0200F980 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5);
void sub_02010FC0(UnkStruct_0201109C *param0, const UnkStruct_0200FB7C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011298(UnkStruct_02011360 *param0, const UnkStruct_0200FC2C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011494(UnkStruct_02011568 *param0, const UnkStruct_0200FD34 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011738(UnkStruct_0201184C *param0, const UnkStruct_02011738 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011A00(UnkStruct_02011AFC *param0, UnkStruct_0200FEA4 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011D34(UnkStruct_02011E04 *param0, UnkStruct_0200FF30 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011FE8(UnkStruct_020120D4 *param0, UnkStruct_0200FE6C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02012290(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, int param7);
void sub_02012384(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void Heap_Free(void *ptr);
static BOOL sub_02010BF4(UnkStruct_02010318 *param0);

void sub_02010B14(ScreenFade *param0, int param1);
BOOL sub_02010BB4(ScreenFade *param0);
static BOOL sub_02010BF4(UnkStruct_02010318 *param0);

void sub_02010B14(ScreenFade *param0, int param1) {
    int v0, v1;
    UnkStruct_02010318 *v2;

    param0->data = Heap_Alloc(param0->heapID, sizeof(UnkStruct_02010318));
    memset(param0->data, 0, sizeof(UnkStruct_02010318));
    v2 = param0->data;

    if (param1 == 0) {
        if (param0->color == 0x7fff) {
            v0 = 16;
            v1 = 0;
        } else if (param0->color == 0x0) {
            v0 = -16;
            v1 = 0;
        } else {
            v0 = -16;
            v1 = 0;

            ((0) ? (void)0 : GF_AssertFail());
        }
    } else {
        if (param0->color == 0x7fff) {
            v0 = 0;
            v1 = 16;
        } else if (param0->color == 0x0) {
            v0 = 0;
            v1 = -16;
        } else {
            v0 = 0;
            v1 = -16;

            ((0) ? (void)0 : GF_AssertFail());
        }
    }

    SetMasterBrightness(param0->screen, v0);

    v2->unk_00 = param0->steps;
    v2->unk_04 = param0->framesPerStep;
    v2->unk_08 = 0;
    v2->unk_0C = v0 * 128;
    v2->unk_10 = v1 * 128;
    v2->unk_14 = sub_02010A6C(v0, v1, param0->steps);
    v2->screen = param0->screen;

    param0->state++;
}

BOOL sub_02010BB4(ScreenFade *param0) {
    UnkStruct_02010318 *v0 = param0->data;
    BOOL v1;
    BOOL v2 = 0;

    switch (param0->state) {
    case 1:
        v1 = sub_02010BF4(v0);

        if (v1 == 1) {
            param0->state++;
        }
        break;
    case 2:
        Heap_Free(param0->data);
        param0->data = 0L;
        param0->state++;
        v2 = 1;
        break;
    case 3:
        v2 = 1;
        break;
    default:
        break;
    }

    return v2;
}

static BOOL sub_02010BF4(UnkStruct_02010318 *param0) {
    BOOL v0 = 0;

    param0->unk_08++;

    if (param0->unk_08 >= param0->unk_04) {
        param0->unk_08 = 0;

        if ((param0->unk_00 - 1) > 0) {
            param0->unk_00--;

            param0->unk_0C += param0->unk_14;
        } else {
            param0->unk_0C = param0->unk_10;
            v0 = 1;
        }

        SetMasterBrightness(param0->screen, param0->unk_0C / 128);
    }

    return v0;
}
