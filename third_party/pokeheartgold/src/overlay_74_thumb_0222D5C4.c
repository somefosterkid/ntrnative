#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_74_wonder_cards_app_data.h"

BOOL ov74_0222D5C4(WonderCardsAppData *appData, Window *window, u32 color);

BOOL ov74_0222D614(WonderCardsAppData *appData, Window *window, u32 color);

BOOL ov74_0222D5C4(WonderCardsAppData *appData, Window *window, u32 color) {
    String *string;

    string = String_New(0xFBU, HEAP_ID_85);
    CopyU16ArrayToStringN(string, appData->wonderCards[appData->selectedWondercardSlot]->text, 0xFAU);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, 0U, 0U, 0xFFU, color, NULL);
    String_Delete(string);
    return 0;
}

BOOL ov74_0222D614(WonderCardsAppData *appData, Window *window, u32 color) {
    String *string;
    u8 num;

    num = appData->wonderCards[appData->selectedWondercardSlot]->shareMax;
    if (num == 0xFF) {
        string = ReadMsgData_ExpandPlaceholders(appData->strTemplate, appData->msgLoader, 0x34U, HEAP_ID_85);
        AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, 0U, 0U, 0xFFU, color, NULL);
        String_Delete(string);
        return 0;
    }
    if (num != 0) {
        BufferIntegerAsString(appData->strTemplate, 0U, (s32)num, 3U, PRINTING_MODE_LEFT_ALIGN, 1);
        return 1;
    }
    return 0;
}
