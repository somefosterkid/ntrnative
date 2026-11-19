#include "global.h"

#include "constants/heap.h"

#include "heap.h"

void sub_0205B380(void *);

static void sub_0205B3A0(u8 *param_1);

static void sub_0205B3A0(u8 *param_1) {
    s32 iVar1;

    iVar1 = 0;
    do {
        sub_0205B380(param_1);
        iVar1 = iVar1 + 1;
        param_1 = param_1 + 0x1c;
    } while (iVar1 < 0x1e);
    return;
}

void sub_0205B35C(void *param0);

void *sub_0205B3B8(enum HeapID param_1);

void *sub_0205B3B8(enum HeapID param_1) {
    void *pvVar1;

    pvVar1 = Heap_Alloc(param_1, 0x350);
    sub_0205B35C(pvVar1);
    return pvVar1;
}

void sub_0205B3CC(void *param_1);

void sub_0205B3CC(void *param_1) {
    sub_0205B3A0((u8 *)param_1);
    Heap_Free(param_1);
    return;
}
