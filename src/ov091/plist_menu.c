// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov165/plist_menu.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pokelist.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/text_banks.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/ui.h"
#include "pml/poke_party.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"

// The party list's menus: what to do with a Pokémon, yes or no, and the buttons of a battle's selection

// The items: 0 summary, 1 the moves usable outside battle, 2 the moves to restore, 3 switch, 4 item, 5 mail,
// 6 back, 7 select, 8 joining or leaving a battle's order, 9 give, 10 take, 11 dress up, 12 read, 13 take mail,
// 14 yes, 15 no, 16 to 19 the moves, and 16 also ends a list, 20 join, 21 leave, 22 and 23 move an item or mail
#define POKELIST_MENU_MAX 8
#define POKELIST_MENU_END 16
#define POKELIST_MENU_MOVE1 16

struct PokeListMenu {
    u8 count;
    u16 items[POKELIST_MENU_MAX + 1];
    AppTaskMenuItem entries[POKELIST_MENU_MAX];
    void *taskMenu;
    u32 unk78;
    NNSG2dCharacterData *chars;
    void *charsFile;
};

static void func_ov091_021bbc04(PokeListWork *wk, PokeListMenu *menu, const u32 *items);
StrBuf *func_ov091_021bbd98(PokeListWork *wk, PokeListMenu *menu, int item);

// The message of each item
// pokebw2's sItemNames
static const u32 data_ov091_021bec8c[] = {
#include "asset/plist_menu_data_ov091_021bec8c.inc"
};

// pokebw2's PokeListMenu_Create
PokeListMenu *func_ov091_021bba8c(PokeListWork *wk) {
    PokeListMenu *menu = GFL_HeapAllocate(wk->heapId, sizeof(PokeListMenu), FALSE, "plist_menu.c", 99);

    GFL_BGSysLoadNCLRDefault(getUINarcIdx(), 31, 0, 0x20, 0x40, wk->heapId);
    menu->charsFile = GFL_G2DIOReadBGNCGR(getUINarcIdx(), 32, FALSE, &menu->chars, wk->heapId);
    return menu;
}

// pokebw2's PokeListMenu_Free
void func_ov091_021bbae0(PokeListWork *wk, PokeListMenu *menu) {
    GFL_HeapFree(menu->charsFile);
    GFL_HeapFree(menu);
}

// Opens the menu of the items listed, which end with POKELIST_MENU_END
// pokebw2's PokeListMenu_Open
void func_ov091_021bbaf8(PokeListWork *wk, PokeListMenu *menu, const u32 *items) {
    AppTaskMenuInit setup;

    func_ov091_021bbc04(wk, menu, items);
    setup.heapId = wk->heapId;
    setup.itemCount = menu->count;
    setup.items = menu->entries;
    setup.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    setup.x = 32;
    setup.y = 24;
    setup.width = 13;
    setup.height = 3;
    func_020355d8(wk->touch);
    menu->taskMenu = func_02027778(&setup, wk->taskMenuRes);
}

// pokebw2's PokeListMenu_OpenYesNo
void func_ov091_021bbb40(PokeListWork *wk, PokeListMenu *menu) {
    AppTaskMenuInit setup;
    u32 items[3] = { 14, 15, POKELIST_MENU_END };

    func_ov091_021bbc04(wk, menu, items);
    setup.heapId = wk->heapId;
    setup.itemCount = menu->count;
    setup.items = menu->entries;
    setup.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    setup.x = 32;
    setup.y = 18;
    setup.width = 8;
    setup.height = 3;
    func_020355d8(wk->touch);
    menu->taskMenu = func_02027778(&setup, wk->taskMenuRes);
}

// pokebw2's PokeListMenu_Close
void func_ov091_021bbba0(PokeListWork *wk, PokeListMenu *menu) {
    u8 i;

    func_02027858(menu->taskMenu);
    for (i = 0; i < menu->count; i++) {
        GFL_StrBufFree(menu->entries[i].str);
    }
    wk->touch = func_020355c4();
}

// pokebw2's PokeListMenu_Update
void func_ov091_021bbbd8(PokeListWork *wk, PokeListMenu *menu) {
    func_02027974(menu->taskMenu);
}

// The item picked, or 0x19 while the menu waits
// pokebw2's PokeListMenu_GetPicked
u32 func_ov091_021bbbe4(PokeListWork *wk, PokeListMenu *menu) {
    if (func_020279e8(menu->taskMenu) == FALSE) {
        return 0x17;
    }
    return menu->items[func_02027a04(menu->taskMenu)];
}

static void func_ov091_021bbc04(PokeListWork *wk, PokeListMenu *menu, const u32 *items) {
    int count;
    int i;
    u8 slot;
    int entry;

    count = 0;
    while (items[count] != POKELIST_MENU_END) {
        count++;
        if (count >= POKELIST_MENU_MAX) {
            break;
        }
    }
    menu->count = 0;
    for (i = 0; i < count; i++) {
        switch (items[i]) {
        case 0:
            menu->items[menu->count] = 0;
            menu->count++;
            break;
        case 1:
            for (slot = 0; slot < 4; slot++) {
                if (func_ov091_021b99e4(wk->pkm, slot) != 0) {
                    menu->items[menu->count] = POKELIST_MENU_MOVE1 + slot;
                    menu->count++;
                }
            }
            break;
        case 2:
            for (slot = 0; slot < 4; slot++) {
                if (PokeParty_GetParam(wk->pkm, PKM_PARAM_MOVE1 + slot, NULL) != 0) {
                    menu->items[menu->count] = POKELIST_MENU_MOVE1 + slot;
                    menu->count++;
                }
            }
            break;
        case 3:
            menu->items[menu->count] = 3;
            menu->count++;
            break;
        case 4:
            menu->items[menu->count] = 4;
            menu->count++;
            break;
        case 5:
            menu->items[menu->count] = 5;
            menu->count++;
            break;
        case 6:
            menu->items[menu->count] = 6;
            menu->count++;
            break;
        case 7:
            menu->items[menu->count] = 7;
            menu->count++;
            break;
        case 8:
            entry = func_ov091_021bb43c(wk->plates[wk->cursorPos]);
            if (entry == POKELIST_ENTRY_ABLE) {
                menu->items[menu->count] = 20;
                menu->count++;
            } else if (entry <= 5) {
                menu->items[menu->count] = 21;
                menu->count++;
            }
            break;
        case 9:
            menu->items[menu->count] = 9;
            menu->count++;
            break;
        case 10:
            menu->items[menu->count] = 10;
            menu->count++;
            break;
        case 11:
            menu->items[menu->count] = 11;
            menu->count++;
            break;
        case 12:
            menu->items[menu->count] = 12;
            menu->count++;
            break;
        case 13:
            menu->items[menu->count] = 13;
            menu->count++;
            break;
        case 14:
            menu->items[menu->count] = 14;
            menu->count++;
            break;
        case 15:
            menu->items[menu->count] = 15;
            menu->count++;
            break;
        }
    }
    for (i = 0; i < menu->count; i++) {
        menu->entries[i].str = func_ov091_021bbd98(wk, menu, menu->items[i]);
        if (menu->items[i] >= POKELIST_MENU_MOVE1 && menu->items[i] <= POKELIST_MENU_MOVE1 + 3 && wk->param->mode != 5) {
            menu->entries[i].color = 0x35e0;
        } else {
            menu->entries[i].color = 0x39e0;
        }
        menu->entries[i].type = menu->items[i] == 6 ? APP_TASKMENU_ITEM_RETURN : 0;
    }
}


// pokebw2's PokeListMenu_GetItemName
StrBuf *func_ov091_021bbd98(PokeListWork *wk, PokeListMenu *menu, int item) {
    StrBuf *str;

    if (item >= POKELIST_MENU_MOVE1 && item <= POKELIST_MENU_MOVE1 + 3) {
        u32 move = PokeParty_GetParam(wk->pkm, PKM_PARAM_MOVE1 + item - POKELIST_MENU_MOVE1, NULL);
        MsgData *moveNames = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_MOVE_NAMES, wk->heapId);

        str = GFL_MsgDataLoadStrbufNew(moveNames, move);
        GFL_MsgDataFree(moveNames);
    } else {
        str = GFL_MsgDataLoadStrbufNew(wk->msgData, data_ov091_021bec8c[item]);
    }
    return str;
}

// A button of a battle's selection, at a tile of the bottom bar
// pokebw2's PokeListMenu_CreateButton
void *func_ov091_021bbdec(PokeListWork *wk, PokeListMenu *menu, u32 msgId, u8 x, u8 y, BOOL isBack) {
    AppTaskMenuItem item;
    void *button;

    item.str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);
    item.color = 0x39e3;
    if (isBack == FALSE) {
        item.type = 0;
    } else {
        item.type = APP_TASKMENU_ITEM_RETURN;
    }
    button = func_02027fec(wk->taskMenuRes, &item, x, y, 10, wk->heapId);
    GFL_StrBufFree(item.str);
    return button;
}

// pokebw2's PokeListMenu_FreeButton
void func_ov091_021bbe44(void *button) {
    if (button != NULL) {
        func_02028148(button);
    }
}

// pokebw2's PokeListMenu_UpdateButton
void func_ov091_021bbe50(void *button) {
    if (button != NULL) {
        func_02028178(button);
    }
}

// pokebw2's PokeListMenu_SetButtonActive
void func_ov091_021bbe5c(void *button, BOOL active) {
    if (button != NULL) {
        func_02028218(button, active);
    }
}

// pokebw2's PokeListMenu_SetButtonPressed
void func_ov091_021bbe68(void *button, BOOL pressed) {
    if (button != NULL) {
        func_0202822c(button, pressed);
    }
}
