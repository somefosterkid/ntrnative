// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov142/itemmenu.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "app/itemmenu.h"
#include "app/bag.h"
#include "app/bag_item.h"
#include "app/itemmenu_disp.h"
#include "constants/items.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/player_action.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/math.h"
#include "pml/item.h"
#include "save/bag.h"
#include "save/encounter.h"
#include "save/save_control.h"
#include "system/app_keycursor.h"
#include "system/blink_palanm.h"
#include "system/scroll_bar.h"
#include "system/shortcut_util.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/myitem_savedata.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/btl_main.h"

// The bag (itemmenu.c): its proc, the item list and its menus

// The key items that are used from the bag, and what using them does
// Black 2's, not checked against Black
typedef struct {
    u16 item;
    u16 count;
    BOOL (*use)(ItemMenuWork *work);
} ItemMenuFieldUse;

// An item of a pocket and what it is sorted by
typedef struct {
    BagItem item;
    u32 key;
} ItemMenuSortEntry;

void func_ov070_021b6100(ItemMenuWork *work, ItemMenuState state);
void func_ov070_021b6120(ItemMenuWork *work);
void func_ov070_021b6138(ItemMenuWork *work);
void func_ov070_021b624c(ItemMenuWork *work, s32 from, s32 to);
void func_ov070_021b6288(ItemMenuWork *work, u32 oldPocket, u32 newPocket);
BOOL func_ov070_021b6300(ItemMenuWork *work, s32 count, BOOL wrap);
BOOL func_ov070_021b6374(ItemMenuWork *work, s32 count, BOOL wrap);
BOOL func_ov070_021b63f8(ItemMenuWork *work);
BOOL func_ov070_021b64a0(ItemMenuWork *work);
BOOL func_ov070_021b6554(ItemMenuWork *work);
BOOL func_ov070_021b65e8(ItemMenuWork *work);
BOOL func_ov070_021b66b8(void);
void func_ov070_021b66e8(ItemMenuWork *work);
void func_ov070_021b66fc(ItemMenuWork *work);
BOOL func_ov070_021b68e8(u16 item);
u32 func_ov070_021b690c(u16 item);
void func_ov070_021b6934(ItemMenuWork *work);
void func_ov070_021b6a14(ItemMenuWork *work);
void func_ov070_021b6a78(ItemMenuWork *work);
void func_ov070_021b6ad8(ItemMenuWork *work);
void func_ov070_021b761c(ItemMenuWork *work);
void func_ov070_021b7640(ItemMenuWork *work);
void func_ov070_021b7688(ItemMenuWork *work);
void func_ov070_021b76e4(ItemMenuWork *work);
static void func_ov070_021b7700(ItemMenuWork *work);
void func_ov070_021b77e4(ItemMenuWork *work);
void func_ov070_021b7804(ItemMenuWork *work);
void func_ov070_021b7868(ItemMenuWork *work);
void func_ov070_021b795c(ItemMenuWork *work);
void func_ov070_021b7a7c(ItemMenuWork *work);
void func_ov070_021b7b08(ItemMenuWork *work);
static void func_ov070_021b7b80(ItemMenuWork *work);
static void func_ov070_021b7c88(ItemMenuWork *work);
BOOL func_ov070_021b8134(ItemMenuWork *work, s32 row);
u32 func_ov070_021b81b0(s32 pocket);
void func_ov070_021b81b8(ItemMenuWork *work);
static void func_ov070_021b81f4(ItemMenuWork *work);
void func_ov070_021b822c(ItemMenuWork *work, ItemData *data, BagItem *slot, u8 *actions);
void func_ov070_021b83b8(u32 button, u32 event, void *data);
void func_ov070_021b8600(TCB *tcb, void *data);
void func_ov070_021b8640(ItemMenuWork *work);
static void func_ov070_021b8688(ItemMenuWork *work);
BOOL func_ov070_021b86b8(GameProc *proc, u32 *state, void *param, void *data);
BOOL func_ov070_021b8904(GameProc *proc, u32 *state, void *param, void *data);
BOOL func_ov070_021b8990(GameProc *proc, u32 *state, void *param, void *data);
static void func_ov070_021b8af4(ItemMenuWork *work);
void func_ov070_021b8b0c(ItemMenuWork *work);
void func_ov070_021b8b24(ItemMenuWork *work);
static void func_ov070_021b8b6c(ItemMenuWork *work);
void func_ov070_021b8b7c(ItemMenuWork *work);
void func_ov070_021b8c1c(ItemMenuWork *work, u32 button, ItemMenuState next);
void func_ov070_021b8cb4(ItemMenuWork *work);
static s32 func_ov070_021b8dac(void);
void func_ov070_021b8ce4(ItemMenuWork *work, u32 oldPocket, BOOL right, ItemMenuState next);
void func_ov070_021b8d88(ItemMenuWork *work);
BOOL func_ov070_021b8f68(ItemMenuWork *work, u8 fieldPocket);

// pokebw2's ItemMenuProc_Exit
// pokebw2's ItemMenuProc_Init
// pokebw2's ItemMenuProc_Main
const GameProcFunctions BAG_PROC_FUNCTIONS = { func_ov070_021b86b8, func_ov070_021b8904, func_ov070_021b8990 };

// pokebw2's ItemMenu_RedrawList

void func_ov070_021b7d20(ItemMenuWork *work, u32 count);
void func_ov070_021b7cd0(ItemMenuWork *work);
BOOL func_ov070_021b6af0(ItemMenuWork *work);
void func_ov070_021b80c4(ItemMenuWork *work, BOOL keys);
void func_ov070_021b7f54(ItemMenuWork *work);
void func_ov070_021b8e84(ItemMenuWork *work);
void func_ov070_021b7504(ItemMenuWork *work);
void func_ov070_021b7d9c(ItemMenuWork *work, u32 mode);
BOOL func_ov070_021b7df4(ItemMenuWork *work);
void func_ov070_021b7dd0(ItemMenuWork *work);
void func_ov070_021b8dcc(ItemMenuWork *work);
void func_ov070_021b8e08(ItemMenuWork *work);
void func_ov070_021b8f0c(ItemMenuWork *work);
void func_ov070_021b7128(ItemMenuWork *work);
void func_ov070_021b832c(ItemMenuWork *work);
void func_ov070_021b6ef8(ItemMenuWork *work);
void func_ov070_021b8080(ItemMenuWork *work);
void func_ov070_021b8ea8(ItemMenuWork *work);
void func_ov070_021b8f90(ItemMenuWork *work);
void func_ov070_021b8e48(ItemMenuWork *work);

void func_ov070_021b6100(ItemMenuWork *work, ItemMenuState state) {
    work->state = state;
    setKeypressFramecounts(work->savedRepeatWait, work->savedRepeatStart);
}

void func_ov070_021b6120(ItemMenuWork *work) {
    work->drawnScroll = 0xffff;
    func_ov070_021b6138(work);
}

// pokebw2's ItemMenu_UpdateList
void func_ov070_021b6138(ItemMenuWork *work) {
    if (func_020355c4() == FALSE) {
        func_ov070_021b9850(work);
    } else {
        func_ov070_021b9aac(work);
    }
    func_ov070_021ba218(work);
    func_ov070_021b81f4(work);
    work->listDirty = TRUE;
}

BagItem *func_ov070_021b616c(ItemMenuWork *work, s32 index) {
    if (work->movingItem != FALSE) {
        return &work->items[index];
    }
    return BagSave_GetItemIndexHandle(work->bag, (u16)work->pocket, index);
}


// pokebw2's ItemMenu_GetSellPrice
s32 func_ov070_021b619c(u32 item, s32 count, HeapID heapId) {
    s32 price = GetItemParam(item, ITEM_PARAM_PRICE, heapId);

    return price / 2 * count;
}

s32 func_ov070_021b61b8(ItemMenuWork *work) {
    return work->cursorRow + work->scroll + 1;
}

u32 func_ov070_021b61c8(ItemMenuWork *work) {
    if (work->movingItem != FALSE) {
        return BagSave_GetUniqueItemCount(work->items, NELEMS(work->items));
    }
    return BagSave_GetPocketItemCount(work->bag, work->pocket);
}

void func_ov070_021b61f4(ItemMenuWork *work, u32 index, u32 item) {
    loadItemNameToStrbuf(work->wordSet, index, item);
}

void func_ov070_021b6204(ItemMenuWork *work, u32 index, u32 item, BOOL plural, BOOL a4) {
    loadItemText(work->wordSet, index, item, plural, a4);
}





void func_ov070_021b621c(ItemMenuWork *work, s32 a, s32 b) {
    BagItem temp;

    MI_CpuCopy8(&work->items[a], &temp, sizeof(BagItem));
    MI_CpuCopy8(&work->items[b], &work->items[a], sizeof(BagItem));
    MI_CpuCopy8(&temp, &work->items[b], sizeof(BagItem));
}


// pokebw2's ItemMenu_MoveItem
void func_ov070_021b624c(ItemMenuWork *work, s32 from, s32 to) {
    if (from == to) {
        return;
    }
    if (from < to) {
        for (; from < to; from++) {
            func_ov070_021b621c(work, from, from + 1);
        }
    } else {
        for (; from > to; from--) {
            func_ov070_021b621c(work, from, from - 1);
        }
    }
}

// pokebw2's ItemMenu_ChangePocket
void func_ov070_021b6288(ItemMenuWork *work, u32 oldPocket, u32 newPocket) {
    s16 row;
    s16 scroll;

    func_02008298(work->cursor, (u16)oldPocket);
    func_02008288(work->cursor, (u16)oldPocket, work->cursorRow, work->scroll + 1);
    func_02008270(work->cursor, (u16)newPocket, &row, &scroll);
    work->cursorRow = row;
    work->scroll = scroll - 1;
    func_ov070_021ba6a4(work);
    func_ov070_021baf38(work, newPocket);
    func_ov070_021ba96c(work, newPocket);
    func_ov070_021baaa4(work, newPocket);
}

// pokebw2's ItemMenu_CursorDown
BOOL func_ov070_021b6300(ItemMenuWork *work, s32 count, BOOL wrap) {
    s32 row;
    s32 scroll;

    if (count == 0) {
        return FALSE;
    }
    row = work->cursorRow;
    scroll = work->scroll;
    if (row == ITEMMENU_LIST_ROWS - 1 && scroll + ITEMMENU_LIST_ROWS + 1 < count) {
        work->scroll++;
    } else if (row != ITEMMENU_LIST_ROWS - 1 && row + 1 < count) {
        work->cursorRow++;
    } else if (wrap == TRUE) {
        work->scroll = -1;
        work->cursorRow = 0;
    }
    if (row != work->cursorRow || scroll != work->scroll) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ItemMenu_CursorUp
BOOL func_ov070_021b6374(ItemMenuWork *work, s32 count, BOOL wrap) {
    s32 row;
    s32 scroll;

    if (count == 0) {
        return FALSE;
    }
    row = work->cursorRow;
    scroll = work->scroll;
    if (row == 0 && scroll != -1) {
        work->scroll--;
    } else if (row != 0) {
        work->cursorRow--;
    } else if (wrap == TRUE) {
        work->scroll = count - (ITEMMENU_LIST_ROWS + 1);
        if (work->scroll < -1) {
            work->scroll = -1;
        }
        work->cursorRow = ITEMMENU_LIST_ROWS - 1;
        if (work->cursorRow >= count) {
            work->cursorRow = count - 1;
        }
    }
    if (row != work->cursorRow || scroll != work->scroll) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov070_021b63f8(ItemMenuWork *work) {
    u32 x;
    u32 y;
    s32 count;
    s32 scroll;

    count = func_ov070_021b61c8(work);
    if (count >= ITEMMENU_LIST_ROWS + 1 && func_02035cb8(&x, &y) == TRUE) {
        if (x >= 256 || x < 224) {
            return FALSE;
        }
        if (work->touchHeld == FALSE) {
            if (func_02035c60() == TRUE) {
                if (y < 16 || y >= 152) {
                    return FALSE;
                }
                work->touchHeld = TRUE;
            } else {
                return FALSE;
            }
        }
        func_ov070_021b80c4(work, FALSE);
        scroll = work->scroll;
        work->scroll = func_0202ede4(count - ITEMMENU_LIST_ROWS, y, 26, 142, 0) - 1;
        func_ov070_021ba654(work);
        if (work->scroll != scroll) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov070_021b64a0(ItemMenuWork *work) {
    BOOL moved = FALSE;
    s32 from;
    s32 count;
    s32 to;
    u32 i;

    from = func_ov070_021b61b8(work);
    count = func_ov070_021b61c8(work);
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        moved = func_ov070_021b6300(work, count, TRUE);
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        moved = func_ov070_021b6374(work, count, TRUE);
    } else if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
        for (i = 0; i < ITEMMENU_LIST_ROWS; i++) {
            if (!func_ov070_021b6300(work, count, FALSE)) {
                break;
            }
        }
        if (i != 0) {
            moved = TRUE;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
        for (i = 0; i < ITEMMENU_LIST_ROWS; i++) {
            if (!func_ov070_021b6374(work, count, FALSE)) {
                break;
            }
        }
        if (i != 0) {
            moved = TRUE;
        }
    }
    if (moved) {
        to = func_ov070_021b61b8(work);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov070_021b624c(work, from, to);
    }
    return moved;
}

BOOL func_ov070_021b6554(ItemMenuWork *work) {
    BOOL moved = FALSE;
    s32 count;
    u32 i;

    count = func_ov070_021b61c8(work);
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        moved = func_ov070_021b6300(work, count, TRUE);
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        moved = func_ov070_021b6374(work, count, TRUE);
    } else if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
        for (i = 0; i < ITEMMENU_LIST_ROWS; i++) {
            if (!func_ov070_021b6300(work, count, FALSE)) {
                break;
            }
        }
        if (i != 0) {
            moved = TRUE;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
        for (i = 0; i < ITEMMENU_LIST_ROWS; i++) {
            if (!func_ov070_021b6374(work, count, FALSE)) {
                break;
            }
        }
        if (i != 0) {
            moved = TRUE;
        }
    }
    return moved;
}

BOOL func_ov070_021b65e8(ItemMenuWork *work) {
    u32 x;
    u32 y;
    s32 count;
    s32 from;
    s32 to;
    u32 steps;
    u32 i;

    if (func_02035cb8(&x, &y) == TRUE) {
        u32 row;
        if (x < 144 || x > 231 || y < 12 || y > 155) {
            return FALSE;
        }
        row = (y - 12) / 24;
        if (work->cursorRow == row) {
            return FALSE;
        }
        count = func_ov070_021b61c8(work);
        from = func_ov070_021b61b8(work);
        steps = MATH_ABS(work->cursorRow - (s32)row);
        if (work->cursorRow < row) {
            for (i = 0; i < steps; i++) {
                if (!func_ov070_021b6300(work, count, FALSE)) {
                    break;
                }
            }
        } else if (work->cursorRow > row) {
            for (i = 0; i < steps; i++) {
                if (!func_ov070_021b6374(work, count, FALSE)) {
                    break;
                }
            }
        }
        if (i == 0) {
            return FALSE;
        }
        to = func_ov070_021b61b8(work);
        func_ov070_021b624c(work, from, to);
        func_ov070_021b80c4(work, FALSE);
        return TRUE;
    }
    return FALSE;
}





// Whether the button that ends moving an item is touched
// pokebw2's ItemMenu_IsMoveDoneTouched
BOOL func_ov070_021b66b8(void) {
    u32 x;
    u32 y;

    if (func_02035d20(&x, &y) == TRUE && x >= 224 && x <= 247 && y >= 168 && y <= 191) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ItemMenu_EndMoveItem
void func_ov070_021b66e8(ItemMenuWork *work) {
    func_ov070_021bb5dc(work, TRUE);
    func_ov070_021b8af4(work);
}

void func_ov070_021b66fc(ItemMenuWork *work) {
    s32 from;
    s32 to;

    func_0202ea40(work->paletteAnim);
    if (func_0201c6b4(work->printQueue) == FALSE) {
        return;
    }
    from = func_ov070_021b61b8(work);
    if (func_ov070_021b63f8(work)) {
        to = func_ov070_021b61b8(work);
        func_ov070_021b624c(work, from, to);
        work->drawnScroll = 0xffff;
        func_ov070_021b6138(work);
        return;
    }
    if (func_02035c38() == FALSE) {
        work->touchHeld = FALSE;
        if (work->touchMoved == TRUE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            work->touchMoved = FALSE;
            BagSave_CopyPocketRaw(work->bag, work->items, work->pocket, FALSE);
            func_0204b878(work->listCursor, 1);
            func_ov070_021bb5dc(work, TRUE);
            work->movingItem = FALSE;
            func_ov070_021b80c4(work, FALSE);
            func_ov070_021b8af4(work);
            return;
        }
    }
    if (work->touchHeld == TRUE) {
        return;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_SELECT)) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        BagSave_CopyPocketRaw(work->bag, work->items, work->pocket, FALSE);
        func_0204b878(work->listCursor, 1);
        func_ov070_021bb5dc(work, TRUE);
        work->movingItem = FALSE;
        func_ov070_021b80c4(work, TRUE);
        func_ov070_021b8af4(work);
        return;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_0204b878(work->listCursor, 1);
        work->movingItem = FALSE;
        func_ov070_021b80c4(work, TRUE);
        work->drawnScroll = 0xffff;
        func_ov070_021b6138(work);
        func_ov070_021b8c1c(work, 4, func_ov070_021b66e8);
        return;
    }
    if (func_ov070_021b66b8() == TRUE) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_0204b878(work->listCursor, 1);
        work->movingItem = FALSE;
        func_ov070_021b80c4(work, FALSE);
        work->drawnScroll = 0xffff;
        func_ov070_021b6138(work);
        func_ov070_021b8c1c(work, 4, func_ov070_021b66e8);
        return;
    }
    if (func_ov070_021b65e8(work)) {
        work->touchMoved = TRUE;
        func_ov070_021ba6a4(work);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        work->drawnScroll = 0xffff;
        func_ov070_021b6138(work);
        return;
    }
    if (func_ov070_021b64a0(work)) {
        func_ov070_021ba6a4(work);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        work->drawnScroll = 0xffff;
        func_ov070_021b6138(work);
    }
}


// pokebw2's ItemMenu_IsRepel
BOOL func_ov070_021b68e8(u16 item) {
    return item == ITEM_SUPER_REPEL || item == ITEM_MAX_REPEL || item == ITEM_REPEL;
}

// pokebw2's ItemMenu_GetFieldEffect
u32 func_ov070_021b690c(u16 item) {
    if (func_ov070_021b68e8(item)) {
        return 1;
    }
    if (item == ITEM_LIGHT_STONE || item == ITEM_DARK_STONE) {
        return 2;
    }
    return 0;
}

void EncountSave_SetRepelSteps(EncountSave *encount, u8 steps);
BOOL EncountSave_IsRepelDepleted(EncountSave *encount);
void func_ov070_021b6934(ItemMenuWork *work) {
    EncountSave *encount;
    u8 steps;

    if (func_ov070_021b68e8((u16)work->item)) {
        encount = SaveControl_GetEncountSave(GameData_GetSaveControl(work->gameData));
        if (EncountSave_IsRepelDepleted(encount)) {
            steps = GetItemParam((u16)work->item, ITEM_PARAM_HOLD_PARAM, work->heapId);
            EncountSave_SetRepelSteps(encount, steps);
            func_ov070_021b7d20(work, TRUE);
            GFL_MsgDataLoadStrbuf(work->msgData, BtlMain_Text_063, work->strbuf);
            copyVarForText(work->wordSet, 0, work->playerInfo);
            func_ov070_021b61f4(work, 1, work->item);
            GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
            func_ov070_021bacf0(work, TRUE);
            GFL_SndSEPlay(SEQ_SE_SYS_92);
        } else {
            GFL_MsgDataLoadStrbuf(work->msgData, BtlMain_Text_064, work->strbuf);
            GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
            func_ov070_021bacf0(work, TRUE);
        }
        func_ov070_021b6100(work, func_ov070_021b6a14);
    }
}


// Waits for a message to be read
// pokebw2's ItemMenu_StateWaitMessage
void func_ov070_021b6a14(ItemMenuWork *work) {
    if (func_ov070_021badf0(work) && ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60())) {
        GFL_BGSysClearScr(3);
        work->drawnScroll = 0xffff;
        func_ov070_021b6138(work);
        work->listDirty = TRUE;
        func_ov070_021ba6a4(work);
        func_0204b92c(work->scrollBar, TRUE);
        func_ov070_021bb3f4(work, TRUE);
        func_ov070_021b8af4(work);
    }
}

// pokebw2's ItemMenu_StateUseLightStone
void func_ov070_021b6a78(ItemMenuWork *work) {
    if (work->item == ITEM_LIGHT_STONE || work->item == ITEM_DARK_STONE) {
        GFL_MsgDataLoadStrbuf(work->msgData, BtlMain_Text_056, work->strbuf);
        copyVarForText(work->wordSet, 0, work->playerInfo);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021bacf0(work, TRUE);
        func_ov070_021b6100(work, func_ov070_021b6a14);
    }
}

// pokebw2's ItemMenu_StateShowMessage
void func_ov070_021b6ad8(ItemMenuWork *work) {
    func_ov070_021bacf0(work, TRUE);
    func_ov070_021b6100(work, func_ov070_021b6a14);
}

BOOL func_ov070_021b6af0(ItemMenuWork *work) {
    if (work->menuAction == 0) {
        if (func_02028754(work->perms, 0)) {
            work->result = 10;
            func_ov070_021b6100(work, FALSE);
            return TRUE;
        }
        GFL_MsgDataLoadStrbuf(work->msgData, 0x3a, work->strbuf);
        copyVarForText(work->wordSet, 0, work->playerInfo);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021b6100(work, func_ov070_021b6ad8);
        return FALSE;
    }
    if (work->menuAction == 1) {
        if (func_02028754(work->perms, 0)) {
            work->result = 0xb;
            func_ov070_021b6100(work, FALSE);
            return TRUE;
        }
        GFL_MsgDataLoadStrbuf(work->msgData, 0x39, work->strbuf);
        copyVarForText(work->wordSet, 0, work->playerInfo);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021b6100(work, func_ov070_021b6ad8);
        return FALSE;
    }
    return FALSE;
}


BOOL func_ov070_021b6bc4(ItemMenuWork *work) {
    if (func_02028754(work->perms, TRUE)) {
        work->result = 7;
        func_ov070_021b6100(work, FALSE);
        return TRUE;
    }
    GFL_MsgDataLoadStrbuf(work->msgData, 0x3a, work->strbuf);
    copyVarForText(work->wordSet, 0, work->playerInfo);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021b6100(work, func_ov070_021b6ad8);
    return FALSE;
}

BOOL func_ov070_021b6c30(ItemMenuWork *work) {
    if (func_02028754(work->perms, 2)) {
        work->result = 8;
        func_ov070_021b6100(work, FALSE);
        return TRUE;
    }
    GFL_MsgDataLoadStrbuf(work->msgData, 0x3a, work->strbuf);
    copyVarForText(work->wordSet, 0, work->playerInfo);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021b6100(work, func_ov070_021b6ad8);
    return FALSE;
}

BOOL func_ov070_021b6c98(ItemMenuWork *work) {
    if (func_02028754(work->perms, 6)) {
        work->result = 0x11;
        func_ov070_021b6100(work, FALSE);
        return TRUE;
    }
    GFL_MsgDataLoadStrbuf(work->msgData, 0x3a, work->strbuf);
    copyVarForText(work->wordSet, 0, work->playerInfo);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021b6100(work, func_ov070_021b6ad8);
    return FALSE;
}

BOOL func_ov070_021b6d00(ItemMenuWork *work) {
    if (func_02028754(work->perms, 4)) {
        work->result = 0xf;
        func_ov070_021b6100(work, FALSE);
        func_ov070_021b7d20(work, TRUE);
        return TRUE;
    }
    GFL_MsgDataLoadStrbuf(work->msgData, 0x3a, work->strbuf);
    copyVarForText(work->wordSet, 0, work->playerInfo);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021b6100(work, func_ov070_021b6ad8);
    return FALSE;
}

BOOL func_ov070_021b6d70(ItemMenuWork *work) {
    if (func_02028754(work->perms, 5)) {
        work->result = 0x10;
        func_ov070_021b6100(work, FALSE);
        return TRUE;
    }
    GFL_MsgDataLoadStrbuf(work->msgData, 0x3a, work->strbuf);
    copyVarForText(work->wordSet, 0, work->playerInfo);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021b6100(work, func_ov070_021b6ad8);
    return FALSE;
}

BOOL func_ov070_021b6dd8(ItemMenuWork *work) {
    if (work->menuAction == 0) {
        if (func_02028754(work->perms, 9)) {
            work->result = 0x14;
            func_ov070_021b6100(work, FALSE);
            return TRUE;
        }
        GFL_MsgDataLoadStrbuf(work->msgData, 0x3a, work->strbuf);
        copyVarForText(work->wordSet, 0, work->playerInfo);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021b6100(work, func_ov070_021b6ad8);
        return FALSE;
    }
    return FALSE;
}

BOOL func_ov070_021b6e4c(ItemMenuWork *work) {
    if (work->menuAction == 0) {
        if (func_02028754(work->perms, 3)) {
            work->result = 0xe;
            func_ov070_021b6100(work, FALSE);
            func_ov070_021b7d20(work, TRUE);
            return TRUE;
        }
        GFL_MsgDataLoadStrbuf(work->msgData, 0x3a, work->strbuf);
        copyVarForText(work->wordSet, 0, work->playerInfo);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021b6100(work, func_ov070_021b6ad8);
        return FALSE;
    }
    return FALSE;
}

BOOL func_ov070_021b6bc4(ItemMenuWork *work);
BOOL func_ov070_021b6c30(ItemMenuWork *work);
BOOL func_ov070_021b6c98(ItemMenuWork *work);
BOOL func_ov070_021b6d00(ItemMenuWork *work);
BOOL func_ov070_021b6d70(ItemMenuWork *work);
BOOL func_ov070_021b6dd8(ItemMenuWork *work);
BOOL func_ov070_021b6e4c(ItemMenuWork *work);

static const ItemMenuFieldUse data_ov070_021bb9c4[] = {
    { ITEM_BICYCLE, 1, func_ov070_021b6af0 },
    { ITEM_TOWN_MAP, 1, func_ov070_021b6bc4 },
    { ITEM_PAL_PAD, 1, func_ov070_021b6c30 },
    { ITEM_HONEY, 1, func_ov070_021b6d00 },
    { ITEM_SUPER_ROD, 1, func_ov070_021b6d70 },
    { ITEM_VS_RECORDER, 1, func_ov070_021b6c98 },
    { ITEM_DOWSING_MCHN, 1, func_ov070_021b6dd8 },
    { ITEM_ESCAPE_ROPE, 1, func_ov070_021b6e4c },
};

// The buttons of the touch screen: the pockets, the arrows, sorting, registering, the exits, then the list's rows and
// their registration marks
static const TouchRect data_ov070_021bba04[] = {
#include "asset/itemmenu_data_ov070_021bba04.inc"
};

static const TouchRect data_ov070_021bb978[] = {
    { 96, 143, 128, 231 },
    { 168, 191, 224, 247 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

s32 func_ov070_021b6ec8(u16 item) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov070_021bb9c4); i++) {
        const ItemMenuFieldUse *use = &data_ov070_021bb9c4[i];

        if (item >= use->item && item < use->item + use->count) {
            return i;
        }
    }
    return -1;
}

void func_ov070_021b6ef8(ItemMenuWork *work) {
    BOOL done;
    u32 type;

    done = FALSE;
    if (!func_ov070_021badf0(work)) {
        return;
    }
    if (func_020279e8(work->taskMenu)) {
        work->menuAction = work->itemMenuActions[func_02027a04(work->taskMenu)];
        switch (work->menuAction) {
        case 0:
            func_ov070_021bb580(work, FALSE);
            if (work->pocket != BAG_POCKET_KEY_ITEMS) {
                func_02027324((u16)work->item);
            }
            if (work->pocket == BAG_POCKET_TMS_HMS) {
                func_ov070_021b6100(work, func_ov070_021b7640);
            } else if (GetItemParam((u16)work->item, 0x1d, work->heapId) == 1) {
                work->result = 12;
                func_ov070_021b6100(work, NULL);
            } else if (func_ov070_021b6ec8((u16)work->item) >= 0) {
                data_ov070_021bb9c4[func_ov070_021b6ec8((u16)work->item)].use(work);
            } else {
                type = func_ov070_021b690c((u16)work->item);
                if (type == 1) {
                    func_ov070_021b6100(work, func_ov070_021b6934);
                } else if (type == 2) {
                    func_ov070_021b6100(work, func_ov070_021b6a78);
                } else {
                    work->result = 5;
                    func_ov070_021b6100(work, NULL);
                }
            }
            break;
        case 1:
            func_ov070_021bb580(work, FALSE);
            if (func_02028754(work->perms, 0)) {
                work->result = 0xb;
                func_ov070_021b6100(work, NULL);
            } else {
                GFL_MsgDataLoadStrbuf(work->msgData, 0x39, work->strbuf);
                copyVarForText(work->wordSet, 0, work->playerInfo);
                GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
                func_ov070_021b6100(work, func_ov070_021b6ad8);
            }
            break;
        case 3:
            func_ov070_021b6100(work, func_ov070_021b7804);
            break;
        case 4:
        case 5:
            if (func_ov070_021b811c(work) == TRUE) {
                func_ov070_021b8134(work, work->cursorRow);
                func_ov070_021b6138(work);
                func_ov070_021bb3f4(work, TRUE);
                func_0204b92c(work->scrollBar, TRUE);
                func_ov070_021b8af4(work);
            }
            break;
        case 2:
            func_ov070_021bb580(work, FALSE);
            if (work->item >= 0x89 && work->item <= 0x94) {
                work->result = 0x13;
                func_ov070_021b6100(work, NULL);
            }
            break;
        case 7:
            break;
        case 8:
            if (func_020355c4() == TRUE) {
                func_ov070_021b9aac(work);
            }
            func_ov070_021b80c4(work, func_020355c4() ? FALSE : TRUE);
            func_ov070_021bb3f4(work, TRUE);
            func_0204b92c(work->scrollBar, TRUE);
            func_ov070_021b8af4(work);
            break;
        case 9:
            work->result = 1;
            func_ov070_021b6100(work, NULL);
            break;
        case 6:
            work->result = 2;
            func_ov070_021bb580(work, FALSE);
            func_ov070_021b6100(work, NULL);
            break;
        }
        done = TRUE;
    }
    if (done) {
        func_ov070_021ba9ac(work);
        func_02027858(work->taskMenu);
        work->taskMenu = NULL;
    }
}


void func_ov070_021b7128(ItemMenuWork *work) {
    BagItem *slot;

    slot = func_ov070_021b616c(work, func_ov070_021b61b8(work));
    work->item = ITEM_NONE;
    if (slot != NULL) {
        work->item = slot->item;
    }
    if (work->mode == 2) {
        if (GetItemParam((u16)work->item, ITEM_PARAM_IMPORTANT, work->heapId) == 0 && PML_ItemIsNotSpecialMonsball((u16)work->item) == TRUE) {
            if (PML_ItemIsMail((u16)work->item)) {
                work->result = 0x12;
            } else {
                work->result = 3;
            }
            func_ov070_021b6100(work, NULL);
            return;
        }
        GFL_MsgDataLoadStrbuf(work->msgData, 0x2f, work->strbuf);
        func_ov070_021b61f4(work, 0, work->item);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021badac(work);
        func_ov070_021bb3f4(work, FALSE);
        func_ov070_021bb580(work, FALSE);
        func_ov070_021b6100(work, func_ov070_021b7688);
    } else if (work->mode == 5) {
        work->result = 1;
        func_ov070_021b6100(work, NULL);
    } else if (work->mode == 4) {
        func_ov070_021b6100(work, func_ov070_021b795c);
    } else {
        func_ov070_021b832c(work);
        func_ov070_021badb8(work, work->item);
        func_ov070_021badac(work);
        func_0204b92c(work->scrollBar, FALSE);
        func_ov070_021b6100(work, func_ov070_021b6ef8);
    }
}

void func_ov070_021b7250(ItemMenuWork *work) {
    u32 keys;
    s32 pocket;
    s32 count;

    if (func_0201c6b4(work->printQueue) == FALSE) {
        return;
    }
    if (func_02035c38() == FALSE) {
        work->touchHeld = FALSE;
    }
    func_02051c88(work->buttonMan);
    func_0202ea40(work->paletteAnim);
    if (work->state != func_ov070_021b7250) {
        return;
    }
    if (func_ov070_021b63f8(work)) {
        work->drawnScroll = 0xffff;
        func_ov070_021b6138(work);
        return;
    }
    if (work->touchHeld == TRUE) {
        return;
    }
    if (func_020355c4() == TRUE) {
        keys = GCTX_HIDGetPressedKeys();
        if (keys == 0) {
            return;
        }
        count = func_ov070_021b61c8(work);
        if (count != 0) {
            if (!(keys & (PAD_BUTTON_B | PAD_BUTTON_START | PAD_BUTTON_X | PAD_BUTTON_Y))) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                func_ov070_021b9850(work);
                func_ov070_021b80c4(work, TRUE);
                return;
            }
        } else {
            func_ov070_021b80c4(work, TRUE);
            if (keys & (PAD_KEY_DOWN | PAD_KEY_UP | PAD_BUTTON_START | PAD_BUTTON_SELECT | PAD_BUTTON_A)) {
                return;
            }
        }
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        work->result = 1;
        work->item = ITEM_NONE;
        func_020355d8(FALSE);
        func_ov070_021b8c1c(work, 4, NULL);
        return;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        if (work->mode == 2) {
            return;
        }
        work->result = 0;
        work->item = ITEM_NONE;
        func_020355d8(FALSE);
        func_ov070_021b8c1c(work, 3, NULL);
        return;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
        if (func_ov070_021b811c(work) == TRUE) {
            if (work->pocket == BAG_POCKET_KEY_ITEMS) {
                func_ov070_021b8134(work, work->cursorRow);
                func_ov070_021b6138(work);
            } else {
                func_ov070_021b81b8(work);
            }
            if (func_020355c4() == TRUE) {
                func_ov070_021b9850(work);
                func_ov070_021b80c4(work, TRUE);
            }
        }
        return;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) {
        if (work->pocket != BAG_POCKET_TMS_HMS && work->pocket != 3) {
            count = func_ov070_021b61c8(work);
            if (count > 1) {
                func_ov070_021b8080(work);
                func_ov070_021b80c4(work, TRUE);
                work->drawnScroll = 0xffff;
                func_ov070_021b6138(work);
                func_ov070_021b6100(work, func_ov070_021b8ea8);
                return;
            }
        }
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        count = func_ov070_021b61c8(work);
        if (count > 0) {
            func_ov070_021b6100(work, func_ov070_021b8b7c);
            return;
        }
    } else {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
            if (work->pocket != BAG_POCKET_TMS_HMS && work->pocket != 3) {
                count = func_ov070_021b61c8(work);
                if (count > 1) {
                    MI_CpuFill8(work->items, 0, sizeof(work->items));
                    BagSave_CopyPocketRaw(work->bag, work->items, work->pocket, TRUE);
                    func_0204b878(work->listCursor, 2);
                    func_ov070_021bb5dc(work, FALSE);
                    work->movingItem = TRUE;
                    func_ov070_021b8b0c(work);
                }
            }
            return;
        }
        if (func_ov070_021b6554(work)) {
            func_ov070_021ba6a4(work);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov070_021b6138(work);
            return;
        }
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        pocket = work->pocket;
        work->pocket = pocket + 1;
        if (work->pocket >= 5) {
            work->pocket = 0;
        }
        func_020355d8(FALSE);
        func_ov070_021b8ce4(work, pocket, TRUE, func_ov070_021b8f90);
        return;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        pocket = work->pocket;
        work->pocket = pocket - 1;
        if (work->pocket < 0) {
            work->pocket = 4;
        }
        func_020355d8(FALSE);
        func_ov070_021b8ce4(work, pocket, FALSE, func_ov070_021b8f90);
    }
}



void func_ov070_021b7504(ItemMenuWork *work) {
    if (func_020279e8(work->taskMenu)) {
        if (func_02027a04(work->taskMenu) == FALSE) {
            func_ov070_021ba9ac(work);
            work->result = 6;
            func_ov070_021b6100(work, FALSE);
        } else {
            func_ov070_021ba9ac(work);
            GFL_BGSysClearScr(3);
            func_0204b92c(work->scrollBar, TRUE);
            func_ov070_021bb3f4(work, TRUE);
            if (func_020355c4() == TRUE) {
                func_ov070_021b9aac(work);
                func_ov070_021b80c4(work, FALSE);
            } else {
                func_ov070_021b80c4(work, TRUE);
            }
            func_ov070_021b8af4(work);
        }
        func_ov070_021bb1ac(work);
    }
}


void func_ov070_021b7584(ItemMenuWork *work) {
    if (func_ov070_021badf0(work)) {
        func_ov070_021bb0f8(work);
        func_ov070_021b6100(work, func_ov070_021b7504);
    }
}

void func_ov070_021b75a4(ItemMenuWork *work) {
    if (GFL_SndPlayerIsActiveAny() != TRUE && ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60())) {
        GFL_MsgDataLoadStrbuf(work->msgData, 0x3e, work->strbuf);
        loadMoveNameToStrbuf(work->wordSet, 0, PML_ItemGetTMWazaID((u16)work->item));
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021bacf0(work, TRUE);
        func_ov070_021b6100(work, func_ov070_021b7584);
    }
}










// pokebw2's ItemMenu_StateTMBootUpSound
void func_ov070_021b761c(ItemMenuWork *work) {
    if (func_ov070_021badf0(work)) {
        GFL_SndSEPlay(SEQ_SE_PC_LOGIN);
        func_ov070_021b6100(work, func_ov070_021b75a4);
    }
}

// Boots up a TM or HM
// pokebw2's ItemMenu_StateUseTM
void func_ov070_021b7640(ItemMenuWork *work) {
    if (PML_ItemGetHMID((u16)work->item) == 0xff) {
        GFL_MsgDataLoadStrbuf(work->msgData, 0x3c, work->expandBuf);
    } else {
        GFL_MsgDataLoadStrbuf(work->msgData, 0x3d, work->expandBuf);
    }
    func_ov070_021bacf0(work, TRUE);
    func_ov070_021b6100(work, func_ov070_021b761c);
}

// Waits for a message to be read, then goes back to the list
// pokebw2's ItemMenu_StateReturnToList
void func_ov070_021b7688(ItemMenuWork *work) {
    if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60()) {
        GFL_BGSysClearScr(3);
        work->drawnScroll = 0xffff;
        func_ov070_021b6138(work);
        work->listDirty = TRUE;
        func_ov070_021ba6a4(work);
        func_0204b92c(work->scrollBar, TRUE);
        func_ov070_021bb3f4(work, TRUE);
        func_ov070_021b8af4(work);
    }
}

// pokebw2's ItemMenu_StateWaitTossMessage
void func_ov070_021b76e4(ItemMenuWork *work) {
    if (func_ov070_021badf0(work)) {
        func_ov070_021b6100(work, func_ov070_021b7688);
    }
}

// Whether to toss the items
// pokebw2's ItemMenu_StateTossConfirm
static void func_ov070_021b7700(ItemMenuWork *work) {
    u32 pos;

    if (func_020279e8(work->taskMenu)) {
        pos = func_02027a04(work->taskMenu);
        func_ov070_021bb1ac(work);
        GFL_BGSysClearScr(3);
        if (pos == 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_08);
            func_ov070_021b7d20(work, work->quantity);
            GFL_MsgDataLoadStrbuf(work->msgData, BtlMain_Text_054, work->strbuf);
            func_ov070_021b6204(work, 0, work->item, work->quantity > 1, FALSE);
            WordSetNumber(work->wordSet, 1, work->quantity, 3, 0, TRUE);
            GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
            func_ov070_021badac(work);
            func_ov070_021b6100(work, func_ov070_021b76e4);
        } else {
            func_0204b92c(work->scrollBar, TRUE);
            func_ov070_021bb3f4(work, TRUE);
            func_ov070_021b80c4(work, func_020355c4() ? FALSE : TRUE);
            func_ov070_021b8af4(work);
        }
    }
}

// pokebw2's ItemMenu_StateTossAsk
void func_ov070_021b77e4(ItemMenuWork *work) {
    if (func_ov070_021badf0(work)) {
        func_ov070_021bb0f8(work);
        func_ov070_021b6100(work, func_ov070_021b7700);
    }
}

// Tossing an item: asks how many
// pokebw2's ItemMenu_StateToss
void func_ov070_021b7804(ItemMenuWork *work) {
    work->quantity = 1;
    func_ov070_021b7d9c(work, 1);
    GFL_MsgDataLoadStrbuf(work->msgData, 0x35, work->strbuf);
    func_ov070_021b6204(work, 0, work->item, TRUE, FALSE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021badac(work);
    func_ov070_021b6100(work, func_ov070_021b7868);
}

// The number of items to toss
// pokebw2's ItemMenu_StateTossQuantity
void func_ov070_021b7868(ItemMenuWork *work) {
    s32 result;

    if (func_ov070_021badf0(work) && func_ov070_021b7df4(work) != TRUE) {
        result = func_ov070_021b8dac();
        if (result == -1) {
            if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
                result = 0;
                func_020355d8(FALSE);
            } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
                func_020355d8(FALSE);
                result = 1;
            }
        }
        if (result == 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov070_021b7dd0(work);
            GFL_MsgDataLoadStrbuf(work->msgData, 0x37, work->strbuf);
            func_ov070_021b6204(work, 0, work->item, work->quantity > 1, FALSE);
            WordSetNumber(work->wordSet, 1, work->quantity, 3, 0, TRUE);
            GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
            func_ov070_021badac(work);
            func_ov070_021b6100(work, func_ov070_021b77e4);
        } else if (result == 1) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov070_021b8c1c(work, 4, func_ov070_021b8dcc);
        }
    }
}

void func_ov070_021b795c(ItemMenuWork *work) {
    s32 price;
    s32 important;

    func_0204b92c(work->scrollBar, FALSE);
    price = GetItemParam((u16)work->item, ITEM_PARAM_PRICE, work->heapId);
    important = GetItemParam((u16)work->item, ITEM_PARAM_IMPORTANT, work->heapId);
    if (price == 0 || important != 0) {
        func_ov070_021bb3f4(work, FALSE);
        GFL_MsgDataLoadStrbuf(work->msgData, 0x4d, work->strbuf);
        func_ov070_021b61f4(work, 0, work->item);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021badac(work);
        func_ov070_021bb580(work, FALSE);
        func_ov070_021b6100(work, func_ov070_021b7c88);
    } else {
        work->quantity = 1;
        func_ov070_021bb08c(work);
        func_ov070_021bb3f4(work, FALSE);
        if (func_ov070_021b616c(work, func_ov070_021b61b8(work))->count == 1) {
            func_ov070_021b6100(work, func_ov070_021b7b08);
            return;
        }
        func_ov070_021b7d9c(work, 2);
        GFL_MsgDataLoadStrbuf(work->msgData, 0x4e, work->strbuf);
        func_ov070_021b61f4(work, 0, work->item);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021badac(work);
        func_ov070_021b6100(work, func_ov070_021b7a7c);
    }
}


// The number of items to sell
// pokebw2's ItemMenu_StateSellQuantity
void func_ov070_021b7a7c(ItemMenuWork *work) {
    s32 result;

    if (func_ov070_021badf0(work) && func_ov070_021b7df4(work) != TRUE) {
        result = func_ov070_021b8dac();
        if (result == -1) {
            if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
                result = 0;
                func_020355d8(FALSE);
            } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
                func_020355d8(FALSE);
                result = 1;
            }
        }
        if (result == 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov070_021b7dd0(work);
            func_ov070_021b6100(work, func_ov070_021b7b08);
        } else if (result == 1) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov070_021b8c1c(work, 4, func_ov070_021b8e08);
        }
    }
}

// Asks whether to sell the items for their price
// pokebw2's ItemMenu_StateSellAsk
void func_ov070_021b7b08(ItemMenuWork *work) {
    s32 price;

    func_ov070_021bb0f8(work);
    price = func_ov070_021b619c(work->item, work->quantity, work->heapId);
    GFL_MsgDataLoadStrbuf(work->msgData, BtlMain_Text_079, work->strbuf);
    WordSetNumber(work->wordSet, 0, price, 7, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021badac(work);
    func_ov070_021b6100(work, func_ov070_021b7b80);
}

// Whether to sell the items
// pokebw2's ItemMenu_StateSellConfirm
static void func_ov070_021b7b80(ItemMenuWork *work) {
    u32 pos;
    s32 price;

    if (func_ov070_021badf0(work) && func_020279e8(work->taskMenu)) {
        pos = func_02027a04(work->taskMenu);
        func_ov070_021bb1ac(work);
        switch (pos) {
        case 0:
            price = func_ov070_021b619c(work->item, work->quantity, work->heapId);
            func_ov070_021b7d20(work, work->quantity);
            addCashToTotal(getTrainerCardDataBlkAddress(work->gameData), price);
            GFL_SndSEPlay(SEQ_SE_SYS_22);
            func_ov070_021bafc4(work);
            GFL_MsgDataLoadStrbuf(work->msgData, BtlMain_Text_080, work->strbuf);
            func_ov070_021b6204(work, 0, work->item, work->quantity > 1, FALSE);
            WordSetNumber(work->wordSet, 1, price, 7, 0, TRUE);
            GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
            func_ov070_021badac(work);
            func_ov070_021b6100(work, func_ov070_021b7c88);
            break;
        case 1:
            func_ov070_021b80c4(work, func_020355c4() ? FALSE : TRUE);
            func_ov070_021b6100(work, func_ov070_021b7cd0);
            break;
        }
    }
}

// pokebw2's ItemMenu_StateSellWaitMessage
static void func_ov070_021b7c88(ItemMenuWork *work) {
    if (func_ov070_021badf0(work)) {
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            func_ov070_021b80c4(work, TRUE);
            func_ov070_021b6100(work, func_ov070_021b7cd0);
        } else if (func_02035c60()) {
            func_ov070_021b80c4(work, FALSE);
            func_ov070_021b6100(work, func_ov070_021b7cd0);
        }
    }
}

void func_ov070_021b7cd0(ItemMenuWork *work) {
    GFL_BGSysClearScr(3);
    work->drawnScroll = 0xffff;
    func_ov070_021b6138(work);
    work->listDirty = TRUE;
    func_ov070_021ba6a4(work);
    func_ov070_021bb0bc(work);
    func_0204b92c(work->scrollBar, TRUE);
    func_ov070_021bb3f4(work, TRUE);
    func_ov070_021b8af4(work);
}


void func_ov070_021b7d20(ItemMenuWork *work, u32 count) {
    s32 listCount;
    s32 row;
    s32 scroll;

    func_ov070_021b616c(work, func_ov070_021b61b8(work));
    BagSave_SubItem(work->bag, work->item, count, work->heapId);
    if (BagSave_GetItemCountByID(work->bag, work->item, work->heapId) == 0) {
        listCount = func_ov070_021b61c8(work);
        if (listCount + 1 < ITEMMENU_LIST_ROWS + 1) {
            row = work->cursorRow - 1;
            if (row <= 0) {
                row = 0;
            }
            work->cursorRow = row;
        } else {
            scroll = work->scroll - 1;
            if (scroll < -1) {
                scroll = -1;
            }
            work->scroll = scroll;
        }
    }
}


void func_ov070_021b7d9c(ItemMenuWork *work, u32 mode) {
    work->quantityMode = mode;
    func_ov070_021bb1c4(work);
    func_ov070_021bb260(work, work->quantity);
    func_0204b3dc(work->buttons[5], TRUE);
    func_0204b3dc(work->buttons[6], TRUE);
}

void func_ov070_021b7dd0(ItemMenuWork *work) {
    GFL_BGSysClearScr(3);
    func_0204b3dc(work->buttons[5], FALSE);
    func_0204b3dc(work->buttons[6], FALSE);
}

BOOL func_ov070_021b7df4(ItemMenuWork *work) {
    s32 old;
    BOOL changed;
    BagItem *slot;
    s32 hit;
    BOOL down;

    old = work->quantity;
    changed = FALSE;
    slot = func_ov070_021b616c(work, func_ov070_021b61b8(work));
    {
        TouchRect rects[] = {
            { 92, 116, 234, 0 },
            { 116, 140, 234, 0 },
            { TOUCH_RECT_END, 0, 0, 0 },
        };

        hit = func_02035ba4(rects);
    }
    if (hit != TOUCH_RECT_NONE) {
        work->quantityRepeat++;
        changed = TRUE;
    } else {
        work->quantityRepeat = 0;
    }
    if (work->quantityRepeat == 1 || work->quantityRepeat > 30) {
        switch (hit) {
        case 0:
            if (work->quantityRepeat > 120) {
                work->quantity += 10;
            } else {
                work->quantity += 1;
            }
            down = FALSE;
            break;
        case 1:
            if (work->quantityRepeat > 120) {
                work->quantity -= 10;
            } else {
                work->quantity -= 1;
            }
            down = TRUE;
            break;
        }
    }
    if (hit == TOUCH_RECT_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
            work->quantity += 1;
            down = FALSE;
            changed = TRUE;
        } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
            work->quantity -= 1;
            down = TRUE;
            changed = TRUE;
        } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
            work->quantity += 10;
            down = FALSE;
            changed = TRUE;
        } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
            work->quantity -= 10;
            down = TRUE;
            changed = TRUE;
        }
    }
    if (slot->count < work->quantity) {
        work->quantity = 1;
    } else if (work->quantity < 1) {
        work->quantity = slot->count;
    }
    if (work->quantity != old) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        if (down == FALSE) {
            func_0204b878(work->buttons[5], 11);
        } else {
            func_0204b878(work->buttons[6], 10);
        }
        func_ov070_021bb260(work, work->quantity);
    }
    return changed;
}


s32 func_ov070_021b7f38(void *a, void *b) {
    ItemMenuSortEntry *entryA = a;
    ItemMenuSortEntry *entryB = b;

    if (entryA->key == entryB->key) {
        return 0;
    }
    if (entryA->key > entryB->key) {
        return 1;
    }
    return -1;
}

void func_ov070_021b7f54(ItemMenuWork *work) {
    s32 i;
    BagItem *items;
    ItemMenuSortEntry *sort;
    s32 count;
    ArcTool *arc;
    ItemData *data;

    sort = GFL_HeapAllocate(work->heapId, sizeof(ItemMenuSortEntry) * NELEMS(work->items), FALSE, "itemmenu.c", 2869);
    items = GFL_HeapAllocate(work->heapId, sizeof(BagItem) * NELEMS(work->items), FALSE, "itemmenu.c", 2870);
    BagSave_CopyPocketRaw(work->bag, items, work->pocket, TRUE);
    count = func_ov070_021b61c8(work);
    arc = PML_ItemArcHandleCreate(work->heapId);
    for (i = 0; i < count; i++) {
        data = PML_ItemArcHandleReadFile(arc, items[i].item, work->heapId);
        sort[i].key = (PML_ItemGetParam(data, ITEM_PARAM_KIND) << 28) + (PML_ItemGetParam(data, ITEM_PARAM_SORT_INDEX) << 16) + items[i].item;
        sort[i].item = items[i];
        GFL_HeapFree(data);
    }
    GFL_ArcToolFree(arc);
    MATH_QSort(sort, count, sizeof(ItemMenuSortEntry), func_ov070_021b7f38, NULL);
    for (i = 0; i < count; i++) {
        items[i] = sort[i].item;
    }
    BagSave_CopyPocketRaw(work->bag, items, work->pocket, FALSE);
    GFL_HeapFree(items);
    GFL_HeapFree(sort);
}





void func_ov070_021b8080(ItemMenuWork *work) {
    GFL_SndSEPlay(0x663);
    func_0204b878(work->sortButton, 2);
}

void func_ov070_021b809c(ItemMenuWork *work) {
    func_ov070_021b7f54(work);
    work->drawnScroll = 0xffff;
    func_ov070_021b6138(work);
}



void func_ov070_021b80bc(ItemMenuWork *work) {
    func_ov070_021bb5a4(work);
}


void func_ov070_021b80c4(ItemMenuWork *work, BOOL keys) {
    if (work->movingItem == TRUE) {
        keys = TRUE;
    }
    if (keys) {
        func_020355d8(FALSE);
    } else {
        func_020355d8(TRUE);
    }
    if (func_ov070_021b61c8(work) == 0) {
        keys = FALSE;
    }
    if (keys == FALSE) {
        func_ov070_021bb398(work, FALSE);
    } else {
        func_ov070_021bb398(work, TRUE);
        func_0202eb04(work->paletteAnim);
    }
    func_ov070_021b97e8(work, keys);
}

// Sorts the pocket
// Changes the Free Space's filter
// Whether the bag's mode lets items be registered
// pokebw2's ItemMenu_CanRegister
BOOL func_ov070_021b811c(ItemMenuWork *work) {
    if (work->mode == 0 || work->mode == 1 || work->mode == 3) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov070_021b8134(ItemMenuWork *work, s32 row) {
    BagItem *slot;
    u32 shortcut;
    s32 count;

    row += work->scroll + 1;
    if (work->pocket == BAG_POCKET_KEY_ITEMS) {
        count = func_ov070_021b61c8(work);
        if (count <= row) {
            return FALSE;
        }
        slot = func_ov070_021b616c(work, row);
        shortcut = func_0202e448(slot->item);
        if (shortcut == SHORTCUT_NONE) {
            return FALSE;
        }
        if (func_ov070_021b83a0(work, slot->item) == TRUE) {
            GameData_SetKeyItemRegistration(work->gameData, shortcut, FALSE);
        } else {
            GameData_SetKeyItemRegistration(work->gameData, shortcut, TRUE);
        }
        GFL_SndSEPlay(SEQ_SE_SYS_07);
        work->drawnScroll = 0xffff;
        return TRUE;
    }
    return FALSE;
}


// pokebw2's ItemMenu_GetPocketShortcut
u32 func_ov070_021b81b0(s32 pocket) {
    return func_ov070_021bb368(pocket);
}

// Registers the pocket for the Y button, or unregisters it
// pokebw2's ItemMenu_TogglePocketRegistration
void func_ov070_021b81b8(ItemMenuWork *work) {
    u32 shortcut = func_ov070_021b81b0(work->pocket);

    GameData_SetKeyItemRegistration(work->gameData, shortcut, GameData_IsShortcutRegistered(work->gameData, shortcut) ^ TRUE);
    GFL_SndSEPlay(SEQ_SE_SYS_07);
    func_ov070_021b81f4(work);
}

// Shows whether the pocket is registered
// pokebw2's ItemMenu_UpdatePocketRegistration
static void func_ov070_021b81f4(ItemMenuWork *work) {
    if (work->movingItem != TRUE) {
        func_0204b878(work->buttons[2], GameData_IsShortcutRegistered(work->gameData, func_ov070_021b81b0(work->pocket)) + 6);
    }
}

// What the item menu's first action is: use, ride or read
// pokebw2's ItemMenu_SetUseAction
void func_ov070_021b822c(ItemMenuWork *work, ItemData *data, BagItem *slot, u8 *actions) {
    if (PML_ItemGetParam(data, ITEM_PARAM_FIELD_FUNC)) {
        if (PML_ItemIsMail(slot->item) == TRUE) {
            actions[0] = 2;
        } else if (slot->item == ITEM_BICYCLE && work->isCycling == TRUE) {
            actions[0] = 1;
        } else {
            actions[0] = 0;
        }
    }
    if (slot->item == ITEM_LIGHT_STONE || slot->item == ITEM_DARK_STONE) {
        actions[0] = 0;
    }
}

void func_ov070_021b8280(ItemMenuWork *work, u8 *actions) {
    BagItem *slot;
    ItemData *data;
    u8 fieldPocket;

    slot = func_ov070_021b616c(work, func_ov070_021b61b8(work));
    if (slot != NULL && slot->item != ITEM_NONE) {
        data = PML_ItemReadDataFile(slot->item, 0, work->heapId);
        fieldPocket = work->pocket;
        func_ov070_021b822c(work, data, slot, actions);
        if (func_ov070_021b8f68(work, fieldPocket) == FALSE && actions[0] != 2) {
            actions[0] = 0xff;
        }
        if (PML_ItemGetParam(data, ITEM_PARAM_IMPORTANT) == 0) {
            if (PML_ItemIsNotSpecialMonsball(slot->item) == TRUE) {
                actions[1] = 6;
            }
            if (fieldPocket != BAG_POCKET_TMS_HMS) {
                actions[2] = 3;
            }
        }
        if (PML_ItemGetParam(data, ITEM_PARAM_REGISTRABLE)) {
            if (func_ov070_021b83a0(work, slot->item) == TRUE) {
                actions[2] = 5;
            } else {
                actions[2] = 4;
            }
        }
        actions[3] = 8;
        GFL_HeapFree(data);
    }
}

void func_ov070_021b832c(ItemMenuWork *work) {
    u32 msgIds[11] = { 0, 6, 16, 1, 2, 18, 3, 5, 8, 87, 0 };
    u32 menuMsgIds[4];
    u8 actions[4] = { 0xff, 0xff, 0xff, 0xff };
    s32 i;
    s32 count;

    func_ov070_021b8280(work, actions);
    for (i = 0, count = 0; i < 4; i++) {
        if (actions[i] != 0xff) {
            work->itemMenuActions[count] = actions[i];
            menuMsgIds[count] = msgIds[actions[i]];
            count++;
        }
    }
    func_ov070_021ba9c0(work, menuMsgIds, count);
}



// pokebw2's ItemMenu_IsItemRegistered
BOOL func_ov070_021b83a0(ItemMenuWork *work, u32 item) {
    return GameData_IsShortcutRegistered(work->gameData, func_0202e448(item));
}

void func_ov070_021b83b8(u32 button, u32 event, void *data) {
    ItemMenuWork *work = data;
    s32 newPocket;
    s32 count;
    s32 pocket;
    s32 row;
    s32 from;
    s32 to;

    newPocket = -1;
    count = func_ov070_021b61c8(work);
    if (event == 0) {
        if (button < 5) {
            s32 pockets[] = { BAG_POCKET_MEDICINE, BAG_POCKET_TMS_HMS, BAG_POCKET_BERRIES, BAG_POCKET_KEY_ITEMS, BAG_POCKET_ITEMS };

            if (work->pocket != pockets[button]) {
                newPocket = pockets[button];
                func_ov070_021b80c4(work, FALSE);
            }
        } else if (button == 5) {
            func_ov070_021b80c4(work, FALSE);
            pocket = work->pocket;
            work->pocket = pocket - 1;
            if (work->pocket < 0) {
                work->pocket = 4;
            }
            func_ov070_021b8ce4(work, pocket, FALSE, func_ov070_021b8f90);
            return;
        } else if (button == 6) {
            func_ov070_021b80c4(work, FALSE);
            pocket = work->pocket;
            work->pocket = pocket + 1;
            if (work->pocket >= 5) {
                work->pocket = 0;
            }
            func_ov070_021b8ce4(work, pocket, TRUE, func_ov070_021b8f90);
            return;
        } else if (button == 7) {
            if (work->pocket != BAG_POCKET_TMS_HMS && work->pocket != BAG_POCKET_BERRIES) {
                count = func_ov070_021b61c8(work);
                if (count > 1) {
                    func_ov070_021b8080(work);
                    func_ov070_021b80c4(work, FALSE);
                    work->drawnScroll = 0xffff;
                    func_ov070_021b6138(work);
                    func_ov070_021b6100(work, func_ov070_021b8ea8);
                }
            }
        } else if (button == 8) {
            if (func_ov070_021b811c(work) == TRUE) {
                func_ov070_021b81b8(work);
                func_ov070_021b80c4(work, FALSE);
                work->drawnScroll = 0xffff;
                func_ov070_021b6138(work);
            }
        } else if (button == 9) {
            if (work->mode != 2) {
                work->result = 0;
                work->item = ITEM_NONE;
                func_ov070_021b8c1c(work, 3, NULL);
                func_020355d8(TRUE);
            }
        } else if (button == 10) {
            work->result = 1;
            work->item = ITEM_NONE;
            func_ov070_021b8c1c(work, 4, NULL);
            func_020355d8(TRUE);
        } else if (button >= 11 && button < 17) {
            if (work->state != func_ov070_021b7250) {
                return;
            }
            row = work->cursorRow;
            from = func_ov070_021b61b8(work);
            if (func_ov070_021b61c8(work) == 0) {
                return;
            }
            if (work->cursorRow != button - 11) {
                work->cursorRow = button - 11;
                if (count <= func_ov070_021b61b8(work)) {
                    work->cursorRow = row;
                    return;
                }
            }
            to = func_ov070_021b61b8(work);
            func_ov070_021b624c(work, from, to);
            func_ov070_021b80c4(work, FALSE);
            work->drawnScroll = 0xffff;
            func_ov070_021b6138(work);
            func_ov070_021b9850(work);
            func_ov070_021b6100(work, func_ov070_021b8b7c);
            return;
        } else if (button >= 17) {
            if (func_ov070_021b811c(work) == TRUE) {
                if (func_ov070_021b8134(work, button - 17) == TRUE) {
                    func_ov070_021b80c4(work, FALSE);
                    func_ov070_021b6138(work);
                }
            }
        }
        if (newPocket != -1) {
            pocket = work->pocket;
            work->pocket = newPocket;
            GFL_SndSEPlay(SEQ_SE_SYS_99);
            func_ov070_021b6288(work, pocket, work->pocket);
            func_ov070_021b80bc(work);
            func_ov070_021b80c4(work, FALSE);
            work->drawnScroll = 0xffff;
            func_ov070_021b6138(work);
        }
    }
}


// pokebw2's ItemMenu_VBlank
void func_ov070_021b8600(TCB *tcb, void *data) {
    ItemMenuWork *work = data;

    if (work->tmInfoRequest == 1) {
        func_ov070_021b97cc(work, TRUE);
        work->tmInfoRequest = 0;
    } else if (work->tmInfoRequest == 2) {
        func_ov070_021b97cc(work, FALSE);
        work->tmInfoRequest = 0;
    }
    func_02021d50(work->paletteFade);
    func_0204a648();
}

// Fades the bag in
// pokebw2's ItemMenu_StateFadeIn
void func_ov070_021b8640(ItemMenuWork *work) {
    if (func_0201c6b4(work->printQueue)) {
        GFL_WipeSet(0, 1, 1, 0, 6, 1, work->heapId);
        GFL_SndSEPlay(SEQ_SE_SYS_90);
        func_ov070_021b6100(work, func_ov070_021b8688);
    }
}

// pokebw2's ItemMenu_StateWaitFadeIn
static void func_ov070_021b8688(ItemMenuWork *work) {
    BOOL finished = GFL_WipeIsFinished();
    BOOL busy = func_ov070_021bb690(work);

    if (finished == TRUE && busy == FALSE) {
        func_0204b92c(work->scrollBar, TRUE);
        func_ov070_021b8af4(work);
    }
}

BOOL func_ov070_021b86b8(GameProc *proc, u32 *state, void *param, void *data) {
    BagProcessData *bag = param;
    ItemMenuWork *work;
    u16 pocket;
    s16 row;
    s16 scroll;
    u16 count;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_BAG, 0x34000);
    work = GFL_ProcInitSubsystem(proc, sizeof(ItemMenuWork), HEAPID_BAG);
    MI_CpuFill8(work, 0, sizeof(ItemMenuWork));
    func_02036370(&work->savedRepeatWait, &work->savedRepeatStart);
    work->heapId = HEAPID_BAG;
    work->buttonsActive = TRUE;
    work->gameData = bag->gameData;
    work->playerInfo = bag->playerInfo;
    work->trainerData = bag->trainerData;
    work->cursor = bag->cursor;
    work->bag = bag->bag;
    work->perms = &bag->perms;
    work->mode = bag->mode;
    work->isCycling = bag->isCycling;
    sortItemBlocks(work->bag);
    work->pocket = func_02008284(work->cursor);
    for (pocket = 0; pocket < 5; pocket++) {
        func_02008270(work->cursor, pocket, &row, &scroll);
        count = BagSave_GetPocketItemCount(work->bag, pocket);
        while (TRUE) {
            if (row == 0 && scroll == 0) {
                break;
            }
            if (scroll + ITEMMENU_LIST_ROWS - 1 >= count && scroll != 0) {
                scroll--;
            } else if (row + scroll >= count) {
                if (scroll != 0) {
                    scroll--;
                } else {
                    row--;
                }
            } else {
                break;
            }
        }
        if (pocket == work->pocket) {
            work->cursorRow = row;
            work->scroll = scroll - 1;
        }
        func_02008288(work->cursor, pocket, row, scroll);
    }
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN, work->heapId);
    work->strbuf = GFL_StrBufCreate(200, work->heapId);
    work->expandBuf = GFL_StrBufCreate(200, work->heapId);
    work->tempBuf = GFL_StrBufCreate(0x40, work->heapId);
    work->wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    work->printQueue = func_0201c450(0x1000, work->heapId);
    work->keyCursor = func_02028590(0xf, TRUE, TRUE, work->heapId);
    work->font = GFL_FontCreate(23, 0, 0, FALSE, work->heapId);
    func_ov070_021b9280(work);
    func_ov070_021b9bdc(work);
    func_ov070_021b9ef4(work);
    func_ov070_021ba8c0(work);
    func_ov070_021b6120(work);
    func_ov070_021ba028(work);
    work->buttonMan = func_02051b84(data_ov070_021bba04, func_ov070_021b83b8, work, work->heapId);
    func_ov070_021b8b24(work);
    func_ov070_021b8e48(work);
    work->vblankTask = GFL_VBlankTCBAdd(func_ov070_021b8600, work, 0);
    work->tcbManager = GFL_TCBExMgrCreate(work->heapId, work->heapId, 1, 0);
    func_ov070_021bae98(work);
    work->taskMenuRes = func_02027f60(3, 9, work->font, work->printQueue, work->heapId);
    func_0203d34c(TRUE, work->heapId);
    func_ov070_021b80c4(work, func_020355c4() ? FALSE : TRUE);
    func_ov070_021b913c();
    func_ov070_021b6100(work, func_ov070_021b8640);
    return TRUE;
}


BOOL func_ov070_021b8904(GameProc *proc, u32 *state, void *param, void *data) {
    ItemMenuWork *work = data;

    if (work->state == NULL) {
        GFL_WipeSet(0, 0, 0, 0, 6, 1, work->heapId);
        return TRUE;
    }
    work->drawnScroll = work->scroll;
    work->state(work);
    if (work->listDirty) {
        func_ov070_021ba41c(work);
        work->listDirty = FALSE;
    }
    if (work->iconsDirty) {
        func_ov070_021ba4a4(work);
        work->iconsDirty = FALSE;
    }
    func_ov070_021b9ed8(work);
    func_ov070_021bb73c(work);
    GFL_TCBExMgrUpdate(work->tcbManager);
    return FALSE;
}

BOOL func_ov070_021b8990(GameProc *proc, u32 *state, void *param, void *data) {
    BagProcessData *bag = param;
    ItemMenuWork *work = data;

    if (GFL_WipeIsFinished() != TRUE) {
        return FALSE;
    }
    if (func_0201c6b4(work->printQueue) == FALSE) {
        return FALSE;
    }
    GFL_TCBRemove(work->vblankTask);
    func_02008298(work->cursor, (u16)work->pocket);
    func_02008288(work->cursor, (u16)work->pocket, (s16)work->cursorRow, (s16)(work->scroll + 1));
    func_02027fd8(work->taskMenuRes);
    func_ov070_021b9af8(work);
    func_ov070_021b9758(work);
    GFL_TCBExMgrFree(work->tcbManager);
    GFL_MsgDataFree(work->msgData);
    GFL_StrBufFree(work->strbuf);
    GFL_StrBufFree(work->expandBuf);
    GFL_StrBufFree(work->tempBuf);
    GFL_WordSetSystemFree(work->wordSet);
    func_ov070_021baf10(work);
    if (work->msgWindow.window != NULL) {
        GFL_BGSysFreeCharMemory(3, CHAR_POS(work->cursorImageChars), CHAR_SIZE(work->cursorImageChars));
        BmpWin_Free(work->msgWindow.window);
    }
    GFL_BGSysFreeFilledChar(3, 1, 0);
    GFL_FontFree(work->font);
    func_02028604(work->keyCursor);
    func_0201c6ec(work->printQueue);
    func_0201c4c0(work->printQueue);
    func_ov070_021b8e84(work);
    func_ov070_021b8b6c(work);
    func_02051c6c(work->buttonMan);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    bag->result = work->result;
    bag->item = work->item;
    setKeypressFramecounts(work->savedRepeatWait, work->savedRepeatStart);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_BAG);
    return TRUE;
}



// pokebw2's ItemMenu_ReturnToList
static void func_ov070_021b8af4(ItemMenuWork *work) {
    func_ov070_021b6100(work, func_ov070_021b7250);
    setKeypressFramecounts(3, 6);
}

// pokebw2's ItemMenu_StartMoveItem
void func_ov070_021b8b0c(ItemMenuWork *work) {
    func_ov070_021b6100(work, func_ov070_021b66fc);
    setKeypressFramecounts(3, 6);
}

void func_ov070_021b8b24(ItemMenuWork *work) {
    ArcTool *arc;

    work->paletteAnim = func_0202e940(0x10, 0x10, 0xfffe, work->heapId);
    arc = GFL_ArcSysCreateFileHandle(0x58, work->heapId);
    func_0202e9ac(work->paletteAnim, arc, 0x15, 0x10, 0x20);
    GFL_ArcToolFree(arc);
}


// pokebw2's ItemMenu_FreePaletteAnim
static void func_ov070_021b8b6c(ItemMenuWork *work) {
    func_0202ea20(work->paletteAnim);
}

// Flashes the cursor on the picked item, then opens the item's menu
// pokebw2's ItemMenu_StateFlashCursor
void func_ov070_021b8b7c(ItemMenuWork *work) {
    switch (work->buttonAnim) {
    case 0:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
    case 3:
        func_ov070_021bb398(work, 0);
        work->buttonAnim++;
        break;
    case 1:
    case 4:
    case 6:
        work->buttonAnimTimer++;
        if (work->buttonAnimTimer == 4) {
            work->buttonAnimTimer = 0;
            work->buttonAnim++;
        }
        break;
    case 2:
    case 5:
        func_ov070_021bb398(work, 2);
        work->buttonAnim++;
        break;
    case 7:
        work->buttonAnim = 0;
        func_ov070_021bb398(work, 1);
        func_0202eb04(work->paletteAnim);
        func_0202ea40(work->paletteAnim);
        func_ov070_021b6100(work, func_ov070_021b7128);
        break;
    }
}

void func_ov070_021b8c1c(ItemMenuWork *work, u32 button, ItemMenuState next) {
    u32 anim;

    if (button == 4) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        anim = 9;
    } else if (button == 3) {
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        anim = 8;
    } else if (button == 1) {
        GFL_SndSEPlay(SEQ_SE_SYS_99);
        anim = 13;
    } else if (button == 0) {
        GFL_SndSEPlay(SEQ_SE_SYS_99);
        anim = 12;
    }
    work->buttonAnim = button;
    func_0204b8c4(work->buttons[work->buttonAnim], 0);
    func_0204b878(work->buttons[work->buttonAnim], (u16)anim);
    func_0204b92c(work->buttons[work->buttonAnim], TRUE);
    work->buttonAnimNext = next;
    func_ov070_021b6100(work, func_ov070_021b8cb4);
}


// pokebw2's ItemMenu_StateWaitButton
void func_ov070_021b8cb4(ItemMenuWork *work) {
    if (func_0204b98c(work->buttons[work->buttonAnim]) == FALSE) {
        work->buttonAnim = 0;
        func_ov070_021b6100(work, work->buttonAnimNext);
    }
}

void func_ov070_021b8ce4(ItemMenuWork *work, u32 oldPocket, BOOL right, ItemMenuState next) {
    u32 anim;

    func_ov070_021b6288(work, oldPocket, work->pocket);
    func_ov070_021b80bc(work);
    func_ov070_021b81f4(work);
    work->drawnScroll = 0xffff;
    func_ov070_021b6138(work);
    if (right == TRUE) {
        GFL_SndSEPlay(SEQ_SE_SYS_99);
        anim = 13;
    } else if (right == FALSE) {
        GFL_SndSEPlay(SEQ_SE_SYS_99);
        anim = 12;
    }
    work->buttonAnim = right;
    work->buttonAnimTimer = 6;
    func_0204b8c4(work->buttons[work->buttonAnim], 0);
    func_0204b878(work->buttons[work->buttonAnim], (u16)anim);
    func_0204b92c(work->buttons[work->buttonAnim], TRUE);
    work->buttonAnimNext = next;
    func_ov070_021b6100(work, func_ov070_021b8d88);
}


// pokebw2's ItemMenu_StateWaitPocketArrow
void func_ov070_021b8d88(ItemMenuWork *work) {
    if (work->buttonAnimTimer == 0) {
        work->buttonAnim = 0;
        func_ov070_021b6100(work, work->buttonAnimNext);
    } else {
        work->buttonAnimTimer--;
    }
}

// The quantity's confirm and cancel buttons
// The quantity's buttons: confirm and cancel
// pokebw2's ItemMenu_GetQuantityButton
static s32 func_ov070_021b8dac(void) {
    s32 hit = func_02035c08(data_ov070_021bb978);

    if (hit != TOUCH_RECT_NONE) {
        func_020355d8(TRUE);
    }
    return hit;
}

void func_ov070_021b8dcc(ItemMenuWork *work) {
    func_ov070_021b7dd0(work);
    func_0204b92c(work->scrollBar, TRUE);
    func_ov070_021bb3f4(work, TRUE);
    func_ov070_021b80c4(work, func_020355c4() ? FALSE : TRUE);
    func_ov070_021b8af4(work);
}

void func_ov070_021b8e08(ItemMenuWork *work) {
    func_ov070_021b7dd0(work);
    func_0204b92c(work->scrollBar, TRUE);
    func_ov070_021bb3f4(work, TRUE);
    func_ov070_021b80c4(work, func_020355c4() ? FALSE : TRUE);
    func_ov070_021b6100(work, func_ov070_021b7cd0);
}

void func_ov070_021b8e48(ItemMenuWork *work) {
    work->paletteFade = func_02021518(work->heapId);
    func_0202155c(work->paletteFade, 2, 0x200, work->heapId);
    func_0202155c(work->paletteFade, 0, 0x20, work->heapId);
    func_020216d4(work->paletteFade, 0, 0, 0x20);
}

void func_ov070_021b8e84(ItemMenuWork *work) {
    func_020215a0(work->paletteFade, 0);
    func_020215a0(work->paletteFade, 2);
    func_02021540(work->paletteFade);
}

void func_ov070_021b8ea8(ItemMenuWork *work) {
    if (func_0204b98c(work->sortButton) == FALSE) {
        func_0204b878(work->sortButton, 0);
        func_ov070_021b809c(work);
        func_0204b92c(work->scrollBar, FALSE);
        GFL_MsgDataLoadStrbuf(work->msgData, 0x8a, work->expandBuf);
        func_ov070_021bacf0(work, FALSE);
        func_ov070_021bb580(work, FALSE);
        func_ov070_021bb3f4(work, FALSE);
        func_ov070_021b6100(work, func_ov070_021b8f0c);
    }
}

void func_ov070_021b8f0c(ItemMenuWork *work) {
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        func_ov070_021b80c4(work, TRUE);
    } else if (func_02035c60()) {
        func_ov070_021b80c4(work, FALSE);
    } else {
        return;
    }
    work->drawnScroll = 0xffff;
    func_ov070_021b6138(work);
    func_ov070_021ba9ac(work);
    func_0204b92c(work->scrollBar, TRUE);
    func_ov070_021bb3f4(work, TRUE);
    func_ov070_021b8af4(work);
}







// Says how the pocket was sorted
// Whether an item of a field pocket can be used in the bag's mode
// pokebw2's ItemMenu_CanUseInMode
BOOL func_ov070_021b8f68(ItemMenuWork *work, u8 fieldPocket) {
    if (work->mode == 3 || work->mode == 1) {
        return FALSE;
    }
    if (GameData_IsForceSeasonSync(work->gameData) == TRUE && fieldPocket == BAG_POCKET_KEY_ITEMS) {
        return FALSE;
    }
    return TRUE;
}

void func_ov070_021b8f90(ItemMenuWork *work) {
    func_ov070_021b8af4(work);
}


