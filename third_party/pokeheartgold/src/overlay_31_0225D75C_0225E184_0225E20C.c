#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_03.h"
#include "overlay_31.h"
#include "pm_string.h"
#include "text.h"

void ov31_0225E20C(MartSubScreen *arg0, u16 unused) {
    MartData *data;
    String *string;
    String *v0;
    String *v1;

    string = NewString_ReadMsgData(arg0->msgData, 0x23);
    AddTextPrinterParameterizedWithColor(&arg0->windows[MART_SUBSCREEN_WIN_OWNED], FONT_SYSTEM, string, 0U, 4U, 0xFFU, 0x10200U, NULL);
    String_Delete(string);
    v0 = String_New(5U, HEAP_ID_FIELD2);
    v1 = NewString_ReadMsgData(arg0->msgData, 0x24);
    data = arg0->martData;
    BufferIntegerAsString(arg0->messageFormat, 0U, ov03_02257978(data, (s32)data->item), 3U, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(arg0->messageFormat, v0, v1);
    AddTextPrinterParameterizedWithColor(&arg0->windows[MART_SUBSCREEN_WIN_OWNED], FONT_SYSTEM, v0, 0x40 - FontID_String_GetWidth(FONT_SYSTEM, v0, 0U), 0x14U, 0xFFU, 0x10200U, NULL);
    String_Delete(v0);
    String_Delete(v1);
    ScheduleWindowCopyToVram(&arg0->windows[MART_SUBSCREEN_WIN_OWNED]);
}

void ov31_0225E2D4(MartSubScreen *arg0, s32 arg1) {
    String *string;
    String *src;
    String *v0;
    String *v1;
    String *v2;
    String *v3;
    String *v4;
    String *v5;

    v2 = String_New(2U, HEAP_ID_FIELD2);
    v4 = String_New(2U, HEAP_ID_FIELD2);
    v0 = NewString_ReadMsgData(arg0->msgData, 0x2C);
    src = v0;
    v1 = NewString_ReadMsgData(arg0->msgData, 0x2D);
    string = v1;
    BufferIntegerAsString(arg0->messageFormat, 0U, arg1 / 10, 1U, PRINTING_MODE_RIGHT_ALIGN, 1);
    BufferIntegerAsString(arg0->messageFormat, 1U, arg1 % 10, 1U, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(arg0->messageFormat, v2, src);
    StringExpandPlaceholders(arg0->messageFormat, v4, string);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_UNK_7], 0U);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_UNK_8], 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[MART_SUBSCREEN_WIN_UNK_7], FONT_SYSTEM, v2, 0U, 4U, 0xFFU, 0x10200U, NULL);
    AddTextPrinterParameterizedWithColor(&arg0->windows[MART_SUBSCREEN_WIN_UNK_8], FONT_SYSTEM, v4, 0U, 4U, 0xFFU, 0x10200U, NULL);
    String_Delete(v2);
    String_Delete(v4);
    String_Delete(v0);
    String_Delete(v1);
    ScheduleWindowCopyToVram(&arg0->windows[MART_SUBSCREEN_WIN_UNK_7]);
    ScheduleWindowCopyToVram(&arg0->windows[MART_SUBSCREEN_WIN_UNK_8]);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_TOTAL], 0U);
    v3 = String_New(9U, HEAP_ID_FIELD2);
    v5 = NewString_ReadMsgData(arg0->msgData, 0x26);
    BufferIntegerAsString(arg0->messageFormat, 0U, arg0->martData->cost * arg1, 6U, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(arg0->messageFormat, v3, v5);
    AddTextPrinterParameterizedWithColor(&arg0->windows[MART_SUBSCREEN_WIN_TOTAL], FONT_SYSTEM, v3, 0x40 - FontID_String_GetWidth(FONT_SYSTEM, v3, 0U), 4U, 0xFFU, 0x10200U, NULL);
    String_Delete(v3);
    String_Delete(v5);
    ScheduleWindowCopyToVram(&arg0->windows[MART_SUBSCREEN_WIN_TOTAL]);
}

void ov31_0225E474(MartSubScreen *arg0) {
    String *string;

    string = NewString_ReadMsgData(arg0->msgData, 0x2A);
    AddTextPrinterParameterizedWithColor(&arg0->windows[MART_SUBSCREEN_WIN_UNK_9], FONT_SYSTEM, string, 4U, 0U, 0xFFU, 0xF0E00U, NULL);
    ScheduleWindowCopyToVram(&arg0->windows[MART_SUBSCREEN_WIN_UNK_9]);
    String_Delete(string);
}

void ov31_0225E4BC(u32 arg0, MessageFormat *arg1, u32 arg2, u32 arg3);

void ov31_0225E4BC(u32 arg0, MessageFormat *messageFormat, u32 arg2, u32 fieldno) {
    switch (arg0) {
    case 0:
    case 3:
    case 4:
        BufferItemName(messageFormat, fieldno, arg2);
        return;
    case 1:
        BufferDecorationName(messageFormat, fieldno, arg2);
    default:
        return;
    }
}

void ov31_0225E4EC(u32 arg0, MessageFormat *messageFormat, u32 arg2, u32 fieldno) {
    switch (arg0) {
    case 0:
    case 3:
    case 4:
        BufferItemNamePlural(messageFormat, fieldno, arg2);
        return;
    case 1:
        BufferDecorationName(messageFormat, fieldno, arg2);
    default:
        return;
    }
}

void ov31_0225E51C(u32 arg0, MessageFormat *messageFormat, u32 arg2, u32 fieldno) {
    switch (arg0) {
    case 0:
    case 3:
    case 4:
        BufferItemNameWithIndefArticle(messageFormat, fieldno, arg2);
        return;
    case 1:
        BufferDecorationName(messageFormat, fieldno, arg2);
    default:
        return;
    }
}
