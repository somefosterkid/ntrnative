#include "types.h"
#include "app/bag_item.h"
#include "constants/items.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/math.h"
#include "pml/item.h"
#include "save/bag.h"

// The bag's list of the items the player moved to the Free Space, kept in the order the player arranged them

// The slots of the bag's pockets, 310 + 48 + 109 + 64 + 83, more than the Free Space can hold. Adding, filtering and
// compacting only look at the entries up to this one, not up to ITEM_LAST
#define BAG_ITEM_LIST_SLOTS 614

typedef struct {
    BagItemListEntry entry;
    u32 key;
} BagItemSortEntry;

static s32 BagItemList_CompareSortKey(void *a, void *b);
static void BagItemList_Compact(BagItemList *list);
static void BagItemList_Sort(BagItemList *list, u16 filter);
static void BagItemList_SortByName(BagItemList *list);
static void BagItemList_SortByType(BagItemList *list);

// Each item's place in alphabetical order
// clang-format off
static const u16 sItemNameOrder[ITEM_LAST + 1] = {
#include "asset/bag_item_sItemNameOrder.inc"
};

// clang-format on

static s32 BagItemList_CompareSortKey(void *a, void *b) {
    BagItemSortEntry *entryA = a;
    BagItemSortEntry *entryB = b;

    if (entryA->key == entryB->key) {
        return 0;
    }
    if (entryA->key > entryB->key) {
        return 1;
    }
    return -1;
}

void BagItemList_Init(BagItemList *list, BagSave *bag, u32 filter, HeapID heapId) {
    int i;
    int count;
    u32 pocket;

    list->bag = bag;
    count = 0;
    list->heapId = heapId;
    MI_CpuFill8(list->entries, 0, sizeof(list->entries));
    for (i = 0; i <= ITEM_LAST; i++) {
        if (BagSave_IsItemInFreeSpace(list->bag, i)) {
            pocket = BagSave_GetExistingItemPocket(list->bag, i);
            if (pocket != BAG_POCKET_NONE) {
                list->entries[count].item = i;
                list->entries[count].pocket = pocket;
                list->entries[count].shown = TRUE;
                count++;
            } else {
                list->entries[count].item = ITEM_NONE;
                list->entries[count].pocket = BAG_POCKET_NONE;
                list->entries[count].shown = FALSE;
            }
        } else {
            list->entries[count].item = ITEM_NONE;
            list->entries[count].pocket = BAG_POCKET_NONE;
            list->entries[count].shown = FALSE;
        }
    }
    list->empty.item = ITEM_NONE;
    list->empty.count = 0;
    BagItemList_SetFilter(list, filter);
}

void BagItemList_Exit(BagItemList *list) {
}

BagItem *BagItemList_GetItem(BagItemList *list, u16 pocket, u16 index) {
    BagItem *item;
    BagItemListEntry *entry;
    int i;
    u32 count;
    BOOL found;

    count = 0;
    if (pocket != BAG_POCKET_FREE_SPACE) {
        for (i = 0; i <= ITEM_LAST; i++) {
            item = BagSave_GetItemIndexHandle(list->bag, pocket, i);
            if (item == NULL) {
                return &list->empty;
            }
            if (!BagSave_IsItemInFreeSpace(list->bag, item->item)) {
                if (count == index) {
                    break;
                }
                count++;
            }
        }
    } else {
        found = FALSE;
        for (i = 0; i <= ITEM_LAST; i++) {
            entry = &list->entries[i];
            if (entry->shown == TRUE && entry->item != ITEM_NONE) {
                if (count == index) {
                    found = TRUE;
                    break;
                }
                count++;
            }
        }
        if (!found) {
            item = &list->empty;
        } else {
            item = BagSave_GetItemHandle(list->bag, entry->pocket, entry->item);
            if (item == NULL) {
                item = &list->empty;
            }
        }
    }
    return item;
}

void BagItemList_Remove(BagItemList *list, u16 index, BOOL release) {
    BagItemListEntry *entry;
    u32 count = 0;
    int i;

    for (i = 0; i <= ITEM_LAST; i++) {
        entry = &list->entries[i];
        if (entry->shown == TRUE && entry->item != ITEM_NONE) {
            if (count == index) {
                if (release == TRUE) {
                    BagSave_MoveBetweenFreeSpace(list->bag, entry->item, FALSE);
                }
                BagSave_ForceItemAsLast(list->bag, list->entries[i].item);
                list->entries[i].item = ITEM_NONE;
                list->entries[i].pocket = BAG_POCKET_NONE;
                list->entries[i].shown = FALSE;
                break;
            }
            count++;
        }
    }
    BagItemList_Compact(list);
}

void BagItemList_Add(BagItemList *list, u32 item, s16 pocket) {
    int i;

    for (i = 0; i <= BAG_ITEM_LIST_SLOTS; i++) {
        if (list->entries[i].item == ITEM_NONE) {
            BagSave_MoveBetweenFreeSpace(list->bag, item, TRUE);
            list->entries[i].item = item;
            list->entries[i].pocket = pocket;
            list->entries[i].shown = TRUE;
            return;
        }
    }
}

// Moves the empty entries to the end
static void BagItemList_Compact(BagItemList *list) {
    BagItemListEntry temp;
    int i;
    int j;

    for (i = 0; i <= BAG_ITEM_LIST_SLOTS - 1; i++) {
        for (j = i + 1; j <= BAG_ITEM_LIST_SLOTS; j++) {
            if (list->entries[i].item == ITEM_NONE) {
                temp = list->entries[i];
                list->entries[i] = list->entries[j];
                list->entries[j] = temp;
            }
        }
    }
}

s32 BagItemList_CountInPocket(BagItemList *list, u32 pocket) {
    int i;
    s32 count = 0;

    for (i = 0; i <= ITEM_LAST; i++) {
        if (pocket == list->entries[i].pocket && list->entries[i].item != ITEM_NONE) {
            count++;
        }
    }
    return count;
}

void BagItemList_SetFilter(BagItemList *list, u16 filter) {
    u32 pockets[] = {
        BAG_POCKET_ITEMS,   BAG_POCKET_ITEMS,   BAG_POCKET_MEDICINE,
        BAG_POCKET_TMS_HMS, BAG_POCKET_BERRIES, BAG_POCKET_KEY_ITEMS,
    };
    int i;

    for (i = 0; i <= BAG_ITEM_LIST_SLOTS; i++) {
        if (filter == BAG_ITEM_FILTER_ALL) {
            list->entries[i].shown = TRUE;
        } else if (list->entries[i].pocket == pockets[filter]) {
            list->entries[i].shown = TRUE;
        } else {
            list->entries[i].shown = FALSE;
        }
    }
    BagItemList_Sort(list, filter);
}

u32 BagItemList_CountShown(BagItemList *list, u32 pocket) {
    BagItem *items;
    u32 count = 0;
    u32 slots;
    u32 i;

    BagSave_GetOwnedItemFlags(list->bag, list->inBag);
    if (pocket != BAG_POCKET_FREE_SPACE) {
        items = BagSave_GetPocketItems(list->bag, pocket, &slots);
        for (i = 0; i < slots; i++) {
            if (items[i].item != ITEM_NONE && list->inBag[items[i].item] &&
                !BagSave_IsItemFreeSpaceBit(list->bag, items[i].item)) {
                count++;
            }
        }
    } else {
        for (i = 0; i <= ITEM_LAST; i++) {
            if (list->entries[i].item != ITEM_NONE && list->inBag[list->entries[i].item] &&
                list->entries[i].shown == TRUE) {
                count++;
            }
        }
    }
    return count;
}

static void BagItemList_Sort(BagItemList *list, u16 filter) {
    switch (filter) {
    case BAG_ITEM_FILTER_ALL:
        BagItemList_SortByName(list);
        break;
    case BAG_ITEM_FILTER_ITEMS:
    case BAG_ITEM_FILTER_MEDICINE:
    case BAG_ITEM_FILTER_TMS_HMS:
    case BAG_ITEM_FILTER_BERRIES:
    case BAG_ITEM_FILTER_KEY_ITEMS:
        BagItemList_SortByType(list);
        break;
    }
}

static void BagItemList_SortByName(BagItemList *list) {
    int i;
    BagItemSortEntry *sort;
    s32 count;

    sort = GFL_HeapAllocate(list->heapId, sizeof(BagItemSortEntry) * ITEM_LAST, FALSE, "bag_item.c", 525);
    count = BagSave_GetPocketItemCount(list->bag, BAG_POCKET_FREE_SPACE);
    for (i = 0; i < count; i++) {
        sort[i].key = list->entries[i].item + (sItemNameOrder[list->entries[i].item] << 16);
        sort[i].entry = list->entries[i];
    }
    MATH_QSort(sort, count, sizeof(BagItemSortEntry), BagItemList_CompareSortKey, NULL);
    for (i = 0; i < count; i++) {
        list->entries[i] = sort[i].entry;
    }
    GFL_HeapFree(sort);
}

static void BagItemList_SortByType(BagItemList *list) {
    BagItemSortEntry *sort;
    int i;
    s32 count;
    BagItemListEntry *entry;
    ArcTool *arc;
    void *data;

    sort = GFL_HeapAllocate(list->heapId, sizeof(BagItemSortEntry) * ITEM_LAST, FALSE, "bag_item.c", 566);
    count = BagSave_GetPocketItemCount(list->bag, BAG_POCKET_FREE_SPACE);
    arc = PML_ItemArcHandleCreate(list->heapId);
    for (i = 0; i < count; i++) {
        entry = &list->entries[i];
        data = PML_ItemArcHandleReadFile(arc, entry->item, list->heapId);
        // By the item's kind, then its place among them
        sort[i].key = (PML_ItemGetParam(data, ITEM_PARAM_KIND) << 28) +
                      (PML_ItemGetParam(data, ITEM_PARAM_SORT_INDEX) << 16) + entry->item;
        sort[i].entry = *entry;
        GFL_HeapFree(data);
    }
    GFL_ArcToolFree(arc);
    MATH_QSort(sort, count, sizeof(BagItemSortEntry), BagItemList_CompareSortKey, NULL);
    for (i = 0; i < count; i++) {
        list->entries[i] = sort[i].entry;
    }
    GFL_HeapFree(sort);
}
