#include "global.h"
#include "overlay_81.h"

#include "bg_window.h"
#include "render_window.h"
#include "font.h"
#include "text.h"

void ov81_02243068(Window *arg0, String *arg1, u32 arg2, u32 arg3, enum Font arg4, u32 arg5, s32 arg6);

void ov81_02243028(Window *window, u32 frame) {
    LoadUserFrameGfx2(window->bgConfig, (enum GFBgLayer)GetWindowBgId(window), 0x3E2U, 0xBU, (u8)frame, HEAP_ID_100);
    FillWindowPixelBuffer(window, 0xFU);
    DrawFrameAndWindow2(window, 1, 0x3E2U, 0xBU);
}

void ov81_02243068(Window *window, String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6) {
    u32 x;

    x = arg2;
    if (arg6 == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0U);
    } else if (arg6 == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0U) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFFU, color, NULL);
}
