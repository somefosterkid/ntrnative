// (Black 2 / White 2) The PC build compiles pokebw2 src/ov036/field_msgbg.c through this file (tools/port_sources.py). Test
// only (port/src/route_test.c, BWPORT_PILOT's Py / Pn tokens): it tells the route pilot that the field's yes/no list is up
// (gPortRouteFieldYesNo: set by MsgBGPrinter_CreateYesNoMenu: 1 + the cursor's row; cleared when MsgBGPrinter_YesNoMenuMain
// has the answer). The scripts' yes/no (scrcmd_menu.c) and the other field questions all use these two. It changes nothing
// else. The DS build never sees this file.
#pragma push_macro("MsgBGPrinter_CreateYesNoMenu")
#pragma push_macro("MsgBGPrinter_YesNoMenuMain")
#define MsgBGPrinter_CreateYesNoMenu MsgBGPrinter_CreateYesNoMenu_game
#define MsgBGPrinter_YesNoMenuMain MsgBGPrinter_YesNoMenuMain_game
#include "../../../../third_party/pokebw2/src/ov036/field_msgbg.c"
#pragma pop_macro("MsgBGPrinter_CreateYesNoMenu")
#pragma pop_macro("MsgBGPrinter_YesNoMenuMain")

extern int gPortRouteFieldYesNo;   // port/src/route_test.c: 0 none, else 1 + the cursor's row (0 Yes, 1 No)

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
