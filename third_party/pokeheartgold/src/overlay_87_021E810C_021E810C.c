#include "global.h"

#include "bg_window.h"
#include "render_window.h"

void ov87_021E810C(BgConfig *arg0, Window *arg1);

void ov87_021E810C(BgConfig *arg0, Window *window) {
    sub_0200E398(arg0, 2U, 0x3F7U, 0U, HEAP_ID_122);
    DrawFrameAndWindow1(window, 1, 0x3F7U, 0xBU);
}

void ov87_021E8134(Window *arg0, u8 arg1);

void ov87_021E8134(Window *window, u8 frame) {
    LoadUserFrameGfx2(window->bgConfig, (enum GFBgLayer)GetWindowBgId(window), 0x3D9U, 0xAU, (u8)(s32)frame, HEAP_ID_122);
    FillWindowPixelBuffer(window, 0xFU);
    DrawFrameAndWindow2(window, 0, 0x3D9U, 0xAU);
}
