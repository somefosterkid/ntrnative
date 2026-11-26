#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_bg_scroll_context.h"

s32 ov80_0223B4A0(void *, void *, s32 *, s32 *);

s32 sub_02014A08(enum HeapID, void *, void *);

extern s32 ov80_0223B528;

extern s32 ov80_0223B53C;

typedef struct Words2 {
    u32 w[8];
} Words2;

void *ov80_0223B544(void *arg0, Words2 *arg1, enum HeapID arg2);

void *ov80_0223B544(void *arg0, Words2 *arg1, enum HeapID arg2) {
    void *sp0;
    s32 temp_r0_2;
    s32 var_r5;
    void *temp_r0;
    Words2 *var_r6;

    temp_r0 = Heap_Alloc(arg2, 0x3024U);
    sp0 = temp_r0;
    memset(temp_r0, 0, 0x3024U);
    if (temp_r0 == NULL) {
        GF_AssertFail();
    }
    temp_r0_2 = sub_02014A08(arg2, sp0 + 0x20, sp0 + 0x1820);
    *(s32 *)((u8 *)sp0 + 0x1C) = temp_r0_2;
    if (temp_r0_2 == 0) {
        GF_AssertFail();
    }
    var_r5 = 0;
    *(void **)((u8 *)sp0 + 0x3020) = arg0;
    var_r6 = (Words2 *)sp0;
    do {
        var_r6[1] = *arg1;
        var_r6[0xC1] = *arg1;
        var_r5 += 1;
        var_r6 = var_r6 + 1;
    } while (var_r5 < 0xC0);
    ov80_0223B4A0(sp0, sp0, &ov80_0223B528, &ov80_0223B53C);
    return sp0;
}
