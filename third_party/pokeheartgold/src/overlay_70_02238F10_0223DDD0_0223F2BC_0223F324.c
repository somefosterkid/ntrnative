#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

u32 ov70_0223F2A0(s32, u32);

extern void *ov70_02245910;

void ov70_0223F324(Window *param_1, MsgData *param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6, u32 param_7);

void ov70_0223F324(Window *param_1, MsgData *param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6, u32 param_7) {
    String *string;
    u32 color;

    if ((param_4 != 0) || (param_3 != 3)) {
        string = NewString_ReadMsgData(param_2, *(s32 *)(&ov70_02245910 + param_3));
        color = ov70_0223F2A0(param_3, param_7);
        AddTextPrinterParameterizedWithColor(param_1, FONT_SYSTEM, string, param_5, param_6, 0xff, color, (PrinterCallback_t)0x0);
        String_Delete(string);
    }
    return;
}
