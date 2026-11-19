#include "global.h"

#include "overlay_96_arena_cpu.h"

extern u32 ov96_0221DBC8;

extern u16 *ov96_0221DBB8[];


s32 ov96_021F333C(s32 param_1, Ov96ArenaCpuItem *param_2) {
    s32 iVar1;
    s32 iVar2;
    u32 uVar3;
    s32 iVar4;
    s32 iVar5;
    uVar3 = 0;
    iVar2 = 0;
    iVar4 = 0;
    do {
        s32 new_var2;
        iVar1 = *((s32 *)((&ov96_0221DBC8) + (new_var2 = param_1)));
        iVar1 = iVar1 + iVar4;
        iVar5 = *((s32 *)((*((s32 *)((&ov96_0221DBC8) + new_var2))) + iVar4));
        if (iVar5 == 0) {
            param_2->radius = 0;
            param_2->x = 0;
            param_2->y = 0;
        } else {
            param_2->radius = *(ov96_0221DBB8[iVar5 - 1]);
            param_2->x = *((u16 *)(iVar1 + 4));
            param_2->y = *((u16 *)(iVar1 + 6));
            uVar3 = (uVar3 + 1) & 0xff;
        }
        iVar2 = iVar2 + 1;
        iVar4 += 8;
        param_2++;
    } while (iVar2 < 4);
    return uVar3;
}
