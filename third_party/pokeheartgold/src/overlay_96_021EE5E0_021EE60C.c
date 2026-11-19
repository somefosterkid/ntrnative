#include "global.h"

#include "bg_window.h"

extern WindowTemplate ov96_0221B19A;

void ov96_021EE60C(void *arg0, void *arg1);

void ov96_021EE60C(void *arg0, void *arg1) {
    WindowTemplate *var_r4;
    s32 var_r6;
    void *var_r5;
    void *var_r7;

    var_r7 = arg0;
    var_r5 = var_r7;
    var_r4 = &ov96_0221B19A;
    *(void **)((u8 *)var_r7 + 4) = arg1;
    var_r6 = 0;
    var_r5 += 0x10;
    do {
        AddWindow((BgConfig *)*(void **)((u8 *)var_r7 + 4), (Window *)var_r5, var_r4);
        FillWindowPixelBuffer((Window *)var_r5, 0U);
        ClearWindowTilemap((Window *)var_r5);
        var_r6 += 1;
        var_r4 = (void *)((u8 *)var_r4 + (8));
        var_r5 += (s32)sizeof(Window);
    } while (var_r6 < 2);
}
