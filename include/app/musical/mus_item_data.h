// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/mus_item_data.h, the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_MUS_ITEM_DATA_H
#define BWPORT_APP_MUSICAL_MUS_ITEM_DATA_H

// Overlay 112's mus_item_data.c: the musical's props

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's MusItemData_Init and MusItemData_Free
MusItemData *func_ov112_021f4f14(HeapID heapId);
void func_ov112_021f4f40(MusItemData *items);
// The kind of a prop (pokebw2's func_ov210_021ef164)
u32 func_ov112_021f5140(MusItemData *items, u16 itemId);

// A prop's entry in the table, 12 bytes
struct MusicalItemData {
    // The BlAct size of its texture
    u32 texSize;
    // Its offset from where it is worn
    s8 offsetX;
    s8 offsetY;
    // The positions it can be worn at
    u16 flags;
    // The kind of position it is worn at
    u8 category;
    u8 unk9;
    // The kind of its effect when it is used
    u8 effect;
};

// The loaded table
struct MusItemData {
    void *data;
    MusicalItemData *items;
};

// pokebw2's MusItemData_GetItem
MusicalItemData *func_ov112_021f4f54(MusItemData *items, u16 itemId);
// pokebw2's MusItemData_GetOffset
// The prop's offset from where it is worn, x then y
void func_ov112_021f4f60(MusicalItemData *item, s32 *offset);
// Whether the prop can be worn at a position, by its flags
BOOL func_ov112_021f4f74(MusicalItemData *item, u32 pos);
BOOL func_ov112_021f4ff4(MusicalItemData *item, s32 pos);
// Whether the prop's category is that of a position
BOOL func_ov112_021f5064(MusicalItemData *item, s32 pos);
// Its flags 0x80 and 0x200
BOOL func_ov112_021f50d0(MusicalItemData *item);
BOOL func_ov112_021f50e0(MusicalItemData *item);
u8 func_ov112_021f50f4(s32 pos);
// pokebw2's MusItemData_GetEffect
// The kind of a prop's effect when it is used
u8 func_ov112_021f514c(MusItemData *items, u16 itemId);
// pokebw2's MusItemData_GetCategory
u8 func_ov112_021f5158(MusItemData *items, u16 itemId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MusItemData_GetTexSize
// The BlAct size of its texture
u32 func_ov112_021f4f70(MusicalItemData *item);

#endif // BWPORT_APP_MUSICAL_MUS_ITEM_DATA_H
