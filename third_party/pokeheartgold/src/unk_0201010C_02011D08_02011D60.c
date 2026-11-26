#include "global.h"

#include "screen_fade_fx.h"

#include "heap.h"
#include "sys_task_api.h"

struct HardwareWindowSettings;

struct ScreenFade;

struct ScreenFadeHBlanks;





























void sub_02011DEC(UnkStruct_02011568 *param0, const UnkStruct_0200FD34 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

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

void sub_020117A0(UnkStruct_02010EA4 *param0, const UnkStruct_0200F980 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5);

void sub_02011918(UnkStruct_0201109C *param0, const UnkStruct_0200FB7C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011BF0(UnkStruct_02011360 *param0, const UnkStruct_0200FC2C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02012090(UnkStruct_0201184C *param0, const UnkStruct_02011738 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02012358(UnkStruct_02011AFC *param0, UnkStruct_0200FEA4 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_0201268C(UnkStruct_02011E04 *param0, UnkStruct_0200FF30 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02012940(UnkStruct_020120D4 *param0, UnkStruct_0200FE6C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02012BE8(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, int param7);

void sub_02012CDC(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011D60(ScreenFade *param0, const UnkStruct_0200FD34 *param1);

void sub_02011D60(ScreenFade *param0, const UnkStruct_0200FD34 *param1) {
    UnkStruct_02011568 *v0;

    param0->data = Heap_Alloc(param0->heapID, sizeof(UnkStruct_02011568));
    v0 = (UnkStruct_02011568 *)param0->data;

    sub_02011DEC(v0, param1, param0->steps, param0->framesPerStep, param0->screen, param0->hwSettings, param0->hblanks, param0->heapID);
    param0->state++;
}

void sub_02010EC8(UnkStruct_02010588 *param0);

static BOOL sub_02011EC0(UnkStruct_02011568 *param0);

void sub_02010710(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);

void sub_02010E48(UnkStruct_02010EA4 *param0, const UnkStruct_0200F980 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5);

void sub_02010FC0(UnkStruct_0201109C *param0, const UnkStruct_0200FB7C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011298(UnkStruct_02011360 *param0, const UnkStruct_0200FC2C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011494(UnkStruct_02011568 *param0, const UnkStruct_0200FD34 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011738(UnkStruct_0201184C *param0, const UnkStruct_02011738 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011A00(UnkStruct_02011AFC *param0, UnkStruct_0200FEA4 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011D34(UnkStruct_02011E04 *param0, UnkStruct_0200FF30 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011FE8(UnkStruct_020120D4 *param0, UnkStruct_0200FE6C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);


void sub_02012384(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

BOOL sub_02011D98(ScreenFade *param0);

BOOL sub_02011D98(ScreenFade *param0) {
    UnkStruct_02011568 *v0;
    BOOL v1;
    BOOL v2 = 0;

    v0 = (UnkStruct_02011568 *)param0->data;

    switch (param0->state) {
    case 1:
        v1 = sub_02011EC0(v0);

        if (v1 == 1) {
            sub_02010F34(v0->unk_24, v0->unk_28, param0->screen);
            param0->state++;
        }
        break;
    case 2:
        sub_02010EC8(&v0->unk_00);
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

struct SysTask;

struct SysTaskQueue;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void sub_0200FF88(ScreenFadeHBlanks *hblanks, void *data, Callback callback, enum DSScreen screen, enum HeapID heapID);

void sub_02010C38(void *param0);

UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *param0, int param1);

void sub_02010F00(SysTask *param0, void *param1);

static void sub_02011F10(UnkStruct_02011568 *param0);

void sub_02010E64(UnkStruct_02010588 *param0, int param1, enum DSScreen screen, enum HeapID heapID);

void sub_02011DEC(UnkStruct_02011568 *param0, const UnkStruct_0200FD34 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID) {
    int v0;
    UnkStruct_02010FC0 *v1;

    v0 = (param1->unk_02 - param1->unk_00);
    v0 /= param2;

    sub_02010E64(&param0->unk_00, param1->unk_04, screen, heapID);

    param0->unk_0C = 128 * FX32_ONE;
    param0->unk_10 = param1->unk_00;
    param0->unk_14 = v0;
    param0->unk_18 = param2;
    param0->unk_1C = param3;
    param0->unk_20 = 0;
    param0->unk_28 = param5;
    param0->unk_2C = param6;
    param0->heapID = heapID;
    param0->unk_24 = param1->unk_07;

    sub_02011F10(param0);
    SysTask_CreateOnVWaitQueue(sub_02010F00, &param0->unk_00, 1023);

    v1 = sub_02010EE0(&param0->unk_00, 0);
    sub_02010F84(param5, param1->unk_05, param1->unk_06, param1->unk_04, screen, v1->unk_300[0][96], 0, v1->unk_300[1][96], 192, param0->unk_24);

    if (param1->unk_04 == 0) {
        sub_02011068(param5, GX_WNDMASK_W0, screen, param0->unk_24);
    } else {
        sub_02011068(param5, GX_WNDMASK_W1, screen, param0->unk_24);
    }

    sub_0200FF88(param0->unk_2C, &param0->unk_00, sub_02010C38, screen, heapID);
}

void sub_0200FFB4(ScreenFadeHBlanks *hblanks, enum DSScreen screen, enum HeapID heapID);

static BOOL sub_02011EC0(UnkStruct_02011568 *param0) {
    param0->unk_20++;

    if (param0->unk_20 >= param0->unk_1C) {
        param0->unk_20 = 0;

        if ((param0->unk_18 - 1) > 0) {
            param0->unk_18--;
            param0->unk_10 += param0->unk_14;
            sub_02011F10(param0);
            SysTask_CreateOnVWaitQueue(sub_02010F00, &param0->unk_00, 1023);
        } else {
            sub_0200FFB4(param0->unk_2C, param0->unk_00.screen, param0->heapID);
            return 1;
        }
    }

    return 0;
}

void sub_02010A00(int param0, int *param1, int param2, int param3);

int sub_02010A54(int param0, int param1);

int sub_02010A7C(int param0, int param1);

static void sub_02011F10(UnkStruct_02011568 *param0) {
    int v0;
    int v1;
    int v2;
    int v3[192];
    int v4;
    int v5;
    int v6;
    int v7, v8;
    UnkStruct_02010FC0 *v9 = sub_02010EE0(&param0->unk_00, 0);
    v5 = FX_MulInline(FX_SinIdx(param0->unk_10), param0->unk_0C);

    v5 >>= 12;

    v2 = v5 * 2;
    v2 = v2 / 21;
    v2 += 1;
    v2 = 180 - (v2 * 2);
    v2 = ((0xffff * (v2)) / 360);
    v2 /= 2;
    v1 = sub_02010A54(v2, 256);
    v1 >>= 12;

    ((v1 < 192) ? (void)0 : GF_AssertFail());

    sub_02010A00(v2, v3, v1, 0);

    for (v0 = 0; v0 < 96; v0++) {
        v4 = v1 - (v0 + 1);
        v6 = v5;

        if (v4 > 0) {
            if (v3[v4] > v6) {
                v6 = v3[v4];
            }
        }

        v7 = sub_02010A7C(128, -v6);
        v8 = sub_02010A7C(128, v6);

        v9->unk_300[0][v0] = v7;
        v9->unk_300[1][v0] = v8;
        v9->unk_300[0][191 - v0] = v7;
        v9->unk_300[1][191 - v0] = v8;
    }
}
