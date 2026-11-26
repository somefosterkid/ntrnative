#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "trainer_card_main.h"

void ov51_021E74F4(Window *, u32, u32, u32, String *, u32, u8, u32, u32);

void ov51_021E7208(TrainerCardMain *cardMain, Window *windows, TrainerCardMainCard *card);

void ov51_021E7208(TrainerCardMain *cardMain, Window *windows, TrainerCardMainCard *card) {
    u8 i;
    MsgData *msgData;
    String *fmtString;
    String *string;
    MessageFormat *messageFormat;
    u32 xOffset;

    msgData = cardMain->msgData;
    for (i = 7; i < 11; i++) {
        FillWindowPixelBuffer(&windows[i], 0);
        AddTextPrinterParameterizedWithColor(&windows[i], FONT_SYSTEM, cardMain->labelStrings[i], 0, 0, 0xff, 0x10200, NULL);
    }
    fmtString = String_New(32, HEAP_ID_25);
    string = cardMain->formattedString;
    messageFormat = MessageFormat_New_Custom(6, 32, HEAP_ID_25);
    if (card->clearMonth != 0) {
        BufferIntegerAsString(messageFormat, 2, card->clearYear, 2, PRINTING_MODE_LEADING_ZEROS, 1);
        BufferMonthNameAbbr(messageFormat, 3, card->clearMonth);
        BufferIntegerAsString(messageFormat, 4, card->clearDay, 2, PRINTING_MODE_LEADING_ZEROS, 1);
        BufferIntegerAsString(messageFormat, 0, card->clearHours, 3, PRINTING_MODE_RIGHT_ALIGN, 1);
        BufferIntegerAsString(messageFormat, 1, card->clearMinutes, 2, PRINTING_MODE_LEADING_ZEROS, 1);
        ReadMsgDataIntoString(msgData, 0x16, fmtString);
        StringExpandPlaceholders(messageFormat, string, fmtString);
    } else {
        ReadMsgDataIntoString(msgData, 0xc, fmtString);
        BufferString(messageFormat, 0, fmtString, 0, 0, 2);
        BufferString(messageFormat, 1, fmtString, 0, 0, 2);
        ReadMsgDataIntoString(msgData, 0x19, string);
    }
    xOffset = 224 - FontID_String_GetWidth(FONT_SYSTEM, string, 0);
    AddTextPrinterParameterizedWithColor(&windows[7], FONT_SYSTEM, string, xOffset, 0, 0xff, 0x10200, NULL);
    ReadMsgDataIntoString(msgData, 0x14, fmtString);
    StringExpandPlaceholders(messageFormat, string, fmtString);
    xOffset = 224 - FontID_String_GetWidth(FONT_SYSTEM, string, 0);
    AddTextPrinterParameterizedWithColor(&windows[7], FONT_SYSTEM, string, xOffset, 16, 0xff, 0x10200, NULL);

    BufferIntegerAsString(messageFormat, 5, card->linkCount, 6, PRINTING_MODE_LEFT_ALIGN, 1);
    ReadMsgDataIntoString(msgData, 0x1b, fmtString);
    StringExpandPlaceholders(messageFormat, string, fmtString);
    xOffset = 224 - FontID_String_GetWidth(FONT_SYSTEM, string, 0);
    AddTextPrinterParameterizedWithColor(&windows[8], FONT_SYSTEM, string, xOffset, 0, 0xff, 0x10200, NULL);

    ReadMsgDataIntoString(msgData, 0x17, string);
    AddTextPrinterParameterizedWithColor(&windows[9], FONT_SYSTEM, string, 0x70, 0, 0xff, 0x10200, NULL);
    ov51_021E74F4(&windows[9], 224, 0, 0, string, card->linkBattleLosses, 4, 1, 0xff);
    ReadMsgDataIntoString(msgData, 0x18, string);
    AddTextPrinterParameterizedWithColor(&windows[9], FONT_SYSTEM, string, 0xb0, 0, 0xff, 0x10200, NULL);
    ov51_021E74F4(&windows[9], 224, 0x40, 0, string, card->linkBattleWins, 4, 1, 0xff);

    BufferIntegerAsString(messageFormat, 5, card->linkTrades, 6, PRINTING_MODE_LEFT_ALIGN, 1);
    ReadMsgDataIntoString(msgData, 0x1b, fmtString);
    StringExpandPlaceholders(messageFormat, string, fmtString);
    xOffset = 224 - FontID_String_GetWidth(FONT_SYSTEM, string, 0);
    AddTextPrinterParameterizedWithColor(&windows[10], FONT_SYSTEM, string, xOffset, 0, 0xff, 0x10200, NULL);
    String_Delete(fmtString);
    MessageFormat_Delete(messageFormat);
}
