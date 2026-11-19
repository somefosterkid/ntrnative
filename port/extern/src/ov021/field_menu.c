// The PC build compiles src/ov021/field_menu.c through this file (tools/port_sources.py). Test only (the twin of
// port/extern_b2w2/src/ov036/field_menu.c): each update of the touch-screen field menu tells the route pilot
// (port/src/route_test.c, its S token) whether the menu takes input, where the cursor is and where SAVE is, so a save
// picks SAVE from the menu's real state: its entries differ before the first Pokemon and before the Pokedex, and SAVE
// is not always down the first column. The DS build never sees this file.
#pragma push_macro("func_ov021_021a85b4")
#define func_ov021_021a85b4 func_ov021_021a85b4_game
#include "../../../../src/ov021/field_menu.c"
#pragma pop_macro("func_ov021_021a85b4")

#include "port_route.h"

#define FIELD_MENU_STATE_INPUT 3   // the update's case 3: the keys move the cursor
#define FIELD_MENU_ITEM_POKEMON 1
#define FIELD_MENU_ITEM_BAG 3
#define FIELD_MENU_ITEM_SAVE 5     // the items: 0 return, 1 Pokemon, 2 Pokedex, 3 bag, 4 trainer card, 5 save, 6 options, 7 none

// pokebw2's FieldMenu_Update
void func_ov021_021a85b4(FieldMenu *menu) {
    func_ov021_021a85b4_game(menu);
    u32 slot = menu->cursorY >= 3 ? FIELD_MENU_SLOT_BOTTOM : menu->cursorX + menu->cursorY * 2u;

    gPortRouteMenu.input = menu->state == FIELD_MENU_STATE_INPUT;
    gPortRouteMenu.onSave = slot < 7 && menu->items[slot].active && menu->items[slot].itemId == FIELD_MENU_ITEM_SAVE;
    gPortRouteMenu.grid = 1;
    gPortRouteMenu.slot = (int)slot;
    gPortRouteMenu.saveSlot = -1;
    gPortRouteMenu.pokeSlot = -1;
    gPortRouteMenu.bagSlot = -1;
    for (int i = 0; i < 7; i++) {
        if (menu->items[i].active && menu->items[i].itemId == FIELD_MENU_ITEM_BAG) {
            gPortRouteMenu.bagSlot = i;
        }
        if (menu->items[i].active && menu->items[i].itemId == FIELD_MENU_ITEM_POKEMON) {
            gPortRouteMenu.pokeSlot = i;
        }
        if (menu->items[i].active && menu->items[i].itemId == FIELD_MENU_ITEM_SAVE) {
            gPortRouteMenu.saveSlot = i;
        }
    }
    gPortRouteMenu.vblank = OS_GetVBlankCount();
}
