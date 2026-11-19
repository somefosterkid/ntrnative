#include "global.h"

#include "constants/heap.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0247.h"

#include "filesystem_files_def.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "msgdata.h"
#include "overlay_74_wonder_cards_app_data.h"

extern const ListMenuTemplate ov74_0223C320;

void ov74_0222D11C(WonderCardsAppData *appData, StateTransitionMenuOptionTemplate *entries, int numEntries, Window *window, int startCursorPos);

void ov74_0222D11C(WonderCardsAppData *appData, StateTransitionMenuOptionTemplate *entries, int numEntries, Window *window, int startCursorPos) {
    ListMenuTemplate template;
    int i;

    if (appData->strList != NULL) {
        ListMenuItems_Delete(appData->strList);
    }
    if (appData->listMenu != NULL) {
        DestroyListMenu(appData->listMenu, NULL, NULL);
    }
    appData->strList = ListMenuItems_New(numEntries, HEAP_ID_85);
    appData->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msg_0247, HEAP_ID_85);
    for (i = 0; i < numEntries; i++) {
        ListMenuItems_AppendFromMsgData(appData->strList, appData->msgLoader, entries->entryID, entries->value);
        entries++;
    }

    DestroyMsgData(appData->msgLoader);
    template = ov74_0223C320;
    template.items = appData->strList;
    template.totalItems = numEntries;
    template.window = window;
    appData->listMenu = ListMenuInit(&template, 0, startCursorPos, HEAP_ID_85);
}
