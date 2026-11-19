#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_43_palpad.h"

void ov43_0222DD88(PalPadDetailsScreen *screens);

void ov43_0222DD88(PalPadDetailsScreen *screens) {
    s32 iVar1;

    iVar1 = 0;
    do {
        Heap_Free(screens->file);
        iVar1 = iVar1 + 1;
        screens = screens + 1;
    } while (iVar1 < 4);
    return;
}

extern u32 ov43_0222EF20;

void ov43_0222DDA0(PalPadDetails *details, PalPadGfx *gfx);

void ov43_0222DDA0(PalPadDetails *details, PalPadGfx *gfx) {
    WindowTemplate *template;
    Window *window;
    s32 iVar1;

    template = (WindowTemplate *)&ov43_0222EF20;
    iVar1 = 0;
    window = details->profileWindows;
    do {
        AddWindow(gfx->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        iVar1 = iVar1 + 1;
        template = template + 1;
        window = window + 1;
    } while (iVar1 < 8);
    return;
}

void ov43_0222DDD0(PalPadDetails *details);

void ov43_0222DDD0(PalPadDetails *details) {
    s32 iVar1;
    Window *window;

    iVar1 = 0;
    window = details->profileWindows;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 8);
    return;
}
