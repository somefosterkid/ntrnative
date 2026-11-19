// (Black / White) The PC build compiles src/ov021/field_msgbg.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c, BWPORT_PILOT's Py / Pn tokens): it tells the route pilot that the field's yes/no list is up
// (gPortRouteFieldYesNo: set by func_ov021_02190be4, pokebw2's MsgBGPrinter_CreateYesNoMenu: 1 + the cursor's row;
// cleared when func_ov021_02190c78, MsgBGPrinter_YesNoMenuMain, has the answer). The scripts' yes/no (s0047_YesNoWin)
// and the other field questions all use these two. It changes nothing else. The DS build never sees this file.
#pragma push_macro("func_ov021_02190be4")
#pragma push_macro("func_ov021_02190c78")
#pragma push_macro("ListMenuUI_Update")
#define func_ov021_02190be4 func_ov021_02190be4_game
#define func_ov021_02190c78 func_ov021_02190c78_game
#define ListMenuUI_Update ListMenuUI_Update_game
#include "../../../../src/ov021/field_msgbg.c"
#pragma pop_macro("func_ov021_02190be4")
#pragma pop_macro("func_ov021_02190c78")
#pragma pop_macro("ListMenuUI_Update")

extern int gPortRouteFieldYesNo;   // port/src/route_test.c: 0 none, else 1 + the cursor's row (0 Yes, 1 No)

ListMenuUI *func_ov021_02190be4(FieldMsgBG *param_1, u32 param_2) {
    ListMenuUI *ui = func_ov021_02190be4_game(param_1, param_2);

    gPortRouteFieldYesNo = 1 + (param_2 != 0);
    return ui;
}

// the script's list menus (FieldScriptEnv_UpdateListMenu; the yes/no above runs ListMenuUI_Update inside this file, so not
// through this one): the Pl:N token's gPortRouteList* (port/src/route_test.c)
extern int gPortRouteList, gPortRouteListCount, gPortRouteListRow, gPortRouteListResult;

intptr_t ListMenuUI_Update(ListMenuUI *menu);   // the header was read under the macro

intptr_t ListMenuUI_Update(ListMenuUI *menu) {
    intptr_t r = ListMenuUI_Update_game(menu);
    u16 row = 0;

    if (r == BMPMENULIST_NULL) {
        func_020202ec(menu->list, &row);
        gPortRouteList = 1;
        gPortRouteListCount = (int)ListMenuCore_GetFirstFreeIndex(menu->options);
        gPortRouteListRow = row;
    } else {
        gPortRouteList = 0;
        gPortRouteListResult = (int)r;
    }
    return r;
}

u32 func_ov021_02190c78(ListMenuUI *window) {
    u32 r = func_ov021_02190c78_game(window);

    if (r != 2) {   // 0 Yes, 1 No (or B); 2 waiting
        gPortRouteFieldYesNo = 0;
    }
    return r;
}
