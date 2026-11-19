#include "global.h"

#include "text.h"

u32 ov72_0223A3A8(Window *, String *, s32, s32, u32, s32);

void ov72_0223A3E0(Window *arg0, String *arg1, s32 arg2, u32 arg3, s32 arg4, u32 arg5);

void ov72_0223A3E0(Window *window, String *string, s32 arg2, u32 y, s32 arg4, u32 color) {
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, ov72_0223A3A8(window, string, arg2, arg4, color, 1), y, 0U, color, NULL);
}
