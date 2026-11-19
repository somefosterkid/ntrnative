#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "unk_0201956C.h"


UnkStruct_0201956C *sub_0201956C(BgConfig *bgConfig, int a1, int a2, enum HeapID a3) {
    enum HeapID heapID;
    UnkStruct_0201956C *v0;
    u32 size;
    void *dest;

    heapID = a3;
    v0 = Heap_Alloc(a3, sizeof(UnkStruct_0201956C));
    v0->bgConfig = bgConfig;
    v0->count = a2;
    v0->heapId = heapID;
    v0->transferMode = a1;
    size = a2 * sizeof(BGWinFrameEntry);
    v0->moving = 0;
    dest = Heap_Alloc(heapID, size);
    v0->entries = dest;
    MI_CpuFill8(dest, 0U, size);
    return v0;
}

void sub_020195C0(UnkStruct_0201956C *a0) {
    u32 i;

    for (i = 0; i < a0->count; i++) {
        if (a0->entries[i].screen != NULL) {
            Heap_Free(a0->entries[i].screen);
        }
    }
    Heap_Free(a0->entries);
    Heap_Free(a0);
}
