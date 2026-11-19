#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "overlay_74_wonder_cards_app2_data.h"

void ov74_0222F024(WonderCardsApp2Data *appData, BOOL unused, int screen);

void ov74_0222FCC4(BgConfig *bgConfig);

void ov74_0222FC50(BgConfig *bgConfig, WonderCard *wonderCard, enum HeapID heapID);

void ov74_0222FC50(BgConfig *bgConfig, WonderCard *wonderCard, enum HeapID heapID) {
    WonderCardsApp2Data *appData;

    ov74_0222FCC4(bgConfig);
    appData = Heap_AllocAtEnd(heapID, sizeof(WonderCardsApp2Data));
    memset(appData, 0, sizeof(WonderCardsApp2Data));
    appData->bgConfig = bgConfig;
    appData->heapID = heapID;
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, appData->heapID);
    appData->wonderCard = wonderCard;
    ov74_0222F024(appData, TRUE, 0);
    Heap_Free(appData);
}
