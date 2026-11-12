// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/bag_item.h: the lines in use so far.
#ifndef BWPORT_APP_BAG_ITEM_H
#define BWPORT_APP_BAG_ITEM_H

#include "types.h"
#include "constants/items.h"
#include "gfl/heap.h"
#include "save/bag.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
struct BagItemListEntry {
    u16 item;
    // The pocket the item belongs in, or BAG_POCKET_NONE for an empty entry
    u16 pocket;
    // Whether the filter shows the item
    BOOL shown;
};

// Black 2's, not checked against Black
struct BagItemList {
    // What the list returns for an item it doesn't have
    BagItem empty;
    BagSave *bag;
    HeapID heapId;
    BagItemListEntry entries[ITEM_LAST + 1];
    // Whether each item is in one of the bag's pockets
    u8 inBag[ITEM_LAST + 1];
};

// Black 2's, not checked against Black
// The filters of the list: every item, or the items of one pocket
#define BAG_ITEM_FILTER_ALL 0

// Black 2's, not checked against Black
#define BAG_ITEM_FILTER_ITEMS 1
#define BAG_ITEM_FILTER_MEDICINE 2
#define BAG_ITEM_FILTER_TMS_HMS 3
#define BAG_ITEM_FILTER_BERRIES 4
#define BAG_ITEM_FILTER_KEY_ITEMS 5

#endif // BWPORT_APP_BAG_ITEM_H
