#include "global.h"

#include "screen_fade_fx.h"

void sub_020124AC(void);

struct HardwareWindowSettings;
struct ScreenFade;
struct ScreenFadeHBlanks;
int sub_020109D8(int param0, int param1);
UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *param0, int param1);
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
void sub_0201268C(UnkStruct_02011AFC *param0, UnkStruct_0200FEA4 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02010728(UnkStruct_0201076C *param0, enum DSScreen screen, int param2, int param3, int param4);
void Heap_Free(void *ptr);
void sub_02010EC8(UnkStruct_02010588 *param0);
BOOL sub_0201275C(UnkStruct_02011AFC *param0);
void sub_020127B4(UnkStruct_02011AFC *param0);
void sub_020124B0(UnkStruct_02011AFC *param0);
void sub_020125D4(UnkStruct_02011C7C *param0, int param1, int param2);
void sub_020125EC(ScreenFade *param0, UnkStruct_0200FEA4 *param1);
BOOL sub_0201262C(ScreenFade *param0);

void sub_020124AC(void) {
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

void sub_020124B0(UnkStruct_02011AFC *param0) {
    UnkStruct_02010FC0 *v0;
    UnkStruct_02010FC0 *v1;
    u16 v2;
    int v3, v4;
    int v5;

    v2 = param0->unk_0C.unk_00 % ((90 * 0xffff) / 360);
    v0 = sub_02010EE0(&param0->unk_00, 0);
    v1 = sub_02010EE0(&param0->unk_00, 1);

    for (v5 = 0; v5 < 96; v5++) {
        if (param0->unk_0C.unk_00 < ((90 * 0xffff) / 360)) {
            v3 = 128;
            v4 = sub_020109D8(v2, (96 - v5));

            if (v4 > 127) {
                v4 = 127;
            }

            v0->unk_300[0][191 - v5] = v3 - v4;
            v0->unk_300[1][191 - v5] = v3;

            v1->unk_300[0][v5] = v3;
            v1->unk_300[1][v5] = v3 + v4;
        } else {
            v0->unk_300[0][191 - v5] = 0;
            v0->unk_300[1][191 - v5] = 128;

            v1->unk_300[0][v5] = 128;
            v1->unk_300[1][v5] = 255;
        }
    }

    for (v5 = 96; v5 < 192; v5++) {
        if (param0->unk_0C.unk_00 < ((90 * 0xffff) / 360)) {
            v0->unk_300[0][191 - v5] = 128;
            v0->unk_300[1][191 - v5] = 128;

            v1->unk_300[0][v5] = 128;
            v1->unk_300[1][v5] = 128;
        } else {
            v3 = sub_020109D8(((90 * 0xffff) / 360) - v2, (v5 - 96));

            if (v3 > 127) {
                v3 = 127;
            }

            v0->unk_300[0][191 - v5] = 0;
            v0->unk_300[1][191 - v5] = 128 - v3;

            v1->unk_300[0][v5] = 128 + v3;
            v1->unk_300[1][v5] = 255;
        }
    }
}

void sub_020125D4(UnkStruct_02011C7C *param0, int param1, int param2) {
    int v0 = param0->unk_08 * param1;
    v0 = v0 / param2;

    param0->unk_00 = v0 + param0->unk_04;
}

void sub_020125EC(ScreenFade *param0, UnkStruct_0200FEA4 *param1) {
    UnkStruct_02011AFC *v0;

    param0->data = Heap_Alloc(param0->heapID, sizeof(UnkStruct_02011AFC));
    memset(param0->data, 0, sizeof(UnkStruct_02011AFC));

    v0 = param0->data;
    sub_0201268C(v0, param1, param0->steps, param0->framesPerStep, param0->screen, param0->hwSettings, param0->hblanks, param0->heapID);

    param0->state++;
}

BOOL sub_0201262C(ScreenFade *param0) {
    UnkStruct_02011AFC *v0;
    BOOL v1;
    BOOL v2 = 0;

    v0 = (UnkStruct_02011AFC *)param0->data;

    switch (param0->state) {
    case 1:
        v1 = sub_0201275C(v0);

        if (v1 == 1) {
            sub_02010F34(v0->unk_28, v0->unk_30, param0->screen);
            param0->state++;
        }
        break;
    case 2:
        sub_020127B4(v0);
        sub_02010EC8(&v0->unk_00);
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
