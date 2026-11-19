#include "global.h"

#include "screen_fade_fx.h"

struct HardwareWindowSettings;
struct ScreenFade;
struct ScreenFadeHBlanks;
void sub_020117A0(UnkStruct_02010EA4 *param0, const UnkStruct_0200F980 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5);

void sub_02010AB0(void *arg0, void *arg1, void *arg2, void *arg3, void *arg4, s32 arg5);

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
void sub_02011068(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);
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
BOOL sub_020117FC(UnkStruct_02010EA4 *param0);
void sub_02010710(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);
void sub_02010E48(UnkStruct_02010EA4 *param0, const UnkStruct_0200F980 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5);

void sub_0201164C(ScreenFade *param0, const UnkStruct_0200F980 *param1);
BOOL sub_0201169C(ScreenFade *param0);
void sub_020116EC(ScreenFade *param0, const UnkStruct_0200F980 *param1, const UnkStruct_0200F980 *param2);
BOOL sub_02011744(ScreenFade *param0);

void sub_0201164C(ScreenFade *param0, const UnkStruct_0200F980 *param1) {
    UnkStruct_02010EA4 *v0;

    param0->data = Heap_Alloc(param0->heapID, sizeof(UnkStruct_02010EA4));

    v0 = (UnkStruct_02010EA4 *)param0->data;

    sub_020117A0(v0, param1, param0->steps, param0->framesPerStep, param0->screen, param0->hwSettings);

    if (param1->unk_08 == 0) {
        sub_02011068(param0->hwSettings, GX_WNDMASK_W0, v0->screen, v0->unk_44);
    } else {
        sub_02011068(param0->hwSettings, GX_WNDMASK_W1, v0->screen, v0->unk_44);
    }

    param0->state++;
}

BOOL sub_0201169C(ScreenFade *param0) {
    UnkStruct_02010EA4 *v0;
    BOOL v1;
    BOOL v2 = 0;

    v0 = (UnkStruct_02010EA4 *)param0->data;

    switch (param0->state) {
    case 1:
        v1 = sub_020117FC(v0);

        if (v1 == 1) {
            sub_02010F34(v0->unk_44, param0->hwSettings, param0->screen);
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

void sub_020116EC(ScreenFade *param0, const UnkStruct_0200F980 *param1, const UnkStruct_0200F980 *param2) {
    UnkStruct_02010D94 *v0;

    param0->data = Heap_Alloc(param0->heapID, sizeof(UnkStruct_02010D94));
    v0 = (UnkStruct_02010D94 *)param0->data;

    sub_020117A0(&v0->unk_00, param1, param0->steps, param0->framesPerStep, param0->screen, param0->hwSettings);
    sub_020117A0(&v0->unk_4C, param2, param0->steps, param0->framesPerStep, param0->screen, param0->hwSettings);
    sub_02011068(param0->hwSettings, GX_WNDMASK_W0 | GX_WNDMASK_W1, param0->screen, v0->unk_00.unk_44);

    param0->state++;
}

BOOL sub_02011744(ScreenFade *param0) {
    UnkStruct_02010D94 *v0;
    BOOL v1;
    BOOL v2 = 0;

    v0 = (UnkStruct_02010D94 *)param0->data;

    switch (param0->state) {
    case 1:
        v1 = sub_020117FC(&v0->unk_00);
        v1 += sub_020117FC(&v0->unk_4C);

        if (v1 == 2) {
            sub_02010F34(v0->unk_00.unk_44, param0->hwSettings, param0->screen);

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
    }

    return v2;
}

void sub_020117A0(UnkStruct_02010EA4 *param0, const UnkStruct_0200F980 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5) {
    sub_02010AB0(&param0->unk_00, &param0->unk_20, &param0->unk_10, &param1->unk_00, &param1->unk_04, param2);

    param0->screen = screen;
    param0->unk_34 = param1->unk_08;
    param0->unk_38 = param2;
    param0->unk_3C = param3;
    param0->unk_40 = 0;
    param0->unk_48 = param5;
    param0->unk_44 = param1->unk_0B;

    sub_02010F84(param5, param1->unk_09, param1->unk_0A, param1->unk_08, screen, param1->unk_00.unk_00, param1->unk_00.unk_01, param1->unk_00.unk_02, param1->unk_00.unk_03, param0->unk_44);
}
