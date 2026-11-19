#include "global.h"

#include "bg_window.h"

extern u32 ov81_022435A8;

void ov81_02242FDC(BgConfig *bgConfig, Window *window);

void ov81_0224300C(Window *windows);

void ov81_02242FDC(BgConfig *bgConfig, Window *window) {
    u32 uVar1;

    uVar1 = 0;
    do {
        AddWindow(bgConfig, window + uVar1, (WindowTemplate *)(&ov81_022435A8 + uVar1 * 2));
        FillWindowPixelBuffer(window + uVar1, 0);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 0x12);
    return;
}

void ov81_0224300C(Window *windows) {
    u32 uVar1;

    uVar1 = 0;
    do {
        RemoveWindow(windows + uVar1);
        uVar1 = uVar1 + 1 & 0xffff;
    } while (uVar1 < 0x12);
    return;
}
