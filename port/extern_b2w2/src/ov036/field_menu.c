// Black 2's and White 2's touch-screen field menu (pokebw2's src/ov036/field_menu.c compiled through this file). Test only:
// each update tells the route pilot (port/src/route_test.c, its S token) whether the menu takes input and whether the
// cursor is on SAVE, so a save waits for the menu instead of a fixed count of frames. The DS build never sees this file.
#define FieldMenu_Update FieldMenu_Update_game
#include "../../../../third_party/pokebw2/src/ov036/field_menu.c"
#undef FieldMenu_Update

#include "port_route.h"

void FieldMenu_Update(FieldMenu *menu) {
    FieldMenu_Update_game(menu);
    u32 slot = menu->cursorY >= FIELD_MENU_ROW_BOTTOM ? FIELD_MENU_SLOT_BOTTOM : menu->cursorX + menu->cursorY * 2u;

    gPortRouteMenu.input = menu->state == FIELD_MENU_STATE_INPUT;
    gPortRouteMenu.onSave = slot < FIELD_MENU_SLOT_COUNT && menu->items[slot].active &&
                            menu->items[slot].itemId == FIELD_MENU_ITEM_SAVE;
    gPortRouteMenu.vblank = OS_GetVBlankCount();
}
