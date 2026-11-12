// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/myitem_savedata.c, ported by tools/port_file.py.
#include "types.h"
#include "save/bag.h"
#include "constants/items.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/std.h"
#include "pml/item.h"

// The bag's save block: the five pockets, the items the player moved to the Free Space, and the cursor memory of the
// bag and the battle bag. The name is the ROM's string. The functions' names are swan's, except where the header
// says otherwise

// The layout of this table is not known: three groups of five words
static const u32 data_0209dd64[] = {
#include "asset/myitem_savedata_data_0209dd64.inc"
};

static u32 BagSave_GetPocketPtr(BagSave *bag, u32 pocket, BagItem **items, u32 *max);
static u32 func_02007d6c(BagSave *bag, u16 item, BagItem **items, u32 *max, HeapID heapId);
static BagItem *BagSave_GetItemHandleAddCheckCore(BagItem *items, u32 max, u32 item, int count, u16 maxCount);
static BagItem *func_02007df8(BagSave *bag, u16 item, int count, HeapID heapId);
static BagItem *BagSave_GetItemHandleIfAmountCore(BagItem *items, u32 max, u32 item, u32 count);
static BagItem *func_02007ef8(BagSave *bag, u16 item, u32 count, HeapID heapId);
static void BagSave_SwapItems(BagItem *a, BagItem *b);
void BagSave_MoveLastToFirst(BagItem *items, u32 count);
static void BagSave_ClearEmpty(BagItem *items, u32 max);
static void BagSave_Defragment(BagItem *items, u32 max);
void func_020080a0(BagItem *items, u32 max);
void func_020080fc(BagItem *items, u32 max);


u32 func_02007cb0(void) {
    return 0x9c0;
}

void func_02007cb8(BagSave *bag) {
    MI_CpuFill8(bag, 0, 0x9c0);
}


void BagSave_CopyPocketRaw(BagSave *bag, BagItem *items, u32 pocket, BOOL load) {
    BagItem *pocketItems;
    u32 max;

    BagSave_GetPocketPtr(bag, pocket, &pocketItems, &max);
    if (load) {
        MI_CpuCopy8(pocketItems, items, max * sizeof(BagItem));
    } else {
        MI_CpuCopy8(items, pocketItems, max * sizeof(BagItem));
    }
}

// A pocket's slots and their number (pokebw2's BagSave_GetPocketPtr)
static u32 BagSave_GetPocketPtr(BagSave *bag, u32 pocket, BagItem **items, u32 *max) {
    switch (pocket) {
    case BAG_POCKET_KEY_ITEMS:
        *items = bag->keyItems;
        *max = BAG_KEY_ITEMS_MAX;
        break;
    case BAG_POCKET_ITEMS:
        *items = bag->items;
        *max = BAG_ITEMS_MAX;
        break;
    case BAG_POCKET_BERRIES:
        *items = bag->berries;
        *max = BAG_BERRIES_MAX;
        break;
    case BAG_POCKET_MEDICINE:
        *items = bag->medicine;
        *max = BAG_MEDICINE_MAX;
        break;
    case BAG_POCKET_TMS_HMS:
        *items = bag->tmsHms;
        *max = BAG_TMS_HMS_MAX;
        break;
    default:
        *items = NULL;
        break;
    }
    return pocket;
}

// The slots of the pocket an item belongs in, and that pocket (pokebw2's BagSave_GetPocketPtrByItem)
static u32 func_02007d6c(BagSave *bag, u16 item, BagItem **items, u32 *max, HeapID heapId) {
    u32 pocket = GetItemParam(item, ITEM_PARAM_FIELD_POCKET, heapId);

    BagSave_GetPocketPtr(bag, pocket, items, max);
    return pocket;
}

// The slot that count more of an item go in: the item's own, unless it would hold more than maxCount, or else the
// first empty one
static BagItem *BagSave_GetItemHandleAddCheckCore(BagItem *items, u32 max, u32 item, int count, u16 maxCount) {
    u32 i;
    s32 empty = -1;

    for (i = 0; i < max; i++) {
        if (items[i].item == item) {
            if (items[i].count + count > maxCount) {
                return NULL;
            }
            return &items[i];
        }
        if (empty == -1 && items[i].item == ITEM_NONE && items[i].count == 0) {
            empty = i;
        }
    }
    if (empty == -1) {
        return NULL;
    }
    return &items[empty];
}

// The slot that count more of an item go in, in its pocket (pokebw2's BagSave_GetItemHandleAddCheck)
static BagItem *func_02007df8(BagSave *bag, u16 item, int count, HeapID heapId) {
    BagItem *items;
    u32 max;
    u32 pocket = func_02007d6c(bag, item, &items, &max, heapId);

    if (pocket == BAG_POCKET_TMS_HMS) {
        return BagSave_GetItemHandleAddCheckCore(items, max, item, count, BAG_TM_HM_COUNT_MAX);
    }
    return BagSave_GetItemHandleAddCheckCore(items, max, item, count, BAG_ITEM_COUNT_MAX);
}

BOOL BagSave_CheckAvailItemSpace(BagSave *bag, u16 item, u16 count, HeapID heapId) {
    if (func_02007df8(bag, item, count, heapId) != NULL) {
        return TRUE;
    }
    return FALSE;
}

// Adds count of an item, then sorts the Berries and the TMs and HMs, or puts an item the bag didn't hold first
BOOL BagSave_AddItem(BagSave *bag, u16 item, u16 count, HeapID heapId) {
    BagItem *slot;
    u32 max;
    BagItem *items;
    u32 total;
    u32 pocket;
    u16 oldCount;

    slot = func_02007df8(bag, item, count, heapId);
    if (slot == NULL) {
        return FALSE;
    }
    oldCount = slot->count;
    slot->item = item;
    slot->count += count;
    pocket = func_02007d6c(bag, item, &slot, &max, heapId);
    if (pocket == BAG_POCKET_BERRIES) {
        func_020080a0(slot, max);
    } else if (pocket == BAG_POCKET_TMS_HMS) {
        func_020080fc(slot, max);
    } else if (oldCount == 0) {
        BagSave_GetPocketPtr(bag, pocket, &items, &total);
        total = BagSave_GetUniqueItemCount(items, total);
        BagSave_MoveLastToFirst(items, total);
    }
    return TRUE;
}

// An item's slot, if it holds at least count
static BagItem *BagSave_GetItemHandleIfAmountCore(BagItem *items, u32 max, u32 item, u32 count) {
    u32 i;

    for (i = 0; i < max; i++) {
        if (items[i].item == item) {
            if (items[i].count >= count) {
                return &items[i];
            }
            return NULL;
        }
    }
    return NULL;
}

// An item's slot in its pocket, if it holds at least count (pokebw2's BagSave_GetItemHandleIfAmount)
static BagItem *func_02007ef8(BagSave *bag, u16 item, u32 count, HeapID heapId) {
    BagItem *items;
    u32 max;

    func_02007d6c(bag, item, &items, &max, heapId);
    return BagSave_GetItemHandleIfAmountCore(items, max, item, count);
}

BOOL BagSave_SubItem(BagSave *bag, u16 item, u16 count, HeapID heapId) {
    BagItem *slot;
    u32 max;

    slot = func_02007ef8(bag, item, count, heapId);
    if (slot == NULL) {
        return FALSE;
    }
    slot->count -= count;
    if (slot->count == 0) {
        slot->item = ITEM_NONE;
    }
    func_02007d6c(bag, item, &slot, &max, heapId);
    BagSave_Defragment(slot, max);
    return TRUE;
}

BOOL BagSave_CheckAmount(BagSave *bag, u16 item, u16 count, HeapID heapId) {
    if (func_02007ef8(bag, item, count, heapId) != NULL) {
        return TRUE;
    }
    return FALSE;
}

u32 BagSave_GetUniqueItemCount(BagItem *items, u32 max) {
    u32 count = 0;
    u32 i;

    if (items != NULL) {
        for (i = 0; i < max; i++) {
            if (items[i].item != ITEM_NONE) {
                count++;
            }
        }
    }
    return count;
}

// The number of slots of a pocket that hold an item
u32 BagSave_GetPocketItemCount(BagSave *bag, u32 pocket) {
    BagItem *items;
    u32 max;

    BagSave_GetPocketPtr(bag, pocket, &items, &max);
    return BagSave_GetUniqueItemCount(items, max);
}

u16 BagSave_GetItemCountByID(BagSave *bag, u16 item, HeapID heapId) {
    BagItem *slot = func_02007ef8(bag, item, 1, heapId);

    if (slot == NULL) {
        return 0;
    }
    return slot->count;
}

static void BagSave_SwapItems(BagItem *a, BagItem *b) {
    BagItem temp = *a;

    *a = *b;
    *b = temp;
}

// Moves the slots of a pocket up by one and puts the last one first
void BagSave_MoveLastToFirst(BagItem *items, u32 count) {
    BagItem last;
    s32 i;

    last.item = ITEM_NONE;
    if (count > 1) {
        for (i = count - 1; i >= 0; i--) {
            if (last.item == ITEM_NONE) {
                MI_CpuCopy8(&items[i], &last, sizeof(BagItem));
            } else {
                MI_CpuCopy8(&items[i], &items[i + 1], sizeof(BagItem));
            }
        }
        MI_CpuCopy8(&last, &items[0], sizeof(BagItem));
    }
}

// Clears the count of the slots that hold no item
static void BagSave_ClearEmpty(BagItem *items, u32 max) {
    u32 i;

    for (i = 0; i < max; i++) {
        if (items[i].item == ITEM_NONE) {
            items[i].count = 0;
        }
    }
}

// Moves the empty slots of a pocket behind the others
static void BagSave_Defragment(BagItem *items, u32 max) {
    u32 i;
    u32 j;

    BagSave_ClearEmpty(items, max);
    for (i = 0; i < max - 1; i++) {
        for (j = i + 1; j < max; j++) {
            if (items[i].count == 0) {
                BagSave_SwapItems(&items[i], &items[j]);
            }
        }
    }
}

// Sorts a pocket by item number, the empty slots last
void func_020080a0(BagItem *items, u32 max) {
    u32 i;
    u32 j;

    BagSave_ClearEmpty(items, max);
    for (i = 0; i < max - 1; i++) {
        for (j = i + 1; j < max; j++) {
            if (items[i].count == 0 || (items[j].count != 0 && items[i].item > items[j].item)) {
                BagSave_SwapItems(&items[i], &items[j]);
            }
        }
    }
}

// Sorts the TMs and HMs by their number, the empty slots last
void func_020080fc(BagItem *items, u32 max) {
    u32 i;
    u32 j;

    BagSave_ClearEmpty(items, max);
    for (i = 0; i < max - 1; i++) {
        for (j = i + 1; j < max; j++) {
            if (items[i].count == 0
                || (items[j].count != 0 && PML_ItemGetTMBitMask(items[i].item) > PML_ItemGetTMBitMask(items[j].item))) {
                BagSave_SwapItems(&items[i], &items[j]);
            }
        }
    }
}

void sortItemBlocks(BagSave *bag) {
    BagItem *items;
    u32 max;

    BagSave_GetPocketPtr(bag, BAG_POCKET_KEY_ITEMS, &items, &max);
    BagSave_Defragment(items, max);
    BagSave_GetPocketPtr(bag, BAG_POCKET_ITEMS, &items, &max);
    BagSave_Defragment(items, max);
    BagSave_GetPocketPtr(bag, BAG_POCKET_BERRIES, &items, &max);
    BagSave_Defragment(items, max);
    BagSave_GetPocketPtr(bag, BAG_POCKET_MEDICINE, &items, &max);
    BagSave_Defragment(items, max);
    BagSave_GetPocketPtr(bag, BAG_POCKET_TMS_HMS, &items, &max);
    BagSave_Defragment(items, max);
}

BagItem *BagSave_GetItemIndexHandle(BagSave *bag, u32 pocket, u16 index) {
    BagItem *items;
    u32 max;

    switch (pocket) {
    case BAG_POCKET_KEY_ITEMS:
        items = bag->keyItems;
        max = BAG_KEY_ITEMS_MAX;
        break;
    case BAG_POCKET_ITEMS:
        items = bag->items;
        max = BAG_ITEMS_MAX;
        break;
    case BAG_POCKET_BERRIES:
        items = bag->berries;
        max = BAG_BERRIES_MAX;
        break;
    case BAG_POCKET_MEDICINE:
        items = bag->medicine;
        max = BAG_MEDICINE_MAX;
        break;
    case BAG_POCKET_TMS_HMS:
        items = bag->tmsHms;
        max = BAG_TMS_HMS_MAX;
        break;
    }
    if (index >= max) {
        return NULL;
    }
    return &items[index];
}

u32 PML_ItemGetMaxStorageCount(u16 item) {
    if (PML_ItemIsTMHM(item) == TRUE) {
        return BAG_TM_HM_COUNT_MAX;
    }
    return BAG_ITEM_COUNT_MAX;
}

BagCursor *func_02008254(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(BagCursor), TRUE, "myitem_savedata.c", 834);
}

void func_02008270(BagCursor *cursor, u32 pocket, s16 *row, s16 *scroll) {
    *row = cursor->rows[pocket];
    *scroll = cursor->scrolls[pocket];
}


u16 func_02008284(BagCursor *cursor) {
    return cursor->pocket;
}

void func_02008288(BagCursor *cursor, u32 pocket, s16 row, s16 scroll) {
    cursor->rows[pocket] = row;
    cursor->scrolls[pocket] = scroll;
}


void func_02008298(BagCursor *cursor, u16 pocket) {
    cursor->pocket = pocket;
}

void func_0200829c(BagCursor *cursor, u16 pocket, s16 *row, s16 *page) {
    *row = cursor->battleRows[pocket];
    *page = cursor->battlePages[pocket];
}


u16 func_020082b0(BagCursor *cursor) {
    return cursor->battleLastItem;
}

u16 func_020082b4(BagCursor *cursor) {
    return cursor->battleLastPocket;
}

void func_020082b8(BagCursor *cursor, const s16 *rows, const s16 *pages) {
    u32 i;

    for (i = 0; i < BATTLE_BAG_POCKET_COUNT; i++) {
        cursor->battleRows[i] = rows[i];
        cursor->battlePages[i] = pages[i];
    }
}


void func_020082d4(BagCursor *cursor, u16 item, u16 pocket) {
    cursor->battleLastItem = item;
    cursor->battleLastPocket = pocket;
}






void func_020082dc(void) {
    setKeyBlkStarted(data_0209dd64);
}



