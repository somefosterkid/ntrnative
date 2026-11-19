#include "global.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_97_course_sub_screen.h"

void ov97_0221F294(Ov97CourseSubScreen *screen);

void ov97_0221F294(Ov97CourseSubScreen *screen) {
    String *temp_r0;
    String *temp_r0_2;
    String *temp_r0_3;
    String *temp_r0_4;
    String *temp_r0_5;
    String *temp_r0_6;
    String *temp_r5;

    BufferPokeathlonCourseName(screen->msgFormat, 0U, (u32) (u8) screen->courseId);
    temp_r5 = ReadMsgData_ExpandPlaceholders(screen->msgFormat, screen->msgData, 0U, screen->heapID);
    FillWindowPixelBuffer(&screen->windows[0], 0U);
    AddTextPrinterParameterizedWithColor(&screen->windows[0], FONT_SYSTEM, temp_r5, 0U, 0U, 0xFFU, 0x10200U, NULL);
    CopyWindowToVram(&screen->windows[0]);
    String_Delete(temp_r5);
    FillWindowPixelBuffer(&screen->windows[1], 0U);
    if (screen->unk_6A != 0) {
        temp_r0 = NewString_ReadMsgData(screen->msgData, 1);
        AddTextPrinterParameterizedWithColor(&screen->windows[1], FONT_SYSTEM, temp_r0, 0U, 0U, 0xFFU, 0x10200U, NULL);
        String_Delete(temp_r0);
    }
    CopyWindowToVram(&screen->windows[1]);
    FillWindowPixelBuffer(&screen->windows[4], 0U);
    temp_r0_2 = NewString_ReadMsgData(screen->msgData, 8);
    AddTextPrinterParameterizedWithColor(&screen->windows[4], FONT_SYSTEM, temp_r0_2, 0U, 0U, 0xFFU, 0x10200U, NULL);
    String_Delete(temp_r0_2);
    temp_r0_3 = NewString_ReadMsgData(screen->msgData, 4);
    AddTextPrinterParameterizedWithColor(&screen->windows[4], FONT_SYSTEM, temp_r0_3, 0U, 0x10U, 0xFFU, 0x10200U, NULL);
    String_Delete(temp_r0_3);
    temp_r0_4 = NewString_ReadMsgData(screen->msgData, 6);
    AddTextPrinterParameterizedWithColor(&screen->windows[4], FONT_SYSTEM, temp_r0_4, 0U, 0x20U, 0xFFU, 0x10200U, NULL);
    String_Delete(temp_r0_4);
    temp_r0_5 = NewString_ReadMsgData(screen->msgData, 5);
    AddTextPrinterParameterizedWithColor(&screen->windows[4], FONT_SYSTEM, temp_r0_5, 0U, 0x30U, 0xFFU, 0x10200U, NULL);
    String_Delete(temp_r0_5);
    temp_r0_6 = NewString_ReadMsgData(screen->msgData, 7);
    AddTextPrinterParameterizedWithColor(&screen->windows[4], FONT_SYSTEM, temp_r0_6, 0U, 0x40U, 0xFFU, 0x10200U, NULL);
    String_Delete(temp_r0_6);
    CopyWindowToVram(&screen->windows[4]);
}

void ov97_0221F428(Ov97CourseSubScreen *screen, void *arg1);

void ov97_0221F428(Ov97CourseSubScreen *screen, void *arg1) {
    String *temp_r0_2;
    String *temp_r4;
    String *temp_r6;
    String *temp_r6_2;
    u8 temp_r0;

    CopyU16ArrayToString(screen->string, arg1 + 0xA);
    FillWindowPixelBuffer(&screen->windows[2], 0U);
    AddTextPrinterParameterizedWithColor(&screen->windows[2], FONT_SYSTEM, screen->string, 0U, 0U, 0xFFU, 0x10200U, NULL);
    CopyWindowToVram(&screen->windows[2]);
    temp_r0 = *(u8 *)((u8 *)arg1 + 8);
    if (temp_r0 == 0) {
        temp_r6 = NewString_ReadMsgData(screen->msgData, 2);
        FillWindowPixelBuffer(&screen->windows[3], 0U);
        AddTextPrinterParameterizedWithColor(&screen->windows[3], FONT_SYSTEM, temp_r6, 4U, 0U, 0xFFU, 0x70800U, NULL);
        CopyWindowToVram(&screen->windows[3]);
        String_Delete(temp_r6);
    } else if (temp_r0 == 1) {
        temp_r6_2 = NewString_ReadMsgData(screen->msgData, 3);
        FillWindowPixelBuffer(&screen->windows[3], 0U);
        AddTextPrinterParameterizedWithColor(&screen->windows[3], FONT_SYSTEM, temp_r6_2, 4U, 0U, 0xFFU, 0x30400U, NULL);
        CopyWindowToVram(&screen->windows[3]);
        String_Delete(temp_r6_2);
    } else {
        FillWindowPixelBuffer(&screen->windows[3], 0U);
        CopyWindowToVram(&screen->windows[3]);
    }
    temp_r0_2 = GetSpeciesName(*(u16 *)((u8 *)arg1 + 4), screen->heapID);
    BufferString(screen->msgFormat, 0U, temp_r0_2, 2, 1, 2);
    String_Delete(temp_r0_2);
    temp_r4 = ReadMsgData_ExpandPlaceholders(screen->msgFormat, screen->msgData, 9U, screen->heapID);
    FillWindowPixelBuffer(&screen->windows[5], 0U);
    AddTextPrinterParameterizedWithColor(&screen->windows[5], FONT_SYSTEM, temp_r4, 0U, 0U, 0xFFU, 0x10200U, NULL);
    CopyWindowToVram(&screen->windows[5]);
    String_Delete(temp_r4);
}
