#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "igt.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "trainer_card_main.h"

void ov51_021E74F4(Window *window, u32 x, u32 y, u32 a3, String *a4, s32 num, u8 digits, u32 mode, u32 a8);

void ov51_021E7540(Window *window, u32 x, u32 y, u32 a3, String *str);

void ov51_021E6F18(u8 *data, Window *windows, u8 *card);

void ov51_021E6F18(u8 *data, Window *windows, u8 *card) {
    u8 i;
    MsgData *msgData;
    String *tmp;
    String *str;
    MessageFormat *msgFmt;
    void *val;
    u32 x;

    msgData = ((TrainerCardMain *)data)->msgData;
    for (i = 0; i < 7; i++) {
        FillWindowPixelBuffer(&windows[i], 0);
        if (i != 3 || (i == 3 && (u32)(card[4] << 28) >> 31)) {
            AddTextPrinterParameterizedWithColor(&windows[i], FONT_SYSTEM, ((TrainerCardMain *)data)->labelStrings[i], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        }
    }
    tmp = String_New(0x20, HEAP_ID_25);
    str = ((TrainerCardMain *)data)->formattedString;
    msgFmt = MessageFormat_New_Custom(6, 0x20, HEAP_ID_25);
    val = *((void **)&((TrainerCardMain *)data)->numberString);
    ov51_021E74F4(&windows[0], 0x60, 0, 0, val, *(u16 *)(card + 0x28), 5, 2, 0xFF);
    CopyU16ArrayToString(((TrainerCardMain *)data)->formattedString, (u16 *)(card + 8));
    ov51_021E7540(&windows[1], 0x68, 0, 0, ((TrainerCardMain *)data)->formattedString);
    BufferIntegerAsString(msgFmt, 5, *(s32 *)(card + 0x1C), 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
    ReadMsgDataIntoString(msgData, 0x13, tmp);
    StringExpandPlaceholders(msgFmt, str, tmp);
    x = 0x88 - FontID_String_GetWidth(FONT_SYSTEM, str, 0);
    AddTextPrinterParameterizedWithColor(&windows[2], FONT_SYSTEM, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    if ((u32)(card[4] << 28) >> 31) {
        BufferIntegerAsString(msgFmt, 5, *(s32 *)(card + 0x20), 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        ReadMsgDataIntoString(msgData, 0x1A, tmp);
        StringExpandPlaceholders(msgFmt, str, tmp);
        x = 0x88 - FontID_String_GetWidth(FONT_SYSTEM, str, 0);
        AddTextPrinterParameterizedWithColor(&windows[3], FONT_SYSTEM, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    ov51_021E74F4(&windows[4], 0x88, 0, 0, val, *(s32 *)(card + 0x24), 9, 1, 0xFF);
    if ((u32)(card[4] << 30) >> 31) {
        BufferIntegerAsString(msgFmt, 0, GetIGTHours(*(IGT **)(card + 0x18)), 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        BufferIntegerAsString(msgFmt, 1, GetIGTMinutes(*(IGT **)(card + 0x18)), 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(msgData, 0x15, tmp);
    } else {
        BufferIntegerAsString(msgFmt, 0, *(u16 *)(card + 0x2A), 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        BufferIntegerAsString(msgFmt, 1, card[0x2E], 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(msgData, 0x14, tmp);
    }
    StringExpandPlaceholders(msgFmt, str, tmp);
    x = 0xE0 - FontID_String_GetWidth(FONT_SYSTEM, str, 0);
    AddTextPrinterParameterizedWithColor(&windows[5], FONT_SYSTEM, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    BufferIntegerAsString(msgFmt, 2, card[0x2F], 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferMonthNameAbbr(msgFmt, 3, card[0x30]);
    BufferIntegerAsString(msgFmt, 4, card[0x31], 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ReadMsgDataIntoString(msgData, 0x16, tmp);
    StringExpandPlaceholders(msgFmt, str, tmp);
    x = 0xE0 - FontID_String_GetWidth(FONT_SYSTEM, str, 0);
    AddTextPrinterParameterizedWithColor(&windows[6], FONT_SYSTEM, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(tmp);
    MessageFormat_Delete(msgFmt);
}
