#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "overlay_74_wonder_cards_app2_data.h"
#include "pm_string.h"
#include "text.h"

s32 ov74_0222EE0C(WonderCardsApp2Data *appData, Window *window, u32 color);

s32 ov74_0222EE58(WonderCardsApp2Data *appData, Window *window, u32 color);

s32 ov74_0222EE0C(WonderCardsApp2Data *appData, Window *window, u32 color) {
    String *string;

    string = String_New(0x25U, appData->heapID);
    CopyU16ArrayToStringN(string, appData->wonderCard->unk104.name, 0x24U);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, 0U, 0U, 0xFFU, color, NULL);
    String_Delete(string);
    return 1;
}

s32 ov74_0222EE58(WonderCardsApp2Data *appData, Window *window, u32 color) {
    String *string;
    u32 msgno;

    if (appData->wonderCard->shareCount == 0) {
        msgno = 0x51;
    } else {
        msgno = 0x52;
    }
    string = ReadMsgData_ExpandPlaceholders(appData->msgFormat, appData->msgData, msgno, appData->heapID);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, 0U, 0U, 0xFFU, color, NULL);
    String_Delete(string);
    return 1;
}
