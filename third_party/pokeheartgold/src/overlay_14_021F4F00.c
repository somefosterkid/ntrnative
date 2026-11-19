#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

u8 ov14_021F4F24(Window *param_1, String *param_2, u32 param_3, u32 param_4, enum Font param_5, u32 param_6, s32 param_7);

void ov14_021F4F00(u8 *param_1) {
    u32 uVar1;

    uVar1 = 0;
    do {
        RemoveWindow((Window *)(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x30 + uVar1 * sizeof(Window)));
        uVar1 = uVar1 + 1 & 0xffff;
    } while (uVar1 < 0x2c);
    FontID_Release(FONT_UNK_4);
    return;
}

u8 ov14_021F4F24(Window *param_1, String *param_2, u32 param_3, u32 param_4, enum Font param_5, u32 param_6, s32 param_7) {
    u8 uVar1;
    u32 uVar2;

    if (param_7 == 1) {
        uVar2 = FontID_String_GetWidth(param_5, param_2, 0);
        param_3 = param_3 - uVar2;
    } else if (param_7 == 2) {
        uVar2 = FontID_String_GetWidth(param_5, param_2, 0);
        param_3 = param_3 - (uVar2 >> 1);
    } else if (param_7 == 3) {
        uVar2 = FontID_String_GetWidthMultiline(param_5, param_2, 0);
        param_3 = param_3 - (uVar2 >> 1);
    }
    uVar1 = AddTextPrinterParameterizedWithColor(param_1, param_5, param_2, param_3, param_4, 0xff, param_6, (PrinterCallback_t)0x0);
    return uVar1;
}
