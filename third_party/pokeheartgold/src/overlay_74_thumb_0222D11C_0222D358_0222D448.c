#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"

void ov74_0222D0EC(Window *window, u8 skipTransfer);
void ov74_0222D448(WonderCardsAppData *appData);
void ov74_0222D448(WonderCardsAppData *appData) {
    ListMenuItems_Delete(appData->strList);
    appData->strList = 0L;
    DestroyListMenu(appData->listMenu, 0L, 0L);
    appData->listMenu = 0L;
    ov74_0222D0EC(&appData->standardWindow, 0);
    ClearWindowTilemapAndCopyToVram(&appData->standardWindow);
    RemoveWindow(&appData->standardWindow);
}
