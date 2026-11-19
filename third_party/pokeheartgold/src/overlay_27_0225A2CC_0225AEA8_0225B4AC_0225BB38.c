#include "global.h"

#include "bg_window.h"
#include "overlay_27.h"
#include "text.h"

extern s32 ov27_0225CEC4;

void ov27_0225BB38(Window *window, String *string, s32 arg2) {
    u32 v0;

    v0 = *(u32 *)(&ov27_0225CEC4 + arg2);
    FillWindowPixelBuffer(window, (u8)v0);
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 3U, 0U, 0U, v0, NULL);
}
