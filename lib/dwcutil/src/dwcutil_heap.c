// libdwcutil's heap.c (Black's ov016 0x021d6360-0x021d64b8): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order. Named
// dwcutil_heap.c here: dsd takes each file name once, and lib/gfl/src/heap.c has heap.c.
#include "dwcutil.h"

static NNSiFndHeapHead * data_ov016_021de684;
void *DWCi_HEAPlAlloc(u32 param_1, s32 param_2);

void DWCi_HEAPlInit(void *address) {
    MI_CpuFill8(address, 0, 0x40000);
    data_ov016_021de684 = NNS_FndCreateExpHeapEx(address, 0x40000, 0);
    if (data_ov016_021de684 != NULL) {
        return;
    }
    OS_Terminate();
}

void DWCi_HEAPlEnd(void) {
    NNS_FndDestroyExpHeap(data_ov016_021de684);
    data_ov016_021de684 = NULL;
}

void *DWCi_HEAPlAlloc(u32 size, s32 alignment) {
    void *ptr;
    u32 mask;

    mask = OS_DisableIrqMask(1U);
    ptr = NNS_FndAllocFromExpHeapEx(data_ov016_021de684, size, alignment);
    if (ptr == NULL) {
        OS_Terminate();
    }
    OS_EnableIrqMask(mask);
    return ptr;
}

void *DWCi_HEAPlAllocEx(u32 size, s32 alignment) {
    void *dest;

    dest = DWCi_HEAPlAlloc(size, alignment);
    MI_CpuFill8(dest, 0, size);
    return dest;
}

void DWCi_HEAPlFree_(u32 *mem) {
    u32 mask;

    mask = OS_DisableIrqMask(1);
    if ((void *)*mem == NULL) {
        return;
    }
    NNS_FndFreeToExpHeap(data_ov016_021de684, (void *)*mem);
    OS_EnableIrqMask(mask);
    *mem = 0;
}

void DWCi_HEAPlFree2(void *mem) {
    u32 mask;

    mask = OS_DisableIrqMask(1);
    if (mem == NULL) {
        return;
    }
    NNS_FndFreeToExpHeap(data_ov016_021de684, mem);
    OS_EnableIrqMask(mask);
}
