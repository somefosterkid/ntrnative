#include "global.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

void ov96_021FBDBC(UnkStruct_ov96_021F94A8 *arg0);

void *ov96_021FBE20(s32 param_1, enum HeapID param_2);

// Counts frames (frameCount) and race time (raceTime: frames while any lane has not finished).
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021FBDBC(UnkStruct_ov96_021F94A8 *arg0) {
    s32 v0;

    arg0->frameCount++;
    for (v0 = 0; v0 < 3; v0++) {
        if (arg0->rows[v0].unk_8 == 0) {
            arg0->raceTime++;
            return;
        }
    }
}
#else
// DS form: the work pointer steps one row (0x6C) per lane, 0xE8 == rows[0].unk_8.
void ov96_021FBDBC(UnkStruct_ov96_021F94A8 *arg0) {
    s32 v0;
    u8 *v1;

    v1 = (u8 *)arg0;
    arg0->frameCount++;
    v0 = 0;
loop_1:
    if (*(v1 + 0xE8) == 0) {
        arg0->raceTime++;
        return;
    }
    v0 += 1;
    v1 += sizeof(UnkStruct_ov96_021FAB24);
    if (v0 >= 3) {
        return;
    }
    goto loop_1;
}
#endif

s32 ov96_021FBDEC(s32 arg0);

s32 ov96_021FBDEC(s32 arg0) {
    return (0x1E * (arg0 / 30)) + ((s32)(0x1E * ((s32)(0xA * (arg0 % 30)) / 30)) / 10);
}

void *ov96_021FBE20(s32 param_1, enum HeapID param_2) {
    void *destp;

    destp = Heap_Alloc(param_2, param_1 * 0x10);
    MIi_CpuClearFast(0, destp, param_1 * 0x10);
    return destp;
}
