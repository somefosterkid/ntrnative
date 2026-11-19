#include "global.h"

#include "bg_window.h"
#include "render_window.h"

u8 ov41_0224AE24(Window *, s32, s32, s32, s32, s32, s32, s32);

s32 ov41_0224AD90(Window *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

s32 ov41_0224AD90(Window *window, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 v0;

    FillWindowPixelBuffer(window, 0xFU);
    v0 = ov41_0224AE24(window, arg1, arg2, arg3, arg4, arg5, 0x1020F, arg6);
    DrawFrameAndWindow2(window, 0, 1U, 1U);
    return v0;
}

u8 ov41_0224AE78(Window *, s32, s32, s32, s32, s32, s32, s32, void *);

s32 ov41_0224ADD8(Window *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, void *arg7);

s32 ov41_0224ADD8(Window *window, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, void *arg7) {
    s32 v0;

    FillWindowPixelBuffer(window, 0xFU);
    v0 = ov41_0224AE78(window, arg1, arg2, arg3, arg4, arg5, 0x1020F, arg6, arg7);
    DrawFrameAndWindow2(window, 0, 1U, 1U);
    return v0;
}
