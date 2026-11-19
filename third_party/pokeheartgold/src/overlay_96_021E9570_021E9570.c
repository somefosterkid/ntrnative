#include "global.h"

#include "error_handling.h"

void ov96_021E9570(s32 *arg0, u8 arg1);

void ov96_021E9570(s32 *arg0, u8 arg1) {
    s32 sp4[4];
    s32 sp0;
    s32 *var_r3;
    s32 *var_r4;
    s32 *var_r1;
    s32 var_r0;
    s32 var_r0_2;
    var_r4 = arg0;
    sp0 = arg1;
    var_r1 = (s32 *)(((u8 *)sp4) + 0);
    var_r1[0] = 0;
    var_r1[1] = 0;
    var_r1[2] = 0;
    var_r1[3] = 0;
    if (sp0 >= 0xA) {
        GF_AssertFail();
    }
    var_r0 = 0;
    if ((*var_r4) > 0) {
        s32 *var_r1;
        s32 *var_r6;
        s32 var_r7;
        var_r7 = 0x51;
        var_r1 = var_r4;
        var_r7 = 0x144;
        do {
            void *temp_r2;
            s32 temp_r3;
            s32 var_r5;
            var_r6 = &sp4[0];
            temp_r2 = *((void **)(((u8 *)var_r1) + 0x144));
            var_r0 += 1;
            temp_r3 = *((s32 *)temp_r2);
            var_r5 = temp_r3 * 4;
            var_r1 = (void *)(((u8 *)var_r1) + 4);
            var_r6[temp_r3] = var_r6[temp_r3] + (*((u16 *)(((u8 *)temp_r2) + 0xA)));
        } while (var_r0 < ((s32)(*var_r4)));
    }
    var_r0_2 = 0;
    var_r4 = (void *)(((u8 *)var_r4) + (sp0 * 0x10));
    var_r3 = &sp4[0];
    do {
        var_r0_2 += 1;
        *((s32 *)(((u8 *)var_r4) + 0x174)) = (s32)(*var_r3);
        var_r3 = (void *)(((u8 *)var_r3) + 4);
        var_r4 = (void *)(((u8 *)var_r4) + 4);
    } while (var_r0_2 < 4);
}
