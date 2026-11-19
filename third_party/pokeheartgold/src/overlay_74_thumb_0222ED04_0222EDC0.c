#include "global.h"

#include "bg_window.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "overlay_74_wonder_cards_app2_data.h"

s32 ov74_0222ECD4(Window *window, u8 skipTransfer);

void ov74_0222EDC0(WonderCardsApp2Data *appData);

void ov74_0222EDC0(WonderCardsApp2Data *appData) {
    ListMenuItems_Delete(appData->strList);
    appData->strList = NULL;
    DestroyListMenu(appData->listMenu, NULL, NULL);
    appData->listMenu = NULL;
    ov74_0222ECD4(&appData->standardWindow, 0);
    ClearWindowTilemapAndCopyToVram(&appData->standardWindow);
    RemoveWindow(&appData->standardWindow);
}

s32 ov74_0222EE08(void);

s32 ov74_0222EE08(void) {
    return 1;
}
