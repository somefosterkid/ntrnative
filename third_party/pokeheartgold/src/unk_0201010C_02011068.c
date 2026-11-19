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

void sub_020131F4(u32 visible, u32 screen);

void sub_02013424(WipeWnd *wnd, u32 visible, u32 screen);

void sub_02011068(WipeWnd *wnd, u32 visible, s32 screen, BOOL deferred);

void sub_02011080(UnkStruct_0201076C *arg0, s8 arg1, s8 arg2, s32 arg3, s32 arg4);

struct SysTask;
struct SysTaskQueue;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
SysTask * SysTask_CreateOnVWaitQueue(SysTaskFunc func, void *data, u32 priority);
void sub_02011104(SysTask *param0, void *param1);
void sub_020110C4(UnkStruct_0201076C *param0);

s32 sub_0200FF88(u8 *, void *, s32 *, enum DSScreen, s32);
extern s32 sub_02011130;
void sub_020110DC(void *arg0, UnkStruct_0201076C *arg1, s32 arg2);

void sub_02011068(WipeWnd *wnd, u32 visible, s32 screen, BOOL deferred) {
    if (deferred == 0) {
        sub_020131F4(visible, screen);
    } else {
        sub_02013424(wnd, visible, screen);
    }
}

void sub_02011080(UnkStruct_0201076C *arg0, s8 arg1, s8 arg2, s32 arg3, s32 arg4) {
    memset(arg0, 0, sizeof(UnkStruct_0201076C));

    if (arg2 == 1) {
        arg0->unk_00[0].unk_180 = arg3;
        arg0->unk_308 = arg2;
        arg0->screen = arg1;
        return;
    }

    arg0->unk_00[0].unk_180 = arg3;
    arg0->unk_00[1].unk_180 = arg4;
    arg0->unk_308 = arg2;
    arg0->screen = arg1;
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

void sub_020110C4(UnkStruct_0201076C *param0) {
    SysTask_CreateOnVWaitQueue(sub_02011104, param0, 1023);
}

void sub_020110DC(void *arg0, UnkStruct_0201076C *arg1, s32 arg2) {
    sub_0200FF88(arg0, arg1, &sub_02011130, (enum DSScreen)(u8)arg1->screen, arg2);  /* the DS call narrows the screen to u8 */
}
