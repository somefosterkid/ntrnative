#include "global.h"

#include "heap.h"
#include "overlay_42.h"

u32 *ov45_02230498(u32, s32, enum HeapID, s32);

s32 ov49_022591D8(void *);

void *ov49_02258AB4(u32 arg0, s16 arg1, void *arg2, enum HeapID arg3, s32 arg4);

void *ov49_02258AB4(u32 a0, s16 arg1, void *arg2, enum HeapID heapID, s32 arg4) {
    enum HeapID v1;
    void *v0;
    u32 size;
    void *v2;
    void *v3;

    v0 = arg2;
    v1 = heapID;
    v2 = Heap_Alloc(heapID, 0x28U);
    memset(v2, 0, 0x28U);
    *(UnkStruct_ov42_022280A8 **)v2 = ov42_02228010(a0, v1);
    *(void **)((u8 *)v2 + 4) = ov45_02230498(a0, arg1, v1, arg4);
    *(UnkStruct_ov42_02229A40 **)((u8 *)v2 + 0x10) = ov42_02229A40(0x20, v1);
    size = a0 * 0x28;
    *(s16 *)((u8 *)v2 + 0xC) = (s16)a0;
    v3 = Heap_Alloc(v1, size);
    *(void **)((u8 *)v2 + 8) = v3;
    memset(v3, 0, size);
    *(void **)((u8 *)v2 + 0x14) = v0;
    *(s16 *)((u8 *)v2 + 0xE) = arg1;
    ov49_022591D8(v2);
    return v2;
}
