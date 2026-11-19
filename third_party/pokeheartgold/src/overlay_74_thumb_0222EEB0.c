#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "overlay_74_wonder_cards_app2_data.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

u32 ov74_0222EEB0(WonderCardsApp2Data *appData);

s32 ov74_0222EF18(WonderCardsApp2Data *appData, Window *window, u32 color);

s32 ov74_0222EF64(void);

u32 ov74_0222EEB0(WonderCardsApp2Data *appData) {
    RTCDate date;

    RTC_ConvertDayToDate(&date, appData->wonderCard->receiveDate);
    BufferIntegerAsString(appData->msgFormat, 0, date.year + 2000, 4, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(appData->msgFormat, 1, date.month, 2, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(appData->msgFormat, 2, date.day, 2, PRINTING_MODE_LEADING_ZEROS, 1);
    return 1;
}

s32 ov74_0222EF18(WonderCardsApp2Data *appData, Window *window, u32 color) {
    String *string;

    string = String_New(0xFBU, HEAP_ID_85);
    CopyU16ArrayToStringN(string, appData->wonderCard->text, 0xFAU);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, 0U, 0U, 0xFFU, color, NULL);
    String_Delete(string);
    return 0;
}

s32 ov74_0222EF64(void) {
    return 0;
}
