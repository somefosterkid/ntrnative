#include "global.h"
#include "overlay_27_ov27task.h"

extern u16 ov27_0225CF94[][2];

extern s32 ov27_0225CFC8;

void ov27_0225B398(Ov27Task *arg0, s32 arg1);

void ov27_0225B398(Ov27Task *arg0, s32 arg1) {
    s32 sp0;
    s32 var_r2;
    s32 var_r4;
    u32 var_r5;
    u8 temp_r1;
    u16 temp_r0;

    sp0 = arg1;
    if (arg1 >= 7) {
        GF_AssertFail();
    }
    var_r4 = 0;
    var_r5 = 0;
    do {
        temp_r1 = *(u8 *)(var_r4 + (((u8 *)&ov27_0225CFC8) + ((arg0->unk_51C_1) * 8)));
        temp_r0 = ov27_0225CF94[temp_r1][0];
        var_r2 = 0;
        if ((temp_r1 != 0xD) && (temp_r0 != 0xFFFF)) {
            if (var_r4 == sp0) {
                var_r2 = 0x10;
            }
            GXS_LoadOBJPltt(&arg0->unk_4CC[var_r2], var_r5, 0x20U);
        }
        var_r4 += 1;
        var_r5 += 0x20;
    } while (var_r4 < 7);
}
