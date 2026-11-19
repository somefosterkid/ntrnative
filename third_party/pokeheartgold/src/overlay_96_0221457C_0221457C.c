#include "global.h"

#include "overlay_96_app_02210C98.h"

s32 ov96_021EABA8(struct Ov96SpriteSetElem *, s32);

s32 ov96_0221454C(void *elem1, void *elem2);

void ov96_0221457C(Ov96App_02210C98 *arg0, u8 *arg1, s32 arg2);

void ov96_0221457C(Ov96App_02210C98 *arg0, u8 *arg1, s32 arg2) {
    u16 sp8[9][2];
    Ov96App_02210C98 *sp4;
    s32 var_r4;
    u8 temp_r6;
    u8 var_r5;
    u8 var_r5_2;

    var_r5 = 0;
    sp4 = arg0;
    var_r4 = 0;
    do {
        if ((var_r4 / 3) != arg2) {
            sp8[var_r5][0] = var_r4;
            sp8[var_r5][1] = arg1[var_r4 + 0xC];
            var_r5 += 1;
        }
        var_r4 = (s16)(u8)(var_r4 + 1);
    } while ((u32)var_r4 < 0xCU);
    MATH_QSort(&sp8[0], 9U, 4U, ov96_0221454C, sp4->unk_81C);
    var_r5_2 = 0;
    do {
        temp_r6 = (u8)sp8[var_r5_2][0];
        ov96_021EABA8(sp4->unk_5C[(s32)temp_r6 / 3][(s32)temp_r6 % 3].unk_0, var_r5_2 + 0x10);
        var_r5_2 += 1;
    } while ((u32)var_r5_2 < 9U);
}
