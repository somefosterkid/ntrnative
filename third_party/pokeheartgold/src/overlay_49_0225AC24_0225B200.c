#include "global.h"

#include "bg_window.h"
#include "list_menu_items.h"

void ov49_0225B200(ListMenuItem **arg0);

void ov49_0225B200(ListMenuItem **arg0) {
    ListMenuItem *items;

    items = *arg0;
    if (items != NULL) {
        ListMenuItems_Delete(items);
        *arg0 = NULL;
    }
}

void ov49_0225B214(Window *arg0, BgConfig **arg1);

void ov49_0225B214(Window *window, BgConfig **arg1) {
    AddWindowParameterized(*arg1, window, 1U, 4U, 4U, 0x17U, 0x10U, 5U, 0x5EU);
    FillWindowPixelBuffer(window, 0xFU);
}
