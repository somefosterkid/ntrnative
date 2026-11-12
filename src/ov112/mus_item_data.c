// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov210/mus_item_data.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/mus_item_data.h"
#include "constants/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"

// Overlay 210's mus_item_data.c, named after its string: the table of the musical's props

// The file of ARCID_MUSICAL_ITEM that holds the table, after the props' textures
#define MUS_ITEM_DATA_FILE 101

// pokebw2's MusItemData_Init
MusItemData *func_ov112_021f4f14(HeapID heapId) {
    MusItemData *items = GFL_HeapAllocate(heapId, sizeof(MusItemData), FALSE, "mus_item_data.c", 71);
    items->data = GFL_ArcSysReadHeapNewLZ(ARCID_MUSICAL_ITEM, MUS_ITEM_DATA_FILE, FALSE, heapId);
    items->items = items->data;
    return items;
}

// pokebw2's MusItemData_Free
void func_ov112_021f4f40(MusItemData *items) {
    GFL_HeapFree(items->data);
    GFL_HeapFree(items);
}

// pokebw2's MusItemData_GetItem
MusicalItemData *func_ov112_021f4f54(MusItemData *items, u16 itemId) {
    return &items->items[itemId];
}

// pokebw2's MusItemData_GetOffset
void func_ov112_021f4f60(MusicalItemData *item, s32 *offset) {
    offset[0] = item->offsetX;
    offset[1] = item->offsetY;
}

// pokebw2's MusItemData_GetTexSize
u32 func_ov112_021f4f70(MusicalItemData *item) {
    return item->texSize;
}

BOOL func_ov112_021f4f74(MusicalItemData *item, u32 pos) {
    switch (pos) {
    case 0:
    case 1:
        if (item->flags & 0x1) {
            return TRUE;
        }
        break;
    case 2:
        if (item->flags & 0x2) {
            return TRUE;
        }
        break;
    case 3:
        if (item->flags & 0x4) {
            return TRUE;
        }
        break;
    case 4:
        if (item->flags & 0x8) {
            return TRUE;
        }
        break;
    case 5:
        if (item->flags & 0x10) {
            return TRUE;
        }
        break;
    case 6:
        if (item->flags & 0x20) {
            return TRUE;
        }
        break;
    case 7:
    case 8:
        if (item->flags & 0x40) {
            return TRUE;
        }
        break;
    case 9:
        return FALSE;
    }
    return FALSE;
}

BOOL func_ov112_021f4ff4(MusicalItemData *item, s32 pos) {
    switch (pos) {
    case 0:
    case 1:
        if (item->flags & 0x1) {
            return TRUE;
        }
        break;
    case 2:
        if (item->flags & 0x2) {
            return TRUE;
        }
        break;
    case 3:
        if (item->flags & 0xc) {
            return TRUE;
        }
        break;
    case 4:
        if (item->flags & 0x10) {
            return TRUE;
        }
        break;
    case 5:
        if (item->flags & 0x20) {
            return TRUE;
        }
        break;
    case 6:
    case 7:
        if (item->flags & 0x40) {
            return TRUE;
        }
        break;
    case 8:
        return FALSE;
    }
    return FALSE;
}

BOOL func_ov112_021f5064(MusicalItemData *item, s32 pos) {
    switch (pos) {
    case 0:
    case 1:
        if (item->category == 0) {
            return TRUE;
        }
        break;
    case 2:
        if (item->category == 1) {
            return TRUE;
        }
        break;
    case 3:
        if (item->category == 2 || item->category == 3) {
            return TRUE;
        }
        break;
    case 4:
        if (item->category == 4) {
            return TRUE;
        }
        break;
    case 5:
        if (item->category == 5) {
            return TRUE;
        }
        break;
    case 6:
    case 7:
        if (item->category == 6) {
            return TRUE;
        }
        break;
    case 8:
        return FALSE;
    }
    return FALSE;
}

BOOL func_ov112_021f50d0(MusicalItemData *item) {
    if (item->flags & 0x80) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov112_021f50e0(MusicalItemData *item) {
    if (item->flags & 0x200) {
        return TRUE;
    }
    return FALSE;
}

u8 func_ov112_021f50f4(s32 pos) {
    switch (pos) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
    case 4:
        return 3;
    case 5:
        return 4;
    case 6:
        return 5;
    case 7:
        return 6;
    case 8:
        return 7;
    case 9:
        return 8;
    }
    return 9;
}

u32 func_ov112_021f5140(MusItemData *items, u16 itemId) {
    return func_ov112_021f4f54(items, itemId)->unk9;
}

// pokebw2's MusItemData_GetEffect
u8 func_ov112_021f514c(MusItemData *items, u16 itemId) {
    return func_ov112_021f4f54(items, itemId)->effect;
}

// pokebw2's MusItemData_GetCategory
u8 func_ov112_021f5158(MusItemData *items, u16 itemId) {
    return func_ov112_021f4f54(items, itemId)->category;
}
