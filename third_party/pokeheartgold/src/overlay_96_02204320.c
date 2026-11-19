#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "math_util.h"

void ov96_02204320(s32 arg0, VecFx32 *arg1);

void ov96_02204320(s32 arg0, VecFx32 *arg1) {
    MtxFx43 v0;
    int v1;

    v1 = ((s32)(u16)(0xB6 * (arg0 - ((s32)LCRandom() % (s32)(arg0 * 2)))) >> 4) * 2;
    MTX_RotY43_(&v0, (s32)FX_SinCosTable_[v1], (s32)FX_SinCosTable_[v1 + 1]);
    MTX_MultVec43(arg1, &v0, arg1);
}

void *ov96_02204364(enum HeapID param_1, s8 param_2);

void *ov96_02204364(enum HeapID param_1, s8 param_2) {
    void *dest;

    dest = Heap_Alloc(param_1, 0xcc);
    MI_CpuFill8(dest, 0, 0xcc);
    *(s8 *)((u8 *)dest + 0xc4) = param_2;
    *(s8 *)((u8 *)dest + 0xc9) = 4 - param_2;
    *(u8 *)((u8 *)dest + 200) = *(u8 *)((u8 *)dest + 0xc9);
    *(u8 *)((u8 *)dest + 199) = 0x28;
    *(u8 *)((u8 *)dest + 0xc6) = *(u8 *)((u8 *)dest + 199);
    return dest;
}
