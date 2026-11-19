#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_021_f7684.h"

UnkStruct_ov96_021F7684 *ov96_021F7684(enum HeapID arg0, u8 arg1, s32 arg2, u8 *arg3, PokeathlonCourseData *arg4);

#if defined(SDK_PORT) || defined(NONMATCHING)
UnkStruct_ov96_021F7684 *ov96_021F7684(enum HeapID arg0, u8 arg1, s32 arg2, u8 *arg3, PokeathlonCourseData *arg4) {
    UnkStruct_ov96_021F7684 *sp8;
    PokeathlonMonKinds *temp_r0_5;
    s32 spC;
    s32 idx;
    s32 j;

    sp8 = Heap_Alloc(arg0, sizeof(UnkStruct_ov96_021F7684));
    MI_CpuFill8(sp8, 0U, sizeof(UnkStruct_ov96_021F7684));
    sp8->count = arg1;
    sp8->unk_21 = (u8)(4 - arg1);
    sp8->first = sp8->unk_21;
    sp8->interval = 3;
    sp8->dataCopyArea = arg3;
    for (spC = 0; spC < sp8->count; spC++) {
        idx = (u8)(sp8->first + spC);
        for (j = 0; j < 3; j++) {
            temp_r0_5 = ov96_021E60D8(arg4, idx, j);
            sp8->unk_8[idx] += temp_r0_5->statIdx[2];
            sp8->unk_C[idx] += temp_r0_5->statIdx[4];
            sp8->unk_10[idx][j] = sp8->unk_8[idx];
        }
    }
    return sp8;
}
#else
// DS form: one byte pointer per participant walks unk_8 / unk_C / unk_10 by raw offsets (8, 0xC, idx * 3 + 0x10).
UnkStruct_ov96_021F7684 *ov96_021F7684(enum HeapID arg0, u8 arg1, s32 arg2, u8 *arg3, PokeathlonCourseData *arg4) {
    s32 spC;
    UnkStruct_ov96_021F7684 *sp8;
    s32 sp4;
    u8 *sp0;
    u8 *var_r7;
    u8 *var_r6;
    s32 var_r4;
    u8 *var_r5;
    u8 temp_r1;
    UnkStruct_ov96_021F7684 *temp_r0;
    u8 temp_r0_3;
    u8 *temp_r0_4;
    PokeathlonMonKinds *temp_r0_5;
    s32 temp_r0_2;
    s32 count;

    count = arg1;
    sp8 = Heap_Alloc(arg0, sizeof(UnkStruct_ov96_021F7684));
    temp_r0 = sp8;
    MI_CpuFill8(temp_r0, 0U, sizeof(UnkStruct_ov96_021F7684));
    temp_r0->count = (u8)count;
    temp_r0->unk_21 = (u8)(4 - count);
    temp_r0->first = temp_r0->unk_21;
    temp_r0->interval = 3;
    temp_r0->dataCopyArea = arg3;
    spC = 0;
    if ((s32)temp_r0->count > 0) {
        do {
            var_r4 = 0;
            temp_r0_3 = sp8->first + spC;
            sp4 = (s32)temp_r0_3;
            var_r5 = (u8 *)sp8 + (temp_r0_3 * 3);
            temp_r0_4 = (u8 *)sp8 + temp_r0_3;
            var_r6 = temp_r0_4;
            var_r7 = temp_r0_4;
            sp0 = temp_r0_4;
            var_r6 += 8;
            var_r7 += 0xC;
loop_2:
            temp_r0_5 = ov96_021E60D8(arg4, sp4, var_r4);
            temp_r1 = temp_r0_5->statIdx[2];
            var_r4 += 1;
            *var_r6 += temp_r1;
            *var_r7 += temp_r0_5->statIdx[4];
            *(var_r5 + 0x10) = *(sp0 + 8);
            var_r5 += 1;
            if (var_r4 < 3) {
                goto loop_2;
            }
            temp_r0_2 = spC + 1;
            spC = temp_r0_2;
        } while (temp_r0_2 < (s32)sp8->count);
    }
    return sp8;
}
#endif
