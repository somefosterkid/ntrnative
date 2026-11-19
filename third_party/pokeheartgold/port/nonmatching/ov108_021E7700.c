/* PORTOK hk39 */
#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"

u8 ov108_021E7700(void *param_1, u8 param_2, u8 param_3) {
    Window *window;
    String **strings;
    u8 width;
    u8 height;
    u8 x;
    u32 color;

    window = (Window *)((u8 *)param_1 + 0x3B4) + param_2;
    width = (GetWindowWidth(window) & 0x1F) * 8;
    height = (GetWindowHeight(window) & 0x1F) * 8;
    color = 0x30102;
    if (param_2 != 2) {
        strings = (String **)((u8 *)param_1 + 0x324);
        x = 0;
        if (param_2 <= 1) {
            color = 0xD0C0E;
            FillWindowPixelBuffer(window, 0xE);
            FillWindowPixelRect(window, 6, 0, 0, width, 2);
            FillWindowPixelRect(window, 6, 0, height - 2, width, 2);
        } else {
            FillWindowPixelBuffer(window, 2);
        }
    } else {
        strings = (String **)((u8 *)param_1 + 0x310);
        x = (width - FontID_String_GetWidth(FONT_SYSTEM, strings[param_3], 0)) >> 1;
    }
    return AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, strings[param_3], x, 4, 0, color, NULL);
}
