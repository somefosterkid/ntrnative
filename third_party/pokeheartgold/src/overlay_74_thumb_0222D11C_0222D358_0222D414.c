#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0247.h"

#include "overlay_74_wonder_cards_app_data.h"
#include "overlay_manager.h"

void ov74_0222D1D4(Window *window, int textEntryID);
void ov74_0222D448(WonderCardsAppData *appData);
int ov74_0222D9E0(WonderCardsAppData *appData, Window *window, int windowTemplateIdx, int baseTile);
enum WonderCardsAppState ov74_0222D414(OverlayManager *man);

enum WonderCardsAppState ov74_0222D414(OverlayManager *man) {
    WonderCardsAppData *appData = OverlayManager_GetData(man);

    ov74_0222D448(appData);
    ov74_0222D1D4(&appData->messageBox, msg_0247_00037);
    ov74_0222D9E0(appData, &appData->standardWindow, MSG_BOX_WC_ACTIONS_MENU, appData->nextWindowBasetile);
    return WC_APP_STATE_WAIT_FOR_MENU_CHOICE;
}
