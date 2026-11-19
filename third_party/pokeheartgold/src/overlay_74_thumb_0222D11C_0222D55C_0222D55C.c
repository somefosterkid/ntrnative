#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"

typedef u32 TextColor;
BOOL ov74_0222D55C(WonderCardsAppData *appData, Window *window, TextColor color);
BOOL ov74_0222D55C(WonderCardsAppData *appData, Window *window, TextColor color) {
    RTCDate date;

    RTC_ConvertDayToDate(&date, appData->wonderCards[appData->selectedWondercardSlot]->receiveDate);

    BufferIntegerAsString(appData->strTemplate, 0, date.year + 2000, 4, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferMonthNameAbbr(appData->strTemplate, 1, date.month);
    BufferIntegerAsString(appData->strTemplate, 2, date.day, 2, PRINTING_MODE_LEADING_ZEROS, 1);

    return TRUE;
}
