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

void ov51_021E6F18(TrainerCardMain *cardMain, Window *windows, TrainerCardMainCard *card);

void ov51_021E6F18(TrainerCardMain *cardMain, Window *windows, TrainerCardMainCard *card) {
    u8 i;
    MsgData *msgData;
    String *tmp;
    String *str;
    MessageFormat *msgFmt;
    void *val;
    u32 x;

    msgData = cardMain->msgData;
    for (i = 0; i < 7; i++) {
        FillWindowPixelBuffer(&windows[i], 0);
        if (i != 3 || (i == 3 && card->pokedexObtained)) {
            AddTextPrinterParameterizedWithColor(&windows[i], FONT_SYSTEM, cardMain->labelStrings[i], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        }
    }
    tmp = String_New(0x20, HEAP_ID_25);
    str = cardMain->formattedString;
    msgFmt = MessageFormat_New_Custom(6, 0x20, HEAP_ID_25);
    val = cardMain->numberString;
    ov51_021E74F4(&windows[0], 0x60, 0, 0, val, card->trainerId, 5, 2, 0xFF);
    CopyU16ArrayToString(cardMain->formattedString, card->name);
    ov51_021E7540(&windows[1], 0x68, 0, 0, cardMain->formattedString);
    BufferIntegerAsString(msgFmt, 5, card->money, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
    ReadMsgDataIntoString(msgData, 0x13, tmp);
    StringExpandPlaceholders(msgFmt, str, tmp);
    x = 0x88 - FontID_String_GetWidth(FONT_SYSTEM, str, 0);
    AddTextPrinterParameterizedWithColor(&windows[2], FONT_SYSTEM, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    if (card->pokedexObtained) {
        BufferIntegerAsString(msgFmt, 5, card->pokedexCount, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        ReadMsgDataIntoString(msgData, 0x1A, tmp);
        StringExpandPlaceholders(msgFmt, str, tmp);
        x = 0x88 - FontID_String_GetWidth(FONT_SYSTEM, str, 0);
        AddTextPrinterParameterizedWithColor(&windows[3], FONT_SYSTEM, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    ov51_021E74F4(&windows[4], 0x88, 0, 0, val, card->score, 9, 1, 0xFF);
    if (card->liveIgt) {
        BufferIntegerAsString(msgFmt, 0, GetIGTHours(card->igt), 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        BufferIntegerAsString(msgFmt, 1, GetIGTMinutes(card->igt), 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(msgData, 0x15, tmp);
    } else {
        BufferIntegerAsString(msgFmt, 0, card->playHours, 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        BufferIntegerAsString(msgFmt, 1, card->playMinutes, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(msgData, 0x14, tmp);
    }
    StringExpandPlaceholders(msgFmt, str, tmp);
    x = 0xE0 - FontID_String_GetWidth(FONT_SYSTEM, str, 0);
    AddTextPrinterParameterizedWithColor(&windows[5], FONT_SYSTEM, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    BufferIntegerAsString(msgFmt, 2, card->startYear, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferMonthNameAbbr(msgFmt, 3, card->startMonth);
    BufferIntegerAsString(msgFmt, 4, card->startDay, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ReadMsgDataIntoString(msgData, 0x16, tmp);
    StringExpandPlaceholders(msgFmt, str, tmp);
    x = 0xE0 - FontID_String_GetWidth(FONT_SYSTEM, str, 0);
    AddTextPrinterParameterizedWithColor(&windows[6], FONT_SYSTEM, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(tmp);
    MessageFormat_Delete(msgFmt);
}
