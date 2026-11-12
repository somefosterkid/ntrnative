// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zknlist_ui.h: the lines in use so far.
#ifndef BWPORT_APP_ZKNLIST_UI_H
#define BWPORT_APP_ZKNLIST_UI_H

#include "types.h"
#include "app/zknlist_main.h"

// Black 2's, not checked against Black
// What ZknListUI_Main returns besides -1: the picture of the species, the icon of a row from 1, and the buttons
enum {
    ZKNLIST_UI_MON,
    ZKNLIST_UI_ROW,
    ZKNLIST_UI_START = ZKNLIST_UI_ROW + ZKNLIST_ROW_COUNT,
    ZKNLIST_UI_SELECT,
    ZKNLIST_UI_SHORTCUT,
    ZKNLIST_UI_EXIT,
    ZKNLIST_UI_RETURN,
    ZKNLIST_UI_CANCEL,
};

// pokebw2's ZknListUI_Main
// What the touch panel and the keys choose in the list: one of the values above, or -1
int func_ov209_021be658(ZknListWork *wk);

#endif // BWPORT_APP_ZKNLIST_UI_H
