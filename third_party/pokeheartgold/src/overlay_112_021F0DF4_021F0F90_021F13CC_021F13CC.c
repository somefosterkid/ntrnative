#include "global.h"

#include "overlay_112_sprite_app.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

void ov112_021F13CC(Ov112SpriteApp *arg0, u32 arg1, u32 arg2);

void ov112_021F13CC(Ov112SpriteApp *arg0, u32 arg1, u32 arg2) {
    Window *sp10;
    String *temp_r0_2;
    String *temp_r6;
    String *temp_r7;
    Window *temp_r0;
    u16 temp_r4;

    temp_r0 = &arg0->windows[2];
    sp10 = temp_r0;
    FillWindowPixelBuffer(temp_r0, 0U);
    temp_r6 = ReadMsgData_ExpandPlaceholders(arg0->msgFormat, arg0->msgData[0], arg1, arg0->heapID);
    if (arg2 != 0) {
        temp_r7 = ReadMsgData_ExpandPlaceholders(arg0->msgFormat, arg0->msgData[0], arg2, arg0->heapID);
        temp_r4 = String_GetLength(temp_r6);
        temp_r0_2 = String_New(temp_r4 + String_GetLength(temp_r7) + 2, arg0->heapID);
        String_Copy(temp_r0_2, temp_r6);
        String_AddChar(temp_r0_2, 0xE000U);
        String_Cat(temp_r0_2, temp_r7);
        AddTextPrinterParameterizedWithColor(sp10, FONT_MESSAGE, temp_r0_2, 0U, 0U, 0xFFU, 0x10200U, NULL);
        String_Delete(temp_r7);
        String_Delete(temp_r0_2);
    } else {
        AddTextPrinterParameterizedWithColor(sp10, FONT_MESSAGE, temp_r6, 0U, 0U, 0xFFU, 0x10200U, NULL);
    }
    String_Delete(temp_r6);
    CopyWindowToVram(sp10);
}
