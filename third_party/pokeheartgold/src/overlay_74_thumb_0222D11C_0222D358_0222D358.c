#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"

void ov74_0222D448(WonderCardsAppData *appData);
int ov74_0222D9E0(WonderCardsAppData *appData, Window *window, int windowTemplateIdx, int baseTile);
int ov74_0222DAF8(WonderCardsAppData *appData, int currentSlot, int direction);
void ov74_0222D824(WonderCardsAppData *appData, BOOL unused, enum WonderCardsAppScreen screen);
void ov74_0222D1D4(Window *window, int textEntryID);
enum WonderCardsAppState ov74_0222D358(OverlayManager *appMan);
enum WonderCardsAppState ov74_0222D358(OverlayManager *appMan) {
    WonderCardsAppData *appData = OverlayManager_GetData(appMan);

    ov74_0222D448(appData);
    ov74_0222D1D4(&appData->messageBox, 64);

    appData->waitDial = WaitingIcon_New(&appData->messageBox, ((1 + (3 * 3)) + (3 * 3)));

    if (SaveMysteryGift_HasAnyGift(appData->mysteryGift, appData->selectedWondercardSlot) == 1) {
        SaveMysteryGift_ReceiveGiftAndClearCardByIndex(appData->mysteryGift, appData->selectedWondercardSlot);
    } else {
        SaveMysteryGift_DeleteWonderCardByIndex(appData->mysteryGift, appData->selectedWondercardSlot);
    }

    SaveGameNormal(appData->saveData);
    sub_0200F450(appData->waitDial);

    if (SaveMysteryGift_HasAnyCard(appData->mysteryGift) == 0) {
        return (enum WonderCardsAppState)27;
    }

    appData->selectedWondercardSlot = ov74_0222DAF8(appData, appData->selectedWondercardSlot, 1);

    ov74_0222D1D4(&appData->messageBox, 37);
    ov74_0222D9E0(appData, &appData->standardWindow, MSG_BOX_WC_ACTIONS_MENU, appData->nextWindowBasetile);
    ov74_0222D824(appData, 1, WC_SCREEN_WONDERCARD_FRONT);

    return WC_APP_STATE_WAIT_FOR_MENU_CHOICE;
}
