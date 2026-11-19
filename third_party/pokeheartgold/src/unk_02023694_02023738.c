#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "billboard.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: host-layout copy of the asm's .bss object (asm/unk_02023694.s: 8 bytes on the DS, 4-aligned: the pointer and the
// count sat misaligned). The asm label stays for the DS build.
BillboardLists _021D2208;
#endif

void sub_02023694(BillboardList *list);

void BillboardLists_Create(s32 count, enum HeapID heapID);

void BillboardLists_Create(s32 count, enum HeapID heapID) {
    s32 i;

    if (_021D2208.lists != NULL) {
        GF_AssertFail();
    }
    _021D2208.lists = Heap_Alloc(heapID, sizeof(BillboardList) * count);
    i = 0;
    _021D2208.count = count;
    if (count > 0) {
        do {
            sub_02023694(&_021D2208.lists[i]);
            i += 1;
        } while (i < count);
    }
}
