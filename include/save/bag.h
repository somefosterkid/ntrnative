// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/bag.h: the prototypes of Black's named
// functions and the types they need.
#ifndef BWPORT_SAVE_BAG_H
#define BWPORT_SAVE_BAG_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "constants/bag.h"

// An item and its count in a pocket (swan's BagItem)
struct BagItem {
    u16 item;
    u16 count;
};

// The slots of each pocket (Black's, from BagSave_GetPocketPtr)
#define BAG_ITEMS_MAX 310
#define BAG_KEY_ITEMS_MAX 83
#define BAG_TMS_HMS_MAX 109
#define BAG_MEDICINE_MAX 48
#define BAG_BERRIES_MAX 64

// How many of an item a slot holds
#define BAG_ITEM_COUNT_MAX 999

#define BAG_TM_HM_COUNT_MAX 1

// The battle bag has four pockets
#define BATTLE_BAG_POCKET_COUNT 4

// The bag's save block (swan's BagSave). Black has no Free Space pocket; its block is 0x9c0 bytes (func_02007cb0)
struct BagSave {
    BagItem items[BAG_ITEMS_MAX];        // 0x000
    BagItem keyItems[BAG_KEY_ITEMS_MAX]; // 0x4d8
    BagItem tmsHms[BAG_TMS_HMS_MAX];     // 0x624
    BagItem medicine[BAG_MEDICINE_MAX];  // 0x7d8
    BagItem berries[BAG_BERRIES_MAX];    // 0x898
    u8 unk_998[0x28];                    // 0x998: nothing in this file reads it
};                                       // 0x9c0

// The cursor memory of the bag and of the battle bag, which is not saved. Black's has five pockets (0x2c bytes)
typedef struct BagCursor {
    s16 scrolls[BAG_POCKET_COUNT];            // 0x00
    s16 rows[BAG_POCKET_COUNT];               // 0x0a
    // The pocket the bag opens on
    u16 pocket;                               // 0x14
    u16 unk_16;
    s16 battlePages[BATTLE_BAG_POCKET_COUNT]; // 0x18
    s16 battleRows[BATTLE_BAG_POCKET_COUNT];  // 0x20
    u16 battleLastItem;                       // 0x28
    u16 battleLastPocket;                     // 0x2a
} BagCursor;                                  // 0x2c

// The number of slots that hold an item
u32 BagSave_GetUniqueItemCount(BagItem *items, u32 max);
// Sorts the bag's pockets
void sortItemBlocks(BagSave *bag);
// The number of slots of a pocket that hold an item
u32 BagSave_GetPocketItemCount(BagSave *bag, u32 pocket);
void BagSave_CopyPocketRaw(BagSave *bag, BagItem *items, u32 pocket, BOOL load);

BOOL BagSave_AddItem(BagSave *bag, u16 item, u16 count, HeapID heapId);
// Whether count of an item fit in the bag
BOOL BagSave_CheckAvailItemSpace(BagSave *bag, u16 item, u16 count, HeapID heapId);
u16 BagSave_GetItemCountByID(BagSave *bag, u16 item, HeapID heapId);
// Whether the bag holds at least count of an item
BOOL BagSave_CheckAmount(BagSave *bag, u16 item, u16 count, HeapID heapId);
BOOL BagSave_SubItem(BagSave *bag, u16 item, u16 count, HeapID heapId);
// The slot at an index of a pocket, or NULL past its end
BagItem *BagSave_GetItemIndexHandle(BagSave *bag, u32 pocket, u16 index);
// pokebw2's BagCursor_Create
// The battle bag's cursor memory, in the same cursor (allocated here): the row and page of a pocket, and the last used
// item and its pocket
BagCursor *func_02008254(HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_020082b8(BagCursor *cursor, const s16 *rows, const s16 *pages);
void func_020082d4(BagCursor *cursor, u16 item, u16 pocket);
void func_0200829c(BagCursor *cursor, u16 pocket, s16 *row, s16 *page);
u16 func_020082b0(BagCursor *cursor);
u16 func_020082b4(BagCursor *cursor);
void func_02008298(BagCursor *cursor, u16 pocket);
void func_02008288(BagCursor *cursor, u32 pocket, s16 row, s16 scroll);
void func_02008270(BagCursor *cursor, u32 pocket, s16 *row, s16 *scroll);

#endif // BWPORT_SAVE_BAG_H
