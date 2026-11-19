// (Black 2 / White 2) The PC build compiles pokebw2 src/ov036/field_msgbg.c through this file (tools/port_sources.py). Test
// only (port/src/route_test.c, BWPORT_PILOT's Py / Pn tokens): it tells the route pilot that the field's yes/no list is up
// (gPortRouteFieldYesNo: set by MsgBGPrinter_CreateYesNoMenu: 1 + the cursor's row; cleared when MsgBGPrinter_YesNoMenuMain
// has the answer). The scripts' yes/no (scrcmd_menu.c) and the other field questions all use these two. A two-item list
// that other files run (ListMenuUI_Update from outside: a script's list menu, such as the C-Gear's "Turn on the C-Gear?"
// Yes / No, which B does not cancel) sets it to 3 + its cursor's row each frame, 0 once it has an answer. It changes
// nothing else. The DS build never sees this file.
#pragma push_macro("MsgBGPrinter_CreateYesNoMenu")
#pragma push_macro("MsgBGPrinter_YesNoMenuMain")
#pragma push_macro("ListMenuUI_Update")
#define ListMenuUI_Update ListMenuUI_Update_game
#define MsgBGPrinter_CreateYesNoMenu MsgBGPrinter_CreateYesNoMenu_game
#define MsgBGPrinter_YesNoMenuMain MsgBGPrinter_YesNoMenuMain_game
#include "../../../../third_party/pokebw2/src/ov036/field_msgbg.c"
#pragma pop_macro("MsgBGPrinter_CreateYesNoMenu")
#pragma pop_macro("MsgBGPrinter_YesNoMenuMain")
#pragma pop_macro("ListMenuUI_Update")

extern int gPortRouteFieldYesNo;   // port/src/route_test.c: 0 none, else 1 + the cursor's row (0 Yes, 1 No); 3 + the
                                   // row: a two-item list (Yes first)

ListMenuUI *MsgBGPrinter_CreateYesNoMenu(FieldMsgBG *sys, u32 cursorRow) {
    ListMenuUI *menu = MsgBGPrinter_CreateYesNoMenu_game(sys, cursorRow);

    gPortRouteFieldYesNo = 1 + (cursorRow != 0);
    return menu;
}

u32 MsgBGPrinter_YesNoMenuMain(ListMenuUI *menu) {
    u32 r = MsgBGPrinter_YesNoMenuMain_game(menu);

    if (r != MSGBG_YES_NO_WAITING) {
        gPortRouteFieldYesNo = 0;
    }
    return r;
}

extern int gPortRouteList, gPortRouteListCount, gPortRouteListRow, gPortRouteListResult;   // the Pl:N token (port/src/route_test.c)

s32 ListMenuUI_Update(ListMenuUI *menu);   // the header was read under the macro

s32 ListMenuUI_Update(ListMenuUI *menu) {
    s32 r = ListMenuUI_Update_game(menu);
    u16 row;

    if (r == BMPMENULIST_NULL) {
        BmpMenuList_GetCursorIndex(menu->list, &row);
        gPortRouteList = 1;
        gPortRouteListCount = (int)ListMenuCore_GetOptionCount(menu->options);
        gPortRouteListRow = row;
    } else {
        gPortRouteList = 0;
        gPortRouteListResult = (int)r;
    }
    if (ListMenuCore_GetOptionCount(menu->options) == 2) {
        BmpMenuList_GetCursorIndex(menu->list, &row);
        gPortRouteFieldYesNo = r == BMPMENULIST_NULL ? 3 + (row != 0) : 0;
    }
    return r;
}
