#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "heap.h"
#include "unk_02033AE0.h"

u32 ov96_021E872C(s32 param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5, fx32 *param_6);

u32 ov96_021E872C(s32 param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5, fx32 *param_6) {
    fx32 fVar1;
    VecFx32 VStack_10;
    VecFx32 VStack_1c;
    VecFx32 VStack_28;
    VStack_10.x = param_1 << 0xc;
    VStack_10.y = param_2 << 0xc;
    VStack_1c.x = param_3 << 0xc;
    VStack_10.z = 0;
    VStack_1c.y = param_4 << 0xc;
    VStack_1c.z = 0;
    VEC_Subtract(&VStack_10, &VStack_1c, &VStack_28);
    fVar1 = VEC_Mag(&VStack_28);
    *param_6 = fVar1;
    if (fVar1 < (param_5 * 0x1000)) {
        return 1;
    }
    return 0;
}

void *ov96_021E883C(u32 param_1, u32 param_2, void *param_3, enum HeapID param_4);

void *ov96_021E8770(int a0, int a1, PokeathlonCourseData *data, int a3, enum HeapID heapId) {
    u16 *puVar1;
    void *pvVar2;

    puVar1 = Heap_Alloc(heapId, 8);
    *puVar1 = (s16)a3;
    puVar1[1] = 0;
    *(u32 *)(puVar1 + 2) = 0;
    if (a3 == 1) {
        sub_0203410C((void *)a0, a1, data);
    } else {
        pvVar2 = (void *)((ov96_021E883C(a0, a1, (data), heapId)));
        *(void **)(puVar1 + 2) = pvVar2;
    }
    return puVar1;
}
