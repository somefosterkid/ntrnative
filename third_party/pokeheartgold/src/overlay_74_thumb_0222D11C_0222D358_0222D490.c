#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "sprite_resources_header.h"
#include "text.h"

BOOL ov74_0222D490(OverlayManager *appMan, int *state);
BOOL ov74_0222D490(OverlayManager *appMan, int *state) {
    return TRUE;
}
typedef u32 TextColor;
BOOL ov74_0222D494(WonderCardsAppData *appData, Window *windwow, TextColor color);
BOOL ov74_0222D494(WonderCardsAppData *appData, Window *windwow, TextColor color) {
    String *string = String_New(36 + 1, appData->heapID);

    CopyU16ArrayToStringN(string, appData->wonderCards[appData->selectedWondercardSlot]->unk104.name, 36);
    AddTextPrinterParameterizedWithColor(windwow, FONT_MESSAGE, string, 0, 0, 0xFF, color, 0L);
    String_Delete(string);

    return 1;
}
