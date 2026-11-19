#include "global.h"

#include "constants/heap.h"

#include "heap.h"

void *ov96_021FBE20(s32 param_1, enum HeapID param_2);

u32 *ov96_021FC028(enum HeapID param_1);

u32 *ov96_021FC028(enum HeapID param_1) {
    u32 *dest;
    void *pvVar1;
    s32 iVar2;
    u32 *puVar3;

    dest = Heap_Alloc(param_1, 0xc24);
    MI_CpuFill8(dest, 0, 0xc24);
    iVar2 = 0;
    puVar3 = dest;
    do {
        pvVar1 = ov96_021FBE20(4, param_1);
        *puVar3 = (u32)(pvVar1);
        iVar2 = iVar2 + 1;
        puVar3 = puVar3 + 2;
    } while (iVar2 < 3);
    return dest;
}
