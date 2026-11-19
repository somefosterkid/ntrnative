#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "error_handling.h"
#include "save.h"
#include "sprite_resources_header.h"
#include "unk_02005D10.h"
#include "constants/save_arrays.h"
#include "overlay_74_main_menu_util.h"

enum MainMenuUtilSavingStatus ov74_022360B0(void);
static void ov74_0222E03C(WonderCardsAppData *appData);
static void ov74_0222E03C(WonderCardsAppData *appData) {
    int saveStatus = ov74_022360B0();

    if (saveStatus == MAIN_MENU_UTIL_SAVING_OK || saveStatus == MAIN_MENU_UTIL_SAVING_CORRUPT) {
        PlaySE(1562);
        appData->mainCallback = NULL;
    }
}
void ov74_022360A0(SaveData *saveData);
void ov74_0222E060(WonderCardsAppData *appData);
void ov74_0222E060(WonderCardsAppData *appData) {
    ((appData->selectedWondercardSlot < 3) ? (void)0 : GF_AssertFail());
    SaveSubstruct_AssertCRC(SAVE_MYSTERY_GIFT);

    if (appData->wonderCards[appData->selectedWondercardSlot]->shareCount != 0xff) {
        appData->wonderCards[appData->selectedWondercardSlot]->shareCount++;
    }

    if (appData->wonderCards[appData->selectedWondercardSlot]->shareMax != 0xff) {
        appData->wonderCards[appData->selectedWondercardSlot]->shareMax--;
    }

    SaveSubstruct_UpdateCRC(SAVE_MYSTERY_GIFT);
    ov74_022360A0(appData->saveData);

    appData->mainCallback = ov74_0222E03C;
}
