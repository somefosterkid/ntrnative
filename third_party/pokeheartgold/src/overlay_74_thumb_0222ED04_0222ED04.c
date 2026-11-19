#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "msgdata.h"
#include "overlay_74_wonder_cards_app2_data.h"

extern const ListMenuTemplate ov74_0223C6E0;

void ov74_0222ED04(WonderCardsApp2Data *appData, const WonderCardsApp2MenuOption *options, int count, Window *window, u32 itemsAbove);

void ov74_0222ED04(WonderCardsApp2Data *appData, const WonderCardsApp2MenuOption *options, int count, Window *window, u32 itemsAbove) {
    ListMenuTemplate template;
    int i;

    if (appData->strList != NULL) {
        ListMenuItems_Delete(appData->strList);
    }
    if (appData->listMenu != NULL) {
        DestroyListMenu(appData->listMenu, NULL, NULL);
    }
    appData->strList = ListMenuItems_New(count, HEAP_ID_85);
    appData->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xF7, HEAP_ID_85);
    for (i = 0; i < count; i++) {
        ListMenuItems_AppendFromMsgData(appData->strList, appData->msgData, options[i].textEntryID, options[i].value);
    }
    DestroyMsgData(appData->msgData);
    template = ov74_0223C6E0;
    template.items = appData->strList;
    template.totalItems = count;
    template.window = window;
    appData->listMenu = ListMenuInit(&template, 0, itemsAbove, HEAP_ID_85);
}
