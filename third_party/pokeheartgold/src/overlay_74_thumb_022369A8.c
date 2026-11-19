#include "global.h"
#include "constants/heap.h"

typedef enum {
    DWC_ALLOCTYPE_AUTH,
    DWC_ALLOCTYPE_AC,
    DWC_ALLOCTYPE_BM,
    DWC_ALLOCTYPE_UTIL,
    DWC_ALLOCTYPE_BASE,
    DWC_ALLOCTYPE_GS,
    DWC_ALLOCTYPE_ND,

    DWC_ALLOCTYPE_ENC,
    DWC_ALLOCTYPE_MAIL,

    DWC_ALLOCTYPE_LAST
} DWCAllocType;

void *Heap_Alloc(u32 heapID, u32 size);

void *ov74_022369A8(DWCAllocType param0, u32 param1, int param2);

void Heap_Free(void *ptr);
void ov74_022369C8(DWCAllocType param0, void *param1, u32 param2);

void *ov74_022369A8(DWCAllocType param0, u32 param1, int param2) {
#pragma unused(param0, param2)
    void *v0;
    u32 *v1;

    param1 = (((param1 + sizeof(void *)) + 32) & ~31) + 32;

    v0 = Heap_Alloc((enum HeapID)84, param1);
    v1 = (u32 *)(((u32)v0 + 32) & ~31);

    v1--;
    *v1 = (u32)v0;
    v1++;

    return v1;
}

void ov74_022369C8(DWCAllocType param0, void *param1, u32 param2) {

    u32 *v0;

    if (!param1) {
        return;
    }

    v0 = (u32 *)((u8 *)param1 - 4);
    Heap_Free((void *)*v0);
}
