#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "mystery_gift.h"
#include "overlay_74_wonder_cards_app_data.h"

void ov74_0222D824(WonderCardsAppData *appData, BOOL unused, enum WonderCardsAppScreen screen);

void ov74_0222FCC4(BgConfig *);

void ov74_0222EC08(BgConfig *bgConfig, WonderCard *wonderCard, enum HeapID heapID);

void ov74_0222EC08(BgConfig *bgConfig, WonderCard *wonderCard, enum HeapID heapID) {
    WonderCardsAppData *appData;
    ov74_0222FCC4(bgConfig);
    appData = Heap_AllocAtEnd(heapID, sizeof(WonderCardsAppData));
    memset(appData, 0, sizeof(WonderCardsAppData));
    appData->bgConfig = bgConfig;
    appData->heapID = heapID;
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, appData->heapID);
    appData->wonderCards[0] = wonderCard;
    appData->selectedWondercardSlot = 0;
    ov74_0222D824(appData, TRUE, WC_SCREEN_WONDERCARD_FRONT);
    Heap_Free(appData);
}
