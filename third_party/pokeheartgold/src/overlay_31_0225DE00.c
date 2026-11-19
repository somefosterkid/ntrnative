#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_31.h"
#include "pm_string.h"
#include "text.h"
#include "pokeathlon/pokeathlon_save.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"




u8 ov31_0225DE00(MartSubScreen *param_1, Window *param_2, String *param_3, s32 unused) {
    u8 uVar1;

    uVar1 = AddTextPrinterParameterizedWithColor(param_2, FONT_SYSTEM, param_3, 0, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    return uVar1;
}

void ov31_0225DE24(MessageFormat *messageFormat, MsgData *msgData, Window *window, s32 num, s32 arg4) {
    String *string;
    u32 msgno;

    msgno = 0x12;
    if ((u32)(arg4 - 3) <= 1U) {
        msgno = 0x13;
    }
    BufferIntegerAsString(messageFormat, 0U, num, 4U, PRINTING_MODE_LEFT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, msgno, HEAP_ID_8);
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 0x24U, 0x10U, 0xFFU, 0x10200U, NULL);
    String_Delete(string);
}

void ov31_0225DE84(MartSubScreen *arg0) {
    String *string;
    String *v0;
    String *v1;
    u32 num;

    FillWindowPixelBuffer(&arg0->moneyWindow, 0U);
    if ((u32)(u8)(arg0->martData->martType + 0xFD) <= 1U) {
        v0 = NewString_ReadMsgData(arg0->msgData, 0x21);
    } else {
        v0 = NewString_ReadMsgData(arg0->msgData, 0x1E);
    }
    AddTextPrinterParameterizedWithColor(&arg0->moneyWindow, FONT_SYSTEM, v0, 0U, 0U, 0xFFU, 0xF0E00U, NULL);
    String_Delete(v0);
    string = String_New(0x10U, HEAP_ID_FIELD2);
    if ((u32)(u8)(arg0->martData->martType + 0xFD) <= 1U) {
        v1 = NewString_ReadMsgData(arg0->msgData, 0x20);
        num = PokeathlonSave_GetAthletePoints(arg0->pokeathlonSave);
    } else {
        v1 = NewString_ReadMsgData(arg0->msgData, 0x1F);
        num = PlayerProfile_GetMoney(arg0->playerProfile);
    }
    BufferIntegerAsString(arg0->messageFormat, 0U, (s32)num, 6U, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(arg0->messageFormat, string, v1);
    AddTextPrinterParameterizedWithColor(&arg0->moneyWindow, FONT_SYSTEM, string, 0x48 - FontID_String_GetWidth(FONT_SYSTEM, string, 0U), 0x10U, 0xFFU, 0xF0E00U, NULL);
    String_Delete(v1);
    String_Delete(string);
    ScheduleWindowCopyToVram(&arg0->moneyWindow);
}

void ov31_0225DF98(MartSubScreen *arg0) {
    s32 num;
    s32 v0;
    String *string;
    String *v3;
    s32 v4;
    u8 v1;
    MartData *v2;

    FillWindowPixelBuffer(&arg0->pageWindow, 0U);
    string = String_New(6U, HEAP_ID_FIELD2);
    v3 = NewString_ReadMsgData(arg0->msgData, 0x2B);
    v2 = arg0->martData;
    num = (s32)(v2->pageOffset + 6) / 6;
    v1 = v2->count;
    v0 = (s32)v1;
    v4 = (s32)v1 / 6;
    if (((s32)v1 % 6) != 0) {
        v4 += 1;
    }
    BufferIntegerAsString(arg0->messageFormat, 0U, num, 2U, PRINTING_MODE_RIGHT_ALIGN, 1);
    BufferIntegerAsString(arg0->messageFormat, 1U, v4, 2U, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(arg0->messageFormat, string, v3);
    AddTextPrinterParameterizedWithColor(&arg0->pageWindow, FONT_SYSTEM, string, 0U, 0U, 0xFFU, 0xF0E00U, NULL);
    String_Delete(v3);
    String_Delete(string);
    ScheduleWindowCopyToVram(&arg0->pageWindow);
}
