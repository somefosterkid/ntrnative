// Black's code at ov021 0x021c0598-0x021c06a8. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/height_ex.h"
#include "gfl/heap.h"

u32 func_ov021_021c0598(s32 x, s32 z, HeightExEntry *entry) {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;

    if (entry->inUse == 0) {
        return 0;
    }
    x0 = entry->x;
    z0 = entry->z;
    x1 = x0 + entry->width - 1;
    z1 = z0 + entry->depth - 1;
    if (x0 <= x && x <= x1 && z0 <= z && z <= z1) {
        return 1;
    }
    return 0;
}

HeightExList *func_ov021_021c05d4(u32 count, u8 heapId) {
    u8 i;
    HeightExList *list;

    list = GFL_HeapAllocate(heapId, sizeof(HeightExList), 0, "height_ex.c", 0x5a);
    list->entries = GFL_HeapAllocate(heapId, count * sizeof(HeightExEntry), 0, "height_ex.c", 0x5b);
    list->count = count;
    for (i = 0; i < count; i++) {
        list->entries[i].inUse = FALSE;
    }
    return list;
}

void func_ov021_021c0628(u8 index, u32 x, u32 z, u32 width, u32 depth, u32 value, u32 flags, HeightExList *list) {
    if (index < list->count) {
        if (list->entries[index].inUse == FALSE) {
            list->entries[index].x = x;
            list->entries[index].z = z;
            list->entries[index].width = width;
            list->entries[index].depth = depth;
            list->entries[index].value = value;
            list->entries[index].flags = flags;
            list->entries[index].inUse = TRUE;
        }
    }
}

void func_ov021_021c0674(HeightExList *list) {
    GFL_HeapFree(list->entries);
    GFL_HeapFree(list);
}

u32 func_ov021_021c0688(HeightExList *list) {
    return list->count;
}

BOOL func_ov021_021c068c(s32 x, s32 z, HeightExList *list, u8 index) {
    if (func_ov021_021c0598(x, z, &list->entries[index]) != 0) {
        return 1;
    }
    return 0;
}
