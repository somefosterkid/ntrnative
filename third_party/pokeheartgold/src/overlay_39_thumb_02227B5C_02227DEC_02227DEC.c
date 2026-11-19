#include "global.h"
#include "constants/heap.h"

void *Heap_Alloc(u32 heapID, u32 size);

extern OSHeapHandle _0222AB80;

void *ov39_02227DEC(enum HeapID heapID);

void *ov39_02227DEC(enum HeapID heapID) {
    void *v0;
    void *v1;
    void *v2;
    int v3 = 0x2000;

    v0 = Heap_Alloc(heapID, v3);
    v2 = v0;
    v1 = (void *)((u8 *)v0 + v3);
    v0 = OS_InitAlloc(OS_ARENA_MAIN, v0, v1, 1);

    OS_SetArenaLo(OS_ARENA_MAIN, v0);

    v0 = (void *)(((u32)(v0) + 32 - 1) & ~(32 - 1));
    v1 = (void *)(((u32)(v1) + 32 - 1) & ~(32 - 1));

    _0222AB80 = OS_CreateHeap(OS_ARENA_MAIN, v0, v1);
    OS_SetCurrentHeap(OS_ARENA_MAIN, _0222AB80);

    return v2;
}
