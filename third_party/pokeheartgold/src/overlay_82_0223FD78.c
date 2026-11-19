#include "global.h"

#include "bg_window.h"
#include "render_window.h"
#include "overlay_82.h"
#include "sys_task.h"

void ov82_0223FD78(Window *arg0, u8 arg1);

SysTask *YesNoPrompt_Create(SysTaskFunc callback, void *param, u32 priority);
SysTask *YesNoPrompt_Destroy(SysTaskFunc callback, void *param, u32 priority);
SysTask *ov82_0223FDC0(SysTaskFunc callback, void *param, int priority);

void ov82_0223FD78(Window *window, u8 frame) {
    LoadUserFrameGfx2(window->bgConfig, (enum GFBgLayer)GetWindowBgId(window), 0x3D9U, 0xAU, (u8)(s32)frame, HEAP_ID_105);
    FillWindowPixelBuffer(window, 0xFU);
    DrawFrameAndWindow2(window, 0, 0x3D9U, 0xAU);
}

SysTask *ov82_0223FDB8(SysTaskFunc callback, void *param, int priority) {
    return YesNoPrompt_Create(callback, param, priority);
}

SysTask *ov82_0223FDC0(SysTaskFunc callback, void *param, int priority) {
    return YesNoPrompt_Destroy(callback, param, priority);
}
