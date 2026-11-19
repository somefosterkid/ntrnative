#include "global.h"

#include "bg_window.h"
#include "render_window.h"
#include "font.h"
#include "text.h"

extern const struct {
    const WindowTemplate *templates;
    u32 numTemplates;
} ov83_0224819C[2];

void ov83_022478D4(BgConfig *bgConfigs, Window *windows, u8 isOpponentApp);

void ov83_02247988(s16 *arg0, s16 *arg1);

void ov83_02247998(Window *arg0, String *arg1, u32 arg2, u32 arg3, enum Font arg4, u32 arg5, s32 arg6);

void ov83_022478D4(BgConfig *bgConfigs, Window *windows, u8 isOpponentApp) {
    const WindowTemplate *template = ov83_0224819C[isOpponentApp].templates;
    u32 numTemplates = ov83_0224819C[isOpponentApp].numTemplates;

    for (u8 i = 0; i < numTemplates; i++) {
        AddWindow(bgConfigs, &windows[i], &template[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

void ov83_0224791C(Window *windows, u8 isOpponentApp);

void ov83_0224791C(Window *windows, u8 isOpponentApp) {
    u16 i;
    u32 numTemplates = ov83_0224819C[isOpponentApp].numTemplates;

    for (i = 0; i < numTemplates; i++) {
        RemoveWindow(&windows[i]);
    }
}

void ov83_02247944(Window *arg0, u8 arg1);

void ov83_02247944(Window *window, u8 frame) {
    LoadUserFrameGfx2(window->bgConfig, (enum GFBgLayer)GetWindowBgId(window), 0x3D9U, 0xAU, (u8)(s32)frame, HEAP_ID_107);
    FillWindowPixelBuffer(window, 0xFU);
    DrawFrameAndWindow2(window, 1, 0x3D9U, 0xAU);
    ScheduleWindowCopyToVram(window);
}

void ov83_02247988(s16 *arg0, s16 *arg1) {
    *arg1 = 0xF0;
    *arg0 = 0x2E9;
}

void ov83_02247998(Window *window, String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6) {
    u32 x;

    x = arg2;
    if (arg6 == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0U);
    } else if (arg6 == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0U) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFFU, color, NULL);
}
