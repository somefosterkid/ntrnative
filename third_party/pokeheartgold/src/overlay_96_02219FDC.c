#include "global.h"

#include "sys_task.h"

typedef struct UnkStruct_ov96_02219FE4 {
    u8 unk_0[0x8];
    u8 unk_8;
    u8 unk_9[0x23];
    u8 unk_2C;
} UnkStruct_ov96_02219FE4;

void Heap_Free(void *ptr);

void ov96_02219FDC(void *ptr);

s32 ov96_0221A400(void *);
void ov96_02219FE4(UnkStruct_ov96_02219FE4 *arg0);
void ov96_0221A00C(void *arg0, s32 arg1, void *arg2);
u32 ov96_0221A034(VecFx32 *param_1, VecFx32 *param_2, s32 param_3);
s32 ov96_02215FA0(VecFx32 *);
s32 ov96_0221A05C(VecFx32 *arg0, VecFx32 *arg1, s32 arg2);

void ov96_02219FDC(void *ptr) {
    Heap_Free(ptr);
}

void ov96_02219FE4(UnkStruct_ov96_02219FE4 *arg0) {
    s32 v0;
    void *v1;

    v0 = 0;
    if ((s32) arg0->unk_2C > 0) {
        v1 = (void *)&arg0->unk_8;
        do {
            ov96_0221A400(v1);
            v0 += 1;
            v1 += 0xC;
        } while (v0 < (s32) arg0->unk_2C);
    }
}

void ov96_0221A00C(void *dest, s32 arg1, void *arg2) {
    MI_CpuFill8(dest, 0U, 0xCU);
    *(void **)dest = arg2;
    *(s32 *)((u8 *)dest + 8) = (s32)(((*(s32 *)((u8 *)dest + 8) & ~3) | (3 & arg1)) & ~0xC);
}

u32 ov96_0221A034(VecFx32 *param_1, VecFx32 *param_2, s32 param_3) {
    fx32 fVar1;
    VecFx32 VStack_18;

    VEC_Subtract(param_2, param_1, &VStack_18);
    fVar1 = VEC_Mag(&VStack_18);
    if (fVar1 <= param_3) {
        return 1;
    }
    return 0;
}

s32 ov96_0221A05C(VecFx32 *b, VecFx32 *a, s32 arg2) {
    VecFx32 v0;

    VEC_Subtract(a, b, &v0);
    VEC_Normalize(&v0, &v0);
    if (arg2 == ov96_02215FA0(&v0)) {
        return 1;
    }
    return 0;
}
