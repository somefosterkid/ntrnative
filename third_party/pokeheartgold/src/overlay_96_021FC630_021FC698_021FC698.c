#include "global.h"

#include "error_handling.h"

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB588(void *, void *);

void ov96_021FC698(void *arg0, u32 arg1, void *arg2);

void ov96_021FC698(void *arg0, u32 arg1, void *arg2) {
    s32 temp_r0;
    void *temp_r4;
    void *temp_r5;

    if (arg1 >= 3U) {
        GF_AssertFail();
    }
    temp_r4 = (void *)(arg0 + (0x2C * arg1));
    temp_r0 = *(u32 *)((u8 *)temp_r4 + 0x28) * 8;
    temp_r5 = temp_r4 + temp_r0;
    if (*(s32 *)(temp_r4 + temp_r0) == 0) {
        ov96_021EB52C(*(void **)((u8 *)temp_r5 + 4), 1, 1);
        ov96_021EB564(*(void **)((u8 *)temp_r5 + 4), 1);
        ov96_021EB588(*(void **)((u8 *)temp_r5 + 4), arg2);
        *(s32 *)temp_r5 = 1;
        *(u32 *)((u8 *)temp_r4 + 0x28) = (u32)((u32)(*(u32 *)((u8 *)temp_r4 + 0x28) + 1) % 5U);
        return;
    }
    GF_AssertFail();
}
