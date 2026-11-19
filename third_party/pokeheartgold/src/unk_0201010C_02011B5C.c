#include "global.h"

#include "screen_fade_fx.h"
#include "sys_task.h"

struct HardwareWindowSettings;
struct ScreenFade;
struct ScreenFadeHBlanks;
struct SysTask;
struct SysTaskQueue;
void sub_02011BF0(UnkStruct_02011360 *param0, const UnkStruct_0200FC2C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
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
void sub_02011494(UnkStruct_02011568 *param0, const UnkStruct_0200FD34 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011738(UnkStruct_0201184C *param0, const UnkStruct_02011738 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011A00(UnkStruct_02011AFC *param0, UnkStruct_0200FEA4 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011D34(UnkStruct_02011E04 *param0, UnkStruct_0200FF30 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02011FE8(UnkStruct_020120D4 *param0, UnkStruct_0200FE6C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void sub_02012290(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, int param7);
void sub_02012384(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
void Heap_Free(void *ptr);
void sub_02010EC8(UnkStruct_02010588 *param0);
static BOOL sub_02011CB8(UnkStruct_02011360 *param0);
void sub_02011298(UnkStruct_02011360 *param0, const UnkStruct_0200FC2C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
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
void sub_02011D08(UnkStruct_02011360 *param0);
void sub_02010E64(UnkStruct_02010588 *param0, int param1, enum DSScreen screen, enum HeapID heapID);
void sub_02011068(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);
void sub_0200FFB4(ScreenFadeHBlanks *hblanks, enum DSScreen screen, enum HeapID heapID);

void sub_02011B5C(ScreenFade *param0, const UnkStruct_0200FC2C *param1);
BOOL sub_02011B94(ScreenFade *param0);
void sub_02011BF0(UnkStruct_02011360 *param0, const UnkStruct_0200FC2C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);
static BOOL sub_02011CB8(UnkStruct_02011360 *param0);

void sub_02011B5C(ScreenFade *param0, const UnkStruct_0200FC2C *param1) {
    UnkStruct_02011360 *v0;

    param0->data = Heap_Alloc(param0->heapID, sizeof(UnkStruct_02011360));
    v0 = (UnkStruct_02011360 *)param0->data;

    sub_02011BF0(v0, param1, param0->steps, param0->framesPerStep, param0->screen, param0->hwSettings, param0->hblanks, param0->heapID);
    param0->state++;
}

BOOL sub_02011B94(ScreenFade *param0) {
    UnkStruct_02011360 *v0;
    BOOL v1;
    BOOL v2 = 0;

    v0 = (UnkStruct_02011360 *)param0->data;

    switch (param0->state) {
    case 1:
        v1 = sub_02011CB8(v0);

        if (v1 == 1) {
            sub_02010F34(v0->unk_20, v0->unk_24, param0->screen);
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

void sub_02011BF0(UnkStruct_02011360 *param0, const UnkStruct_0200FC2C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID) {
    UnkStruct_02010FC0 *v0;

    param0->unk_10 = sub_02010A6C(param1->unk_00, param1->unk_02, param2);
    sub_02010E64(&param0->unk_00, param1->unk_04, screen, heapID);

    param0->unk_0C = param1->unk_00 * 128;
    param0->unk_14 = param2;
    param0->unk_18 = param3;
    param0->unk_1C = 0;
    param0->unk_24 = param5;
    param0->unk_28 = param6;
    param0->heapID = heapID;
    param0->unk_20 = param1->unk_07;

    sub_02011D08(param0);
    SysTask_CreateOnVWaitQueue(sub_02010F00, &param0->unk_00, 1023);

    v0 = sub_02010EE0(&param0->unk_00, 0);
    sub_02010F84(param5, param1->unk_05, param1->unk_06, param1->unk_04, screen, v0->unk_300[0][0], 0, v0->unk_300[1][0], 192, param0->unk_20);

    if (param1->unk_04 == 0) {
        sub_02011068(param5, GX_WNDMASK_W0, screen, param0->unk_20);
    } else {
        sub_02011068(param5, GX_WNDMASK_W1, screen, param0->unk_20);
    }

    sub_0200FF88(param0->unk_28, &param0->unk_00, sub_02010C38, screen, heapID);
}

static BOOL sub_02011CB8(UnkStruct_02011360 *param0) {
    param0->unk_1C++;

    if (param0->unk_1C >= param0->unk_18) {
        param0->unk_1C = 0;

        if ((param0->unk_14 - 1) > 0) {
            param0->unk_14--;
            param0->unk_0C += param0->unk_10;
            sub_02011D08(param0);
            SysTask_CreateOnVWaitQueue(sub_02010F00, &param0->unk_00, 1023);
        } else {
            sub_0200FFB4(param0->unk_28, param0->unk_00.screen, param0->heapID);
            return 1;
        }
    }

    return 0;
}
