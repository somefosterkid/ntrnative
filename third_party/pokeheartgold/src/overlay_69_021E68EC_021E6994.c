#include "global.h"

#include "bg_window.h"
#include "geonet_globe.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"

void ov69_021E68D8(ListMenu *listMenu, s32 unused, u8 mute);

void ov69_021E6994(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *listMenuTemplate, u32 bankId, const u8 *itemIds, u32 itemCount);

void ov69_021E6994(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *listMenuTemplate, u32 bankId, const u8 *itemIds, u32 itemCount) {
    ListMenuTemplate template;
    MsgData *loader;

    AddWindow(globe->bgConfig, window, windowTemplate);
    loader = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, bankId, globe->heapID);
    globe->menuChoices = ListMenuItems_New(itemCount, globe->heapID);

    for (int i = 0; i < itemCount; i++) {
        ListMenuItems_AppendFromMsgData(globe->menuChoices, loader, itemIds[i], i);
    }

    DestroyMsgData(loader);

    template = *listMenuTemplate;
    template.items = globe->menuChoices;
    template.totalItems = itemCount;
    template.window = window;
    template.moveCursorFunc = ov69_021E68D8;

    globe->listMenu = ListMenuInit(&template, 0, 0, (enum HeapID)(u8)globe->heapID);

    DrawFrameAndWindow1(template.window, 1, ((512 - ((6 * 3) + ((2 * 2) * 3))) - (3 * 3)), 7);
    CopyWindowToVram(window);
}
