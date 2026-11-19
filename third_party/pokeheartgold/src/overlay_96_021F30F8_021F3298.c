#include "global.h"

#include "error_handling.h"

s32 ov96_021EB52C(void *, s32, s32);

void ov96_021F3298(void *arg0, s32 arg1);

void ov96_021F3298(void *arg0, s32 arg1) {
    s32 v0;
    void *v1;

    v1 = arg0;
    v0 = 0;
    do {
        if (arg1 == *(s32 *)((u8 *)v1 + 8)) {
            *(s32 *)((u8 *)v1 + 8) = 0xF;
            *(s32 *)((u8 *)v1 + 4) = 0;
            *(s32 *)v1 = 0;
            ov96_021EB52C(*(void **)((u8 *)v1 + 0xC), 0, 0);
        }
        v0 += 1;
        v1 += 0x10;
    } while (v0 < 8);
}

s32 ov96_021F32C4(u32 arg0);

s32 ov96_021F32C4(u32 arg0) {
    s32 v0;

    v0 = 0;
    switch (arg0) {
    case 1:
        v0 = 1;
        break;
    case 2:
        v0 = 2;
        break;
    case 3:
        v0 = 3;
        break;
    case 5:
        v0 = 4;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return v0;
}
