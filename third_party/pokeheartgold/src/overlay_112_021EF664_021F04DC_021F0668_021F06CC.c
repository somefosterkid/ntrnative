#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_112_sub_screen.h"

void ov112_021F06CC(Ov112SubScreen *arg0, void *arg1);

void ov112_021F06CC(Ov112SubScreen *arg0, void *arg1) {
    unsigned int new_var;
    u32 sp10;
    String *temp_r0;
    String *temp_r4;
    String *temp_r4_2;
    String *temp_r4_3;
    String *temp_r4_4;
    String *temp_r6;
    String *temp_r6_2;
    u8 temp_r0_2;
    u8 temp_r7;
    u8 temp_r7_2;
    temp_r7 = *((u8 *)(((u8 *)arg1) + 0x28));
    sp10 = (u32)(*((u16 *)(((u8 *)arg1) + 0x26)));
    CopyU16ArrayToString(arg0->string, arg1 + 0xA);
    FillWindowPixelBuffer(&arg0->windows[3], 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[3], FONT_SYSTEM, arg0->string, 0U, 0U, 0xFFU, 0x10200U, 0L);
    CopyWindowToVram(&arg0->windows[3]);
    temp_r0 = GetSpeciesName(*((u16 *)(((u8 *)arg1) + 4)), arg0->heapID);
    BufferString(arg0->msgFormat, 0U, temp_r0, 2, 1, 2);
    String_Delete(temp_r0);
    temp_r6 = ReadMsgData_ExpandPlaceholders(arg0->msgFormat, arg0->msgData, 0U, arg0->heapID);
    FillWindowPixelBuffer(&arg0->windows[0], 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[0], FONT_SYSTEM, temp_r6, 0U, 0U, 0xFFU, 0x10200U, 0L);
    CopyWindowToVram(&arg0->windows[0]);
    String_Delete(temp_r6);
    BufferIntegerAsString(arg0->msgFormat, 0U, (s32)temp_r7, 3U, PRINTING_MODE_RIGHT_ALIGN, 1);
    temp_r6_2 = ReadMsgData_ExpandPlaceholders(arg0->msgFormat, arg0->msgData, 3U, arg0->heapID);
    FillWindowPixelBuffer(&arg0->windows[1], 0U);
    temp_r7_2 = GetWindowWidth(&arg0->windows[1]);
    new_var = (temp_r7_2 * 8) - FontID_String_GetWidth(FONT_SYSTEM, temp_r6_2, 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[1], FONT_SYSTEM, temp_r6_2, new_var, 0U, 0xFFU, 0x10200U, 0L);
    CopyWindowToVram(&arg0->windows[1]);
    String_Delete(temp_r6_2);
    temp_r0_2 = *((u8 *)(((u8 *)arg1) + 8));
    if (temp_r0_2 == 0) {
        temp_r4 = NewString_ReadMsgData(arg0->msgData, 1);
        FillWindowPixelBuffer(&arg0->windows[4], 0U);
        AddTextPrinterParameterizedWithColor(&arg0->windows[4], FONT_SYSTEM, temp_r4, 0U, 0U, 0xFFU, 0x70800U, 0L);
        CopyWindowToVram(&arg0->windows[4]);
        String_Delete(temp_r4);
    } else if (temp_r0_2 == 1) {
        temp_r4_2 = NewString_ReadMsgData(arg0->msgData, 2);
        FillWindowPixelBuffer(&arg0->windows[4], 0U);
        AddTextPrinterParameterizedWithColor(&arg0->windows[4], FONT_SYSTEM, temp_r4_2, 0U, 0U, 0xFFU, 0x30400U, 0L);
        CopyWindowToVram(&arg0->windows[4]);
        String_Delete(temp_r4_2);
    } else {
        FillWindowPixelBuffer(&arg0->windows[4], 0U);
        CopyWindowToVram(&arg0->windows[4]);
    }
    temp_r4_3 = NewString_ReadMsgData(arg0->msgData, 4);
    FillWindowPixelBuffer(&arg0->windows[2], 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[2], FONT_SYSTEM, temp_r4_3, 0U, 0U, 0xFFU, 0x10200U, 0L);
    CopyWindowToVram(&arg0->windows[2]);
    String_Delete(temp_r4_3);
    BufferItemName(arg0->msgFormat, 0U, sp10);
    temp_r4_4 = ReadMsgData_ExpandPlaceholders(arg0->msgFormat, arg0->msgData, 5U, arg0->heapID);
    FillWindowPixelBuffer(&arg0->windows[5], 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[5], FONT_SYSTEM, temp_r4_4, 0U, 0U, 0xFFU, 0x10200U, 0L);
    CopyWindowToVram(&arg0->windows[5]);
    String_Delete(temp_r4_4);
}
