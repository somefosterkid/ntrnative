#include "global.h"

#include "heap.h"
#include "overlay_92.h"

s32 ov92_0225EBE0(void *);

void *ov92_0225EB88(s32 arg0, s32 arg1, s32 arg2) {
    void *v0;

    v0 = Heap_Alloc(HEAP_ID_113, 0x2BBCU);
    memset(v0, 0, 0x2BBCU);
    *(s32 *)((u8 *)v0 + 4) = arg0;
    *(s32 *)v0 = arg1;
    *(s32 *)((u8 *)v0 + 8) = 0;
    *(s32 *)((u8 *)v0 + 0xC) = 0;
    *(s32 *)((u8 *)v0 + 0x14) = arg2;
    *(s32 *)((u8 *)v0 + 0x2BB8) = 0;
    *(s32 *)((u8 *)v0 + 0x2228) = (s32) * (s32 *)((u8 *)v0 + 0x2AEC);
    *(void **)((u8 *)v0 + 0x222C) = (void *)(v0 + 0x2AEC);
    *(s32 *)((u8 *)v0 + 0x2B94) = 0;
    ov92_0225EBE0(v0);
    return v0;
}
