#include "global.h"

#include "heap.h"
#include "overlay_80_02239D74.h"

struct UnkStruct_ov80_02239DB8_ptr {
    void *unk_0;
    void *unk_4;
};

void *ov80_02239DD0(void *);

void *ov80_02239F48(void *);

void *ov80_02239D74(void *arg0, void *arg1) {
    void *dest;

    dest = Heap_Alloc(HEAP_ID_101, 0xCU);
    MI_CpuFill8(dest, 0U, 0xCU);
    *(void **)dest = ov80_02239DD0(arg0);
    *(void **)((u8 *)dest + 4) = ov80_02239F48(arg1);
    G2_SetBlendAlpha(4, 0x1F, 8, 9);
    return dest;
}

s32 ov80_02239EC4(void *);

s32 ov80_02239F94(void *);

void ov80_02239DB8(UnkStruct_ov80_02239DB8_ptr *ptr) {
    ov80_02239EC4(ptr->unk_0);
    ov80_02239F94(ptr->unk_4);
    Heap_Free(ptr);
}
