#include "global.h"

#include "overlay_96_arena_cpu.h"

s32 _s32_div_f(s32, u32);

s32 ov96_021F39F0(Ov96ArenaCpu *cpu, Ov96ArenaCpuMon *mon, s32 cell) {
    s32 iVar1;
    s32 iVar2;
    s32 iVar3;
    s32 i;
    s32 iVar5;

    if (mon->cell == -1) {
        return 0;
    }
    iVar3 = 0;
    iVar5 = mon->idx;
    i = 0;
    do {
        if ((i != iVar5) && (cell == cpu->mons[i].cell)) {
            if (mon->unk_04 < cpu->mons[i].unk_04) {
                iVar1 = _s32_div_f(iVar5, 3);
                iVar2 = _s32_div_f(cpu->mons[i].idx, 3);
                if (iVar1 == iVar2) {
                    iVar3 = iVar3 + -1;
                } else {
                    iVar3 = iVar3 + -2;
                }
            } else {
                iVar3 = iVar3 + -1;
            }
        }
        i = i + 1;
    } while (i < 12);
    return iVar3;
}
