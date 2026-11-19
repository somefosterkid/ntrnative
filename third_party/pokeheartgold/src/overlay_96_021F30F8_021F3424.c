#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_96_arena_cpu.h"

void ov96_021F3424(Ov96ArenaCpu *cpu) {
    Heap_Free(cpu);
}

u8 ov96_021F342C(Ov96ArenaCpu *cpu, s32 arg1) {
    s32 v1;
    u8 v0;
    u8 v2;
    u8 v3;

    v3 = 12;
    if (cpu->unk_4EA == 4) {
        return 12;
    }
    cpu->timer++;
    if (cpu->timer >= cpu->interval) {
        cpu->active = TRUE;
        v2 = 0;
        cpu->timer = 0;
        while (v2 < cpu->count) {
            cpu->unk_4B4[v2] = 0;
            v2++;
        }
    }
    if (cpu->active) {
        v0 = cpu->unk_4E9;
        v1 = (2 - cpu->unk_4E8) + ((v0 + cpu->unk_4E5) * 3);
        if (v1 / 3 < (s32)v0) {
            GF_AssertFail();
        }
        ov96_021F350C(cpu, v1, arg1);
        v3 = v1;
        cpu->unk_4E5++;
        if (cpu->unk_4E5 >= cpu->count) {
            cpu->unk_4E5 = 0;
            cpu->unk_4E8 = (cpu->unk_4E8 + 1) % 3;
            cpu->active = FALSE;
        }
    }
    return v3;
}
