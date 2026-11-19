#include "global.h"

#include "screen_fade_fx.h"

void sub_02012A8C(void);

struct HardwareWindowSettings;
struct ScreenFade;
struct ScreenFadeHBlanks;
void sub_02012ACC(const UnkStruct_0200F898 *param0, UnkStruct_02012174 *param1, int param2, int param3);
void sub_02010F34(int param0, HardwareWindowSettings *param1, enum DSScreen screen);
void sub_02010F84(HardwareWindowSettings *param0, int param1, int param2, int param3, enum DSScreen screen, int param5, int param6, int param7, int param8, int param9);
void sub_02010710(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);
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
void sub_02012BE8(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, int param7);
void sub_02012CDC(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02010728(UnkStruct_0201076C *param0, enum DSScreen screen, int param2, int param3, int param4);
void Heap_Free(void *ptr);
BOOL sub_02012C68(UnkStruct_02012290 *param0, ScreenFade *param1);
BOOL sub_02012D4C(UnkStruct_02012290 *param0, ScreenFade *param1);
void sub_020117A0(UnkStruct_02010EA4 *param0, const UnkStruct_0200F980 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5);
void sub_02011068(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);
BOOL sub_020117FC(UnkStruct_02010EA4 *param0);
void sub_02012940(UnkStruct_020120D4 *param0, UnkStruct_0200FE6C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
BOOL sub_02012A2C(UnkStruct_020120D4 *param0);
void sub_02012A90(UnkStruct_020120D4 *param0);
void sub_02012ACC(const UnkStruct_0200F898 *param0, UnkStruct_02012174 *param1, int param2, int param3);
void sub_02012B1C(ScreenFade *param0, UnkStruct_0201006C *param1);
BOOL sub_02012B80(ScreenFade *param0);
void sub_02012BE8(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, int param7);
BOOL sub_02012C68(UnkStruct_02012290 *param0, ScreenFade *param1);
void sub_02012CDC(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
BOOL sub_02012D4C(UnkStruct_02012290 *param0, ScreenFade *param1);

void sub_02012A8C(void) {
}

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

void sub_02012A90(UnkStruct_020120D4 *param0) {
    int v0;
    UnkStruct_02012174 *v1;
    const UnkStruct_0200F898 *v2;

    v1 = &param0->unk_00.unk_00[0];

    for (v0 = 0; v0 < param0->unk_310; v0++) {
        v2 = &param0->unk_30C[v0];
        sub_02012ACC(v2, v1, param0->unk_318, param0->unk_314);
    }
}

void sub_02012ACC(const UnkStruct_0200F898 *param0, UnkStruct_02012174 *param1, int param2, int param3) {
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;

    v1 = (param0->unk_01 - param0->unk_00) * param2;
    v2 = v1 / param3;

    v2 += param0->unk_00;

    if (param0->unk_00 <= param0->unk_01) {
        v3 = param0->unk_00;
        v4 = param0->unk_01;
        v5 = param0->unk_02;
    } else {
        v3 = param0->unk_01;
        v4 = param0->unk_00;

        if (param0->unk_02 == 0) {
            v5 = 1;
        } else {
            v5 = 0;
        }
    }

    for (v0 = v3; v0 < v4; v0++) {
        if (v0 == v2) {
            if (v5 == 0) {
                v5 = 1;
            } else {
                v5 = 0;
            }
        }

        param1->unk_00[v0] = v5;
    }
}

void sub_02012B1C(ScreenFade *param0, UnkStruct_0201006C *param1) {
    UnkStruct_02012290 *v0;

    param0->data = Heap_Alloc(param0->heapID, sizeof(UnkStruct_02012290));
    memset(param0->data, 0, sizeof(UnkStruct_02012290));

    v0 = param0->data;

    if (param1->unk_00.unk_0B == 0) {
        sub_02012BE8(v0, param1, param0->steps, param0->framesPerStep, param0->screen, param0->hwSettings, param0->hblanks, param0->heapID);
    } else {
        sub_02012CDC(v0, param1, param0->steps, param0->framesPerStep, param0->screen, param0->hwSettings, param0->hblanks, param0->heapID);
    }

    param0->state++;
}

BOOL sub_02012B80(ScreenFade *param0) {
    UnkStruct_02012290 *v0;
    BOOL v1;
    BOOL v2 = 0;

    v0 = (UnkStruct_02012290 *)param0->data;

    switch (param0->state) {
    case 1:
        if (v0->unk_386 == 0) {
            v1 = sub_02012C68(v0, param0);
        } else {
            v1 = sub_02012D4C(v0, param0);
        }

        if (v1 == 1) {
            sub_02010F34(param0->direction, param0->hwSettings, param0->screen);
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
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return v2;
}

void sub_02012BE8(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, int param7) {
    int v0 = FX_MulInline(param2 * ((fx32)0x0000000000001000L), param1->unk_14) >> 12;

    param0->unk_384 = param2 - v0;
    param0->unk_380 = param1;
    param0->unk_386 = param1->unk_00.unk_0B;

    sub_020117A0(&param0->unk_00, &param1->unk_00, v0, param3, screen, param5);

    if (param1->unk_00.unk_08 == 0) {
        sub_02011068(param5, GX_WNDMASK_W0, screen, param1->unk_00.unk_0B);
    } else {
        sub_02011068(param5, GX_WNDMASK_W1, screen, param1->unk_00.unk_0B);
    }

    param0->unk_385 = 0;
}

BOOL sub_02012C68(UnkStruct_02012290 *param0, ScreenFade *param1) {
    BOOL v0;
    BOOL v1 = 0;

    switch (param0->unk_385) {
    case 0:
        v0 = sub_020117FC(&param0->unk_00);

        if (v0 == 1) {
            param0->unk_385++;

            sub_02012940(&param0->unk_4C, &param0->unk_380->unk_0C, param0->unk_384, param1->framesPerStep, param1->screen, param1->hwSettings, param1->hblanks, param1->heapID);
        }
        break;
    case 1:
        v0 = sub_02012A2C(&param0->unk_4C);

        if (v0 == 1) {
            v1 = 1;
            param0->unk_385++;
        }
        break;
    case 2:
        v1 = 1;
        break;
    }

    return v1;
}

void sub_02012CDC(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID) {
    int v0;

    param0->unk_384 = FX_MulInline(param2 * ((fx32)0x0000000000001000L), param1->unk_14) >> 12;

    v0 = param2 - param0->unk_384;

    param0->unk_380 = param1;
    param0->unk_386 = param1->unk_00.unk_0B;

    sub_02012940(&param0->unk_4C, &param0->unk_380->unk_0C, v0, param3, screen, param5, param6, heapID);

    param0->unk_385 = 0;
}

BOOL sub_02012D4C(UnkStruct_02012290 *param0, ScreenFade *param1) {
    BOOL v0;
    BOOL v1 = 0;

    switch (param0->unk_385) {
    case 0:
        v0 = sub_02012A2C(&param0->unk_4C);

        if (v0 == 1) {
            param0->unk_385++;
            sub_020117A0(&param0->unk_00, &param0->unk_380->unk_00, param0->unk_384, param1->framesPerStep, param1->screen, param1->hwSettings);

            if (param0->unk_380->unk_00.unk_08 == 0) {
                sub_02011068(param1->hwSettings, GX_WNDMASK_W0, param1->screen, param0->unk_380->unk_00.unk_0B);
            } else {
                sub_02011068(param1->hwSettings, GX_WNDMASK_W1, param1->screen, param0->unk_380->unk_00.unk_0B);
            }
        }
        break;
    case 1:
        v0 = sub_020117FC(&param0->unk_00);

        if (v0 == 1) {
            v1 = 1;
            param0->unk_385++;
        }
        break;
    case 2:
        v1 = 1;
        break;
    }

    return v1;
}
