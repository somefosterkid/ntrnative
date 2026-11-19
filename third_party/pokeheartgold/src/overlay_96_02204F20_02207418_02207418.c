#include "global.h"

#include "error_handling.h"

s32 ov96_021E8228(s32, u32, u32, s32, s32);

s32 ov96_021EAF78(s32, s32, s32, VecFx32 *, s32 *, s32 *);

s32 ov96_021EB0A4(s32, s32, s32, s32 *, s32 *);

s32 ov96_02207A34(s32, void *, void *);

s32 ov96_02207418(s32 arg0, void *arg1, void *arg2);

s32 ov96_02207418(s32 arg0, void *arg1, void *arg2) {
    VecFx32 sp78;
    VecFx32 sp6C;
    VecFx32 sp60;
    VecFx32 sp54;
    VecFx32 sp48;
    VecFx32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    s32 sp18;
    s32 sp14;
    void *sp10;
    void *spC;
    s32 sp8;
    VecFx32 *temp_r0_3;
    VecFx32 *temp_r0_4;
    VecFx32 *temp_r1_2;
    VecFx32 *temp_r1_3;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r1;
    s32 temp_r2;
    s32 var_r7;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 var_r4;
    void *var_r5;
    void *var_r6;
    sp8 = arg0;
    spC = arg1;
    sp10 = arg2;
    sp18 = 0;
    sp78.z = 0;
    sp6C.z = 0;
    var_r7 = 0;
    var_r6 = arg1;
loop_1:
    temp_r1 = *((u8 *)(((u8 *)var_r6) + 0xB1));

    if ((*((u8 *)(((u8 *)var_r6) + 0xAB))) == 0) {
        temp_r0 = (sp20 = *((s32 *)(var_r6 + (temp_r1 * 4))));
        temp_r2 = *((s32 *)(((u8 *)var_r6) + 0x58));
        temp_r3 = *((s32 *)(((u8 *)var_r6) + 0x5C));
        ov96_021EB0A4(temp_r0, temp_r2 / 0x1000, temp_r3 / 0x1000, &sp30, &sp2C);
        var_r5 = spC;
        var_r4 = 0;
loop_4:
        if ((var_r7 != var_r4) && ((*((u8 *)(((u8 *)var_r5) + 0xAB))) == 0)) {
            sp14 = sp30;
            temp_r0_2 = (sp1C = *((s32 *)(var_r5 + ((*((u8 *)(((u8 *)var_r5) + 0xB1))) * 4))));
            temp_r1 = *((s32 *)(((u8 *)var_r5) + 0x58));
            temp_r3_2 = *((s32 *)(((u8 *)var_r5) + 0x5C));
            ov96_021EB0A4(temp_r0_2, temp_r1 / 0x1000, temp_r3_2 / 0x1000, &sp28, &sp24);
            ov96_021EAF78(sp1C, sp28 << 0xC, sp24 << 0xC, &sp6C, &sp6C.y, &sp34);
            ov96_021EAF78(sp20, sp14 << 0xC, sp2C << 0xC, &sp78, &sp78.y, &sp38);
            VEC_Subtract(&sp78, &sp6C, &sp60);
            if (VEC_Mag(&sp60) < ((s32)((sp34 + sp38) << 0xC))) {
                if ((*((u8 *)(((u8 *)(var_r6 + var_r4)) + 0x94))) != 0) {
                    goto block_9;
                }
                if (ov96_02207A34(sp8, var_r6, var_r5) == 0) {
                    *((s8 *)(((u8 *)var_r6) + 0xA4)) = 6;
                    *((s8 *)(((u8 *)var_r5) + 0xA4)) = 6;
                }
                if (VEC_Mag(var_r6 + 0x64) > 0xB000) {
                    temp_r1_2 = (VecFx32 *)(&sp54);
                    temp_r1_2->x = 0;
                    temp_r1_2->y = 0;
                    temp_r1_2->z = 0;
                    temp_r0_3 = var_r6 + 0x64;
                    VEC_Normalize(temp_r0_3, temp_r0_3);
                    temp_r1_2 = var_r6 + 0x64;
                    VEC_MultAdd(0xB000, var_r6 + 0x64, &sp54, var_r6 + 0x64);
                }
                if (VEC_Mag(var_r5 + 0x64) > 0xB000) {
                    temp_r1_2 = (VecFx32 *)(&sp48);
                    temp_r1_2->x = 0;
                    temp_r1_2->y = 0;
                    temp_r1_2->z = 0;
                    temp_r0_4 = var_r5 + 0x64;
                    VEC_Normalize(temp_r0_4, temp_r0_4);
                    temp_r1_3 = var_r5 + 0x64;
                    VEC_MultAdd(0xB000, temp_r1_3, &sp48, var_r5 + 0x64);
                }
                if ((*((u8 *)(((u8 *)(var_r6 + var_r4)) + 0x94))) != 0) {
                    GF_AssertFail();
                }
                if ((*((u8 *)(((u8 *)(var_r5 + var_r7)) + 0x94))) != 0) {
                    GF_AssertFail();
                }
                *((s8 *)(((u8 *)(var_r6 + var_r4)) + 0x94)) = 1;
                *((s8 *)(((u8 *)(var_r5 + var_r7)) + 0x94)) = 1;
                if (sp18 == 0) {
                    temp_r1_2 = (VecFx32 *)(&sp3C);
                    temp_r1_2->x = 0;
                    temp_r1_2->y = 0;
                    temp_r1_2->z = 0;
                    sp18 = 1;
                    VEC_Subtract(&sp6C, &sp78, &sp3C);
                    VEC_Normalize(&sp3C, &sp3C);
                    VEC_MultAdd(sp38 << 0xC, &sp3C, &sp78, &sp3C);
                    *((VecFx32 *)((u8 *)sp10)) = sp3C;
                }
                ov96_021E8228(sp8, (u8)(*((s32 *)(((u8 *)var_r6) + 0x98))), *((u8 *)(((u8 *)var_r6) + 0xB1)), 4, 1);
                ov96_021E8228(sp8, (u8)(*((s32 *)(((u8 *)var_r5) + 0x98))), *((u8 *)(((u8 *)var_r5) + 0xB1)), 4, 1);
            } else {
                *((s8 *)(((u8 *)(var_r6 + var_r4)) + 0x94)) = 0;
                ((u8 *)var_r5)[var_r7 + 0x94] = 0;
                goto block_24;
            }
        } else {
block_9:
block_24:
            var_r4 += 1;

            var_r5 += 0xB8;
            if (var_r4 < 4) {
                goto loop_4;
            }
        }
    }
    var_r7 += 1;
    var_r6 += 0xB8;
    if (var_r7 < 4) {
        goto loop_1;
    }
    return sp18;
}
