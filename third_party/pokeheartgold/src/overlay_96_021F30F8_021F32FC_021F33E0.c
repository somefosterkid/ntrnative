#include "global.h"

#include "overlay_96_arena_cpu.h"

void ov96_021F33E0(Ov96ArenaCpu *cpu, s32 idx, s32 arg2, u16 *arg3, VecFx32 *pos, s32 *state, VecFx32 *target, struct Ov96SpriteSetElem *spr) {
    cpu->mons[idx].unk_04 = arg2;
    cpu->mons[idx].unk_08 = arg3;
    cpu->mons[idx].pos = pos;
    cpu->mons[idx].state = state;
    cpu->mons[idx].cell = -1;
    cpu->mons[idx].idx = idx;
    cpu->mons[idx].spr = spr;
    if (idx / 3 >= cpu->unk_4E9) {
        cpu->mons[idx].target = target;
        return;
    }
    cpu->mons[idx].target = NULL;
}
