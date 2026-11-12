// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov286/b_bag_item.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/b_bag_item.h"
#include "battle/b_bag_main.h"
#include "constants/items.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "system/shooter_item.h"
#include "constants/bag.h"

// The battle bag's item lists (b_bag_item.c, our guess after the ROM's b_bag_main.c and Diamond and Pearl's
// b_bag_item.c): the four battle pockets filled from the bag, from the Wonder Launcher's items or with the catching
// demo's Poké Balls, the last used item, and the item in a slot of the shown page

// The bag pockets the battle pockets are filled from: Items, Medicine and Berries
static const u8 data_ov197_0220db3c[3] = { BAG_POCKET_ITEMS, BAG_POCKET_MEDICINE, BAG_POCKET_BERRIES };
// The battle pocket of each bit of an item's byte in archive 228
static const u8 data_ov197_0220db3f[4] = { 2, 3, 0, 1 };

// Clears the last used item when the bag no longer holds it
// pokebw2's BBagItem_CheckLastItem
BOOL func_ov197_0220b9a4(BBagWork *work) {
    if (work->lastItem == 0) {
        return FALSE;
    }
    if (BagSave_CheckAmount(work->param->bag, work->lastItem, 1, work->param->heapId) == FALSE) {
        work->lastItem = 0;
        work->lastPocket = 0;
        return FALSE;
    }
    return TRUE;
}

// Puts the cursor on the last used item in the pocket
// pokebw2's BBagItem_SetCursorToLastItem
void func_ov197_0220b9d8(BBagWork *work) {
    u32 i;

    for (i = 0; i < 36; i++) {
        if (work->lastItem == work->items[work->pocket][i].item) {
            work->param->rows[work->pocket] = i % 6;
            work->param->pages[work->pocket] = i / 6;
            return;
        }
    }
}

// Fills the battle pockets from the bag: archive 228 has a byte per item whose bits are the battle pockets it shows in
// pokebw2's BBagItem_MakePocketLists
// Fills the battle pockets from the bag: archive 232 has a byte per item whose bits are the battle pockets it shows in
void func_ov197_0220ba2c(BBagWork *work) {
    BagItem *slot;
    u8 *pockets;
    u32 i, j, k;
    u8 pocket;
    u8 bits;

    pockets = GFL_ArcSysReadHeapNew(232, 0, work->param->heapId);
    for (i = 0; i < 3; i++) {
        for (j = 0;; j++) {
            slot = BagSave_GetItemIndexHandle(work->param->bag, data_ov197_0220db3c[i], j);
            if (slot == NULL) {
                break;
            }
            if (slot->item == 0 || slot->count == 0) {
                continue;
            }
            bits = pockets[slot->item];
            for (k = 0; k < 4; k++) {
                if (bits & (1 << k)) {
                    pocket = data_ov197_0220db3f[k];
                    work->items[pocket][work->itemCount[pocket]] = *slot;
                    work->itemCount[pocket]++;
                }
            }
        }
    }
    GFL_HeapFree(pockets);

    for (i = 0; i < 4; i++) {
        if (work->itemCount[i] == 0) {
            work->lastPage[i] = 0;
        } else {
            work->lastPage[i] = (work->itemCount[i] - 1) / 6;
        }
        if (work->lastPage[i] < work->param->pages[i]) {
            work->param->pages[i] = work->lastPage[i];
        }
    }
}

// Fills the first pocket with the Wonder Launcher's enabled items
// pokebw2's BBagItem_MakeShooterList
void func_ov197_0220bb1c(BBagWork *work) {
    u32 i;

    for (i = 0; i < SHOOTER_ITEM_COUNT; i++) {
        if (func_0202f114(work->param->shooterDisabled, i) == TRUE) {
            work->items[0][work->itemCount[0]].item = func_0202f138(i);
            work->items[0][work->itemCount[0]].count = 1;
            work->itemCount[0]++;
        }
    }
    work->lastPage[0] = (work->itemCount[0] - 1) / 6;
}

// The catching demo's bag: 30 Poké Balls
// pokebw2's BBagItem_MakeDemoList
void func_ov197_0220bb78(BBagWork *work) {
    work->items[2][0].item = ITEM_POKE_BALL;
    work->items[2][0].count = 30;
    work->itemCount[2] = 1;
    work->lastPage[2] = 0;
}

// The item in a slot of the shown page, 0 if the slot is empty
// pokebw2's BBagItem_GetSlotItem
u16 func_ov197_0220bb98(BBagWork *work, int slot) {
    u32 i = work->param->pages[work->pocket] * 6 + slot;

    if (work->items[work->pocket][i].item == 0 || work->items[work->pocket][i].count == 0) {
        return 0;
    }
    return work->items[work->pocket][i].item;
}

// pokebw2's BBagItem_GetShooterCost
u16 func_ov197_0220bbd8(u16 item) {
    return func_0202f144(item);
}
