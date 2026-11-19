#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

extern void ov51_021E74D4(Window windows[], u8 start, u8 end);

u8 ov51_021E74F4(Window *param_1, s32 param_2, s32 param_3, u32 param_4, String *param_5, s32 param_6, u8 param_7, PrintingMode param_8);
u8 ov51_021E7540(Window *param_1, s32 param_2, s32 param_3, u32 param_4, String *param_5);

void ov51_021E74D4(Window windows[], u8 start, u8 end) {
    for (u8 i = start; i <= end; i++) {
        ClearWindowTilemapAndCopyToVram(&windows[i]);
    }
}

u8 ov51_021E74F4(Window *param_1, s32 param_2, s32 param_3, u32 param_4, String *param_5, s32 param_6, u8 param_7, PrintingMode param_8) {
    u8 uVar1;
    u32 uVar2;

    String16_FormatInteger(param_5, param_6, (u32)param_7, param_8, 1);
    uVar2 = FontID_String_GetWidth(FONT_SYSTEM, param_5, 0);
    uVar1 = AddTextPrinterParameterizedWithColor(param_1, FONT_SYSTEM, param_5, param_2 - (uVar2 + param_3), param_4, 0xff, 0x10200, (PrinterCallback_t)0x0);
    return uVar1;
}

u8 ov51_021E7540(Window *param_1, s32 param_2, s32 param_3, u32 param_4, String *param_5) {
    u8 uVar1;
    u32 uVar2;

    uVar2 = FontID_String_GetWidth(FONT_SYSTEM, param_5, 0);
    uVar1 = AddTextPrinterParameterizedWithColor(param_1, FONT_SYSTEM, param_5, param_2 - (uVar2 + param_3), param_4, 0xff, 0x10200, (PrinterCallback_t)0x0);
    return uVar1;
}
