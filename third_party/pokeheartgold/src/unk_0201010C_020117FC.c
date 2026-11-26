#include "global.h"

#include "screen_fade_fx.h"
#include "sys_task.h"

struct WipeWnd;

typedef struct WipeWnd WipeWnd;

typedef struct {
    u32 visible;
    u32 screen;
} WipeWndDisp;

typedef struct {
    u32 planes;
    BOOL effect;
    u32 window;
    u32 screen;
} WipeWndIn;

typedef struct {
    u32 planes;
    BOOL effect;
    u32 screen;
} WipeWndOut;

typedef struct {
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    u32 window;
    u32 screen;
} WipeWndPos;

struct WipeWnd {
    WipeWndDisp disp[2];
    WipeWndIn in[2][2];
    u8 unk50[0x18];
    WipeWndOut out[2];
    WipeWndPos pos[2][2];
};

void sub_02013488(WipeWnd *wnd, int x1, int y1, int x2, int y2, u32 window, u32 screen);

typedef struct {
    s32 cur[4];
    s32 step[4];
    s32 end[4];
    s32 screen;
    s32 window;
    s32 division;
    s32 sync;
    s32 syncCount;
    BOOL isOut;
    WipeWnd *wnd;
} WipeRectWork;

void sub_02010A8C(s32 *cur, s32 *step);

BOOL sub_020117FC(WipeRectWork *work);

struct HardwareWindowSettings;
struct ScreenFade;
struct ScreenFadeHBlanks;
struct SysTask;
struct SysTaskQueue;
void sub_02011918(UnkStruct_0201109C *param0, const UnkStruct_0200FB7C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02010F34(int param0, HardwareWindowSettings *param1, enum DSScreen screen);
void sub_02010F84(HardwareWindowSettings *param0, int param1, int param2, int param3, enum DSScreen screen, int param5, int param6, int param7, int param8, int param9);
void sub_02010710(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);
void sub_02010E48(UnkStruct_02010EA4 *param0, const UnkStruct_0200F980 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5);
void sub_02011298(UnkStruct_02011360 *param0, const UnkStruct_0200FC2C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011494(UnkStruct_02011568 *param0, const UnkStruct_0200FD34 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011738(UnkStruct_0201184C *param0, const UnkStruct_02011738 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011A00(UnkStruct_02011AFC *param0, UnkStruct_0200FEA4 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011D34(UnkStruct_02011E04 *param0, UnkStruct_0200FF30 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011FE8(UnkStruct_020120D4 *param0, UnkStruct_0200FE6C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02012384(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void Heap_Free(void *ptr);
void sub_02010EC8(UnkStruct_02010588 *param0);
BOOL sub_020119F4(UnkStruct_0201109C *param0);
void sub_02010FC0(UnkStruct_0201109C *param0, const UnkStruct_0200FB7C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
void sub_0200FF88(ScreenFadeHBlanks *hblanks, void *data, Callback callback, enum DSScreen screen, enum HeapID heapID);
SysTask * SysTask_CreateOnVWaitQueue(SysTaskFunc func, void *data, u32 priority);
int sub_02010A6C(int param0, int param1, int param2);
void sub_02010C38(void *param0);
UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *param0, int param1);
void sub_02010F00(SysTask *param0, void *param1);
void sub_02011AD8(UnkStruct_0201109C *param0);
void sub_02010E64(UnkStruct_02010588 *param0, int param1, enum DSScreen screen, enum HeapID heapID);
void sub_02011068(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);
void sub_0200FFB4(ScreenFadeHBlanks *hblanks, enum DSScreen screen, enum HeapID heapID);
void sub_02011884(ScreenFade *param0, const UnkStruct_0200FB7C *param1);
BOOL sub_020118BC(ScreenFade *param0);
void sub_02011918(UnkStruct_0201109C *param0, const UnkStruct_0200FB7C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
BOOL sub_020119F4(UnkStruct_0201109C *param0);
void sub_02011A44(int param0, int param1, int param2, int param3, int *param4, int *param5);

BOOL sub_020117FC(WipeRectWork *work) {
    work->syncCount++;
    if (work->syncCount >= work->sync) {
        work->syncCount = 0;
        if (work->division - 1 > 0) {
            work->division--;
            sub_02010A8C(work->cur, work->step);
        } else {
            sub_02013488(work->wnd, work->end[0], work->end[1], work->end[2], work->end[3], work->window, work->screen);
            return 1;
        }
        sub_02013488(work->wnd, work->cur[0] / 128, work->cur[1] / 128, work->cur[2] / 128, work->cur[3] / 128, work->window, work->screen);
    }
    return 0;
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

void sub_02011884(ScreenFade *param0, const UnkStruct_0200FB7C *param1) {
    UnkStruct_0201109C *v0;

    param0->data = Heap_Alloc(param0->heapID, sizeof(UnkStruct_0201109C));
    v0 = (UnkStruct_0201109C *)param0->data;

    sub_02011918(v0, param1, param0->steps, param0->framesPerStep, param0->screen, param0->hwSettings, param0->hblanks, param0->heapID);

    param0->state++;
}

BOOL sub_020118BC(ScreenFade *param0) {
    UnkStruct_0201109C *v0;
    BOOL v1;
    BOOL v2 = 0;

    v0 = (UnkStruct_0201109C *)param0->data;

    switch (param0->state) {
    case 1:
        v1 = sub_020119F4(v0);

        if (v1 == 1) {
            sub_02010F34(v0->unk_2C, v0->unk_30, param0->screen);
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
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return v2;
}

void sub_02011918(UnkStruct_0201109C *param0, const UnkStruct_0200FB7C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID) {
    int v0;
    UnkStruct_02010FC0 *v1;

    v0 = sub_02010A6C(param1->unk_00, param1->unk_02, param2);
    sub_02010E64(&param0->unk_00, param1->unk_08, screen, heapID);

    param0->unk_0C = param1->unk_00 * 128;
    param0->unk_10 = param1->unk_04;
    param0->unk_14 = param1->unk_06;
    param0->unk_18 = v0;
    param0->unk_1C = param2;
    param0->unk_20 = param3;
    param0->unk_24 = 0;
    param0->unk_30 = param5;
    param0->unk_34 = param6;
    param0->heapID = heapID;
    param0->unk_2C = param1->unk_0B;

    sub_02011AD8(param0);
    SysTask_CreateOnVWaitQueue(sub_02010F00, &param0->unk_00, 1023);

    v1 = sub_02010EE0(&param0->unk_00, 0);
    sub_02010F84(param5, param1->unk_09, param1->unk_0A, param1->unk_08, screen, v1->unk_300[0][0], 0, v1->unk_300[1][0], 192, param0->unk_2C);

    if (param1->unk_08 == 0) {
        sub_02011068(param5, GX_WNDMASK_W0, screen, param0->unk_2C);
    } else {
        sub_02011068(param5, GX_WNDMASK_W1, screen, param0->unk_2C);
    }

    sub_0200FF88(param0->unk_34, &param0->unk_00, sub_02010C38, screen, heapID);
}

BOOL sub_020119F4(UnkStruct_0201109C *param0) {
    param0->unk_24++;

    if (param0->unk_24 >= param0->unk_20) {
        param0->unk_24 = 0;

        if ((param0->unk_1C - 1) > 0) {
            param0->unk_1C--;
            param0->unk_0C += param0->unk_18;
            sub_02011AD8(param0);
            SysTask_CreateOnVWaitQueue(sub_02010F00, &param0->unk_00, 1023);
        } else {
            sub_0200FFB4(param0->unk_34, param0->unk_00.screen, param0->heapID);
            return 1;
        }
    }

    return 0;
}

void sub_02011A44(int param0, int param1, int param2, int param3, int *param4, int *param5) {
    fx32 v0;
    fx32 v1;
    fx32 v2;

    v0 = param0 / 128;
    v1 = param3 - param2;

    if (v1 < 0) {
        v1 = -v1;
    }

    if (v1 >= v0) {
        *param4 = 0;
        *param5 = 0;
    } else {
        v1 <<= 12;
        v0 <<= 12;
        v2 = FX_Sqrt(FX_MulInline(v0, v0) - FX_MulInline(v1, v1));
        v2 >>= 12;

        *param4 = param1 - v2;

        if (*param4 < 0) {
            *param4 = 0;
        }

        *param5 = *param4 + (v2 * 2);

        if (*param5 > 255) {
            *param5 = 255;
        }
    }
}
