#include "global.h"

#include "math_util.h"

#include "overlay_96_app_02210C98.h"

extern s32 ov96_0221D438;

extern s32 ov96_0221D43A;

void ov96_02213E60(Ov96App_02210C98 *arg0, Ov96App_02210C98_Mover *arg1);

void ov96_02213E60(Ov96App_02210C98 *arg0, Ov96App_02210C98_Mover *arg1) {
    u8 sp0[16];
    u8 temp_r6;
    u32 var_r3;
    u8 temp_r0;
    u8 var_r4;
    u8 temp_r5;

    temp_r6 = arg0->unk_62C[0].unk_42;
    var_r4 = 0;
    var_r3 = 0;
    do {
        if ((temp_r6 != var_r3) && (arg0->unk_62C[1].unk_42 != var_r3)) {
            temp_r5 = var_r4;
            var_r4 += 1;
            sp0[temp_r5] = var_r3;
        }
        var_r3 += 1;
    } while ((s32)var_r3 < 0xF);
    temp_r0 = sp0[(u8)((s32)LCRandom() % (s32)var_r4)];
    arg1->unk_8.x = (s32)(*(s16 *)(&ov96_0221D438 + temp_r0) << 0xC);
    arg1->unk_8.y = (s32)(*(s16 *)(&ov96_0221D43A + temp_r0) << 0xC);
    arg1->unk_42 = temp_r0;
}
