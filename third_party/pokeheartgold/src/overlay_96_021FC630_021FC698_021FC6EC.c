#include "global.h"

#include "error_handling.h"

s32 ov96_021EB57C(void *);

s32 ov96_021EB588(void *, s32 *);

void *ov96_021EB594(void *);

s32 ov96_021FC758(void *);

void ov96_021FC6EC(s32 arg0, u32 arg1, s32 arg2);

void ov96_021FC6EC(s32 arg0, u32 arg1, s32 arg2) {
    s32 sp4[3];
    s32 sp0;
    s32 temp_r1;
    u8 var_r4;
    void *temp_r0;
    void *temp_r5;
    if (arg1 >= 3U) {
        GF_AssertFail();
    }
    sp0 = arg0 + (0x2C * arg1);
    var_r4 = 0;
    do {
        temp_r1 = var_r4 * 8;
        temp_r5 = (void *)(sp0 + temp_r1);
        if ((*((s32 *)(sp0 + temp_r1))) != 0) {
            temp_r0 = ov96_021EB594(*((void **)(((u8 *)temp_r5) + 4)));
            *((VecFx32 *)((u8 *)sp4)) = *((VecFx32 *)((u8 *)temp_r0));
            sp4[1] += arg2 << 0xC;
            ov96_021EB588(*((void **)(((u8 *)temp_r5) + 4)), sp4);
            if (ov96_021EB57C(*((void **)(((u8 *)temp_r5) + 4))) == 0) {
                ov96_021FC758(temp_r5);
            }
        }
        var_r4 += 1;
    } while (((u32)var_r4) < 5U);
}
