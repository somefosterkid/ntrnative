#include "global.h"

#include "bg_window.h"

void ov65_0221FAE0(Window *window);

void ov65_0221FAE0(Window *window) {
    s32 var_r5;
    s32 var_r5_2;

    var_r5 = 0;
    do {
        RemoveWindow(&window[var_r5 + 7]);
        var_r5 += 1;
    } while (var_r5 < 0xE);
    var_r5_2 = 0;
    do {
        RemoveWindow(&window[var_r5_2 + 0x1A]);
        var_r5_2 += 1;
    } while (var_r5_2 < 8);
    RemoveWindow(window);
    RemoveWindow(((void *)window) + 0x10);
    RemoveWindow(((void *)window) + 0x60);
    RemoveWindow(((void *)window) + 0x150);
    RemoveWindow(((void *)window) + 0x160);
    RemoveWindow(((void *)window) + 0x170);
    RemoveWindow(((void *)window) + 0x190);
}
