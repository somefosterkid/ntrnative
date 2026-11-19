#include "global.h"

#include "heap.h"
#include "overlay_91.h"

void *ov91_0225CCC4(enum HeapID heapID, s32 arg1, s32 arg2, s32 arg3) {
    void *v0;

    v0 = Heap_Alloc(heapID, 0x38U);
    memset(v0, 0, 0x38U);
    *(s32 *)v0 = arg1;
    *(s32 *)((u8 *)v0 + 8) = arg2;
    *(s32 *)((u8 *)v0 + 0x30) = arg3;
    *(s32 *)((u8 *)v0 + 4) = 1;
    return v0;
}
