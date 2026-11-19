#include "global.h"

#include "bg_window.h"
#include "geonet_globe.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "pm_string.h"
#include "render_window.h"

typedef struct GeonetGlobeMenuItem {
    u32 messageId;
    u32 value;
} GeonetGlobeMenuItem;

void ov69_021E68D8(ListMenu *listMenu, s32 unused, u8 mute);

void ov69_021E68EC(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *listMenuTemplate, const GeonetGlobeMenuItem *items);

void ov69_021E68EC(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *listMenuTemplate, const GeonetGlobeMenuItem *items) {
    ListMenuTemplate template;

    AddWindow(globe->bgConfig, window, windowTemplate);
    globe->menuChoices = ListMenuItems_New(listMenuTemplate->totalItems, globe->heapID);

    for (int i = 0; i < listMenuTemplate->totalItems; i++) {
        ListMenuItems_AppendFromMsgData(globe->menuChoices, globe->messageLoader, items[i].messageId, items[i].value);
    }

    template = *listMenuTemplate;
    template.items = globe->menuChoices;
    template.window = window;
    template.moveCursorFunc = ov69_021E68D8;
    globe->listMenu = ListMenuInit(&template, 0, 0, (enum HeapID)(u8)globe->heapID);

    DrawFrameAndWindow1(template.window, 1, ((512 - ((6 * 3) + ((2 * 2) * 3))) - (3 * 3)), 7);
    CopyWindowToVram(window);
}
