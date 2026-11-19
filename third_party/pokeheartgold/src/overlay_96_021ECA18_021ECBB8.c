#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_96_app_021ed1d0.h"

Ov96PlayerSlot *ov96_021ECBB8(enum HeapID param_1, ManagedSprite **param_2);

Ov96PlayerSlot *ov96_021ECBB8(enum HeapID param_1, ManagedSprite **param_2) {
    Ov96PlayerSlot *dest;
    ManagedSprite *uVar1;
    u32 uVar8;
    s32 iVar2;
    s32 iVar3;
    Ov96PlayerSlot *puVar4;
    s32 iVar5;
    ManagedSprite **puVar6;
    ManagedSprite **puVar7;
    dest = Heap_Alloc(param_1, 4 * sizeof(Ov96PlayerSlot));
    MI_CpuFill8(dest, 0, 4 * sizeof(Ov96PlayerSlot));
    iVar2 = 0;
    iVar5 = 5;
    puVar4 = dest;
    do {
        iVar3 = 0;
        puVar6 = param_2 + iVar5;
        puVar7 = puVar4->sprites;
        do {
            uVar1 = *puVar6;
            iVar3 = iVar3 + 1;
            puVar6 = puVar6 + 1;
            *puVar7 = uVar1;
            puVar7 = puVar7 + 1;
        } while (iVar3 < 4);
        iVar2 = iVar2 + 1;
        uVar8 = 4;
        puVar4 = puVar4 + 1;
        iVar5 = iVar5 + uVar8;
    } while (iVar2 < 4);
    return dest;
}
