// The PC build compiles src/ov091/plist_menu.c through this file. Test only: each update of the Pokemon list's submenu tells the
// route pilot (port/src/route_fly.h, the F:fly token) its items (16-19 = move slots 0-3) and where its cursor is. The DS build never sees this file.
#pragma push_macro("func_ov091_021bbbd8")
#define func_ov091_021bbbd8 func_ov091_021bbbd8_game
#include "../../../../src/ov091/plist_menu.c"
#pragma pop_macro("func_ov091_021bbbd8")

#include "port_route.h"

void func_ov091_021bbbd8(PokeListWork *wk, PokeListMenu *menu);
void func_ov091_021bbbd8(PokeListWork *wk, PokeListMenu *menu) {
    int i;

    func_ov091_021bbbd8_game(wk, menu);
    gPortRoutePList.menuCount = menu->count < 8 ? menu->count : 8;
    for (i = 0; i < gPortRoutePList.menuCount; i++) {
        gPortRoutePList.menuItems[i] = menu->items[i];
    }
    gPortRoutePList.menuCursor = (int)func_02027a04(menu->taskMenu);
    gPortRoutePList.menuVblank = OS_GetVBlankCount();
}
