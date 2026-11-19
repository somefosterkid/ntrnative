#include "global.h"

#include "unk_02012DD8.h"
#include "screen_fade_internal.h"
#include "sys_task.h"


struct SysTask;
struct SysTaskQueue;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
SysTask * SysTask_CreateOnVWaitQueue(SysTaskFunc func, void *data, u32 priority);
void sub_020134BC(SysTask *task, void *data);
void sub_02013364(int left, int top, int right, int bottom, enum HardwareWindow windowID, enum DSScreen screen);
void sub_02013440(HardwareWindowSettings *settings, int wnd, BOOL applyColorEffect, enum HardwareWindow windowID, enum DSScreen screen);
void sub_02013468(HardwareWindowSettings *settings, int wnd, BOOL applyColorEffect, enum DSScreen screen);
void sub_02013488(HardwareWindowSettings *settings, int left, int top, int right, int bottom, enum HardwareWindow windowID, enum DSScreen screen);
void sub_020134D0(SysTask *task, void *data);
void sub_02013424(HardwareWindowSettings *settings, GXWndMask windowMask, enum DSScreen screen);
void sub_020134EC(SysTask *task, void *data);
void sub_02013504(SysTask *task, void *data);
void sub_02013364(int left, int top, int right, int bottom, enum HardwareWindow windowID, enum DSScreen screen);
void sub_02013424(HardwareWindowSettings *settings, GXWndMask windowMask, enum DSScreen screen);
void sub_02013440(HardwareWindowSettings *settings, int wnd, BOOL applyColorEffect, enum HardwareWindow windowID, enum DSScreen screen);
void sub_02013468(HardwareWindowSettings *settings, int wnd, BOOL applyColorEffect, enum DSScreen screen);
void sub_02013488(HardwareWindowSettings *settings, int left, int top, int right, int bottom, enum HardwareWindow windowID, enum DSScreen screen);

GXWndPlane sub_0201333C(u32 screen) {
    if (screen == 0) {
        return G2_GetWndOutsidePlane();
    } else {
        return G2S_GetWndOutsidePlane();
    }
}

void sub_02013364(int left, int top, int right, int bottom, enum HardwareWindow windowID, enum DSScreen screen) {
    if (windowID == HW_WINDOW_WND0) {
        if (screen == DS_SCREEN_MAIN) {
            G2_SetWnd0Position(left, top, right, bottom);
        } else {
            G2S_SetWnd0Position(left, top, right, bottom);
        }
    } else {
        if (screen == DS_SCREEN_MAIN) {
            G2_SetWnd1Position(left, top, right, bottom);
        } else {
            G2S_SetWnd1Position(left, top, right, bottom);
        }
    }
}

void sub_02013424(HardwareWindowSettings *settings, GXWndMask windowMask, enum DSScreen screen) {
    HardwareWindowVisibility *visible = &settings->visible[screen];
    visible->windowMask = windowMask;
    visible->screen = screen;

    SysTask_CreateOnVWaitQueue(sub_020134BC, visible, 1);
}

void sub_02013440(HardwareWindowSettings *settings, int wnd, BOOL applyColorEffect, enum HardwareWindow windowID, enum DSScreen screen) {
    HardwareWindowInsidePlane *inside = &settings->inside[screen][windowID];
    inside->wnd = wnd;
    inside->applyColorEffect = applyColorEffect;
    inside->windowID = windowID;
    inside->screen = screen;

    SysTask_CreateOnVWaitQueue(sub_020134D0, inside, 1);
}

void sub_02013468(HardwareWindowSettings *settings, int wnd, BOOL applyColorEffect, enum DSScreen screen) {
    HardwareWindowOutsidePlane *outside = &settings->outside[screen];
    outside->wnd = wnd;
    outside->applyColorEffect = applyColorEffect;
    outside->screen = screen;

    SysTask_CreateOnVWaitQueue(sub_020134EC, outside, 1);
}

void sub_02013488(HardwareWindowSettings *settings, int left, int top, int right, int bottom, enum HardwareWindow windowID, enum DSScreen screen) {
    HardwareWindowDimensions *v0 = &settings->dimensions[screen][windowID];
    v0->left = left;
    v0->top = top;
    v0->right = right;
    v0->bottom = bottom;
    v0->windowID = windowID;
    v0->screen = screen;

    SysTask_CreateOnVWaitQueue(sub_02013504, v0, 1);
}
