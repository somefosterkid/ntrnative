#include "global.h"

#include "text.h"

u32 ov73_021EA050(Window *, String *, s32, s32, u32, s32);

void ov73_021EA088(Window *arg0, String *arg1, s32 arg2, u32 arg3, s32 arg4, u32 arg5);

void ov73_021EA088(Window *window, String *string, s32 arg2, u32 y, s32 arg4, u32 color) {
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, ov73_021EA050(window, string, arg2, arg4, color, 1), y, 0U, color, NULL);
}

void ov73_021EA0BC(Window *arg0, String *arg1, s32 arg2, u32 arg3, s32 arg4, u32 arg5);

void ov73_021EA0BC(Window *window, String *string, s32 arg2, u32 y, s32 arg4, u32 color) {
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, ov73_021EA050(window, string, arg2, arg4, color, 0), y, 0U, color, NULL);
}
