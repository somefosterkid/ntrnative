
#include "error_handling.h"
#include "math_util.h"
s32 ov96_021EAF78(s32, s32, s32, s32 *, s32 *, s32 *);
s32 ov96_021EB06C(s32, s32, s32, s32 *, s32 *);
s32 ov96_021EB0A4(s32, s32, s32, s32 *, s32 *);
s32 ov96_021F333C(s32, s32 *);
s32 ov96_021F380C(s32, void **);
s32 ov96_021F3888(void *, s32, s32 *, void *, void **);
s32 ov96_021F38FC(s32, s32);
s32 ov96_021F3B04(u16, u16, void *, s32 *);
s32 ov96_021F3B38(s32, void **, void **);

void ov96_021F350C(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_r0_5;
    void *sp134[25];
    void *spD0[25];
    s32 spB0[8];
    void *sp8C[9];
    void *sp4C[16];
    s32 temp_r0_4;
    void *temp_r0;
    s32 sp48;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    void *sp20;
    s32 sp1C;
    s32 sp18;
    s32 sp28;
    u8 sp24[4];
    s32 spC;
    void *sp8;
    s32 *var_r4_2;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 sp14;
    s32 sp10;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r4;
    s32 temp_r4_2;
    s32 var_r0;
    s32 var_r4;
    s32 var_r6;
    void **var_r7;
    void *temp_r2;
    void *temp_r2_2;
    void *temp_r2_3;
    void *temp_r2_4;
    void *temp_r2_5;
    void *var_r4_3;
    s32 var_r6_2;
    void *var_r5;
    void *var_r5_2;
    void *var_r5_3;

    sp8 = arg0;
    temp_r0 = arg0 + (arg1 << 5);
    spC = arg2;
    sp20 = temp_r0;
    if (**(s32 **)((u8 *)temp_r0 + 0x10) == 0) {
        return;
    }
    if (*(s32 **)((u8 *)sp20 + 0x18) == NULL) {
        GF_AssertFail();
        return;
    }
    if (((s32) *(s32 *)((u8 *)((sp8 + (*(u8 *)((u8 *)sp8 + 0x4E8) * 4))) + 0x4B4) < 3) && ((s32) ((s32) LCRandom() % 100) < 0x1E)) {
        temp_r2 = sp8 + 0x4B4;
        temp_r1 = *(u8 *)((u8 *)sp8 + 0x4E8) * 4;
        *(s32 *)(temp_r2 + temp_r1) = *(s32 *)(temp_r2 + temp_r1) + 1;
        return;
    }
    var_r5 = sp8;
    var_r4 = 0;
    do {
        if (**(s32 **)((u8 *)var_r5 + 0x10) != 0) {
            temp_r2_2 = *(void **)((u8 *)var_r5 + 0xC);
            ov96_021EAF78(*(s32 *)((u8 *)var_r5 + 0x1C), *(s32 *)temp_r2_2, *(s32 *)((u8 *)temp_r2_2 + 4), &sp40, &sp3C, &sp38);
            var_r0 = ov96_021F38FC((sp40 / 0x1000), (sp3C / 0x1000));
        } else {
            var_r0 = -1;
        }
        var_r4 += 1;
        *(s32 *)var_r5 = var_r0;
        var_r5 += 0x20;
    } while (var_r4 < 0xC);
    temp_r2_3 = *(void **)((u8 *)sp20 + 0xC);
    temp_r3 = *(s32 *)temp_r2_3;
    temp_r3_2 = *(s32 *)((u8 *)temp_r2_3 + 4);
    ov96_021EB06C(*(s32 *)((u8 *)sp20 + 0x1C), (temp_r3 / 0x1000), (temp_r3_2 / 0x1000), &sp48, &sp44);
    *(s32 *)sp20 = ov96_021F38FC(sp48, sp44);
    ov96_021F333C(spC, spB0);
    var_r6 = 0;
    var_r5_2 = sp8;
    var_r4_2 = spB0;
    do {
        if ((s32) *(s32 *)var_r4_2 > 0) {
            *(s32 *)((u8 *)var_r5_2 + 0x4A0) = ov96_021F38FC((s32) *(u16 *)((u8 *)var_r4_2 + 4), (s32) *(u16 *)((u8 *)var_r4_2 + 6));
        } else {
            *(s32 *)((u8 *)var_r5_2 + 0x4A0) = -1;
        }
        var_r6 += 1;
        var_r4_2 = (void *)((u8 *)var_r4_2 + (8));
        var_r5_2 += 4;
    } while (var_r6 < 4);
    if (ov96_021F3B04((u16) sp48, (u16) sp44, &sp24[2], (s32 *)sp24) == 0) {
        GF_AssertFail();
    }
    var_r4_3 = sp8;
    sp1C = 0;
    sp18 = 0;
    var_r6_2 = 0;
    var_r5_3 = var_r4_3 + 0x180;
    var_r7 = sp134;
    sp14 = (s32) *(s16 *)((u8 *)((s32 *)sp24) + 2);
    sp10 = (s32) *(s16 *)((u8 *)sp24 + sp1C);
    do {
        *(s32 *)((u8 *)var_r4_3 + 0x19C) = var_r6_2;
        *(s32 *)((u8 *)var_r4_3 + 0x180) = (s32) (((var_r6_2 % 5) - 2) + sp14);
        *(s32 *)((u8 *)var_r4_3 + 0x184) = (s32) (((var_r6_2 / 5) - 2) + sp10);
        temp_r1_2 = *(s32 *)((u8 *)var_r4_3 + 0x180);
        if ((temp_r1_2 < 0) || (temp_r0_2 = *(s32 *)((u8 *)var_r4_3 + 0x184), (temp_r0_2 < 0)) || (temp_r1_2 >= 7) || (temp_r0_2 >= 5)) {
            *(s32 *)((u8 *)var_r4_3 + 0x190) = 0;
        } else {
            *(s32 *)((u8 *)var_r4_3 + 0x190) = 1;
        }
        temp_r1_3 = *(s32 *)((u8 *)var_r4_3 + 0x180) - sp14;
        temp_r0_3 = *(s32 *)((u8 *)var_r4_3 + 0x184) - sp10;
        *var_r7 = var_r5_3;
        if ((temp_r1_3 >= -1) && (temp_r1_3 <= 1) && (temp_r0_3 >= -1) && (temp_r0_3 <= 1)) {
            temp_r0_4 = sp1C;
            sp1C += 1;
            (&sp8C[0])[temp_r0_4] = var_r5_3;
        } else {
            temp_r0_5 = sp18;
            sp18 += 1;
            (&sp4C[0])[temp_r0_5] = var_r5_3;
        }
        var_r6_2 += 1;
        var_r4_3 += 0x20;
        var_r5_3 += 0x20;
        var_r7 += 1;
    } while (var_r6_2 < 0x19);
    if (sp1C != 9) {
        GF_AssertFail();
    }
    if (sp18 != 0x10) {
        GF_AssertFail();
    }
    ov96_021F380C(9, &sp8C[0]);
    *(s32 *)((u8 *)sp8C[4] + 8) = sp48;
    *(s32 *)((u8 *)sp8C[4] + 0xC) = sp44;
    ov96_021F3888(sp8, 9, spB0, sp20, &sp8C[0]);
    temp_r4 = ov96_021F3B38(9, &sp8C[0], spD0);
    if ((s32) *(s32 *)((u8 *)spD0[0] + 0x18) >= 3) {
        temp_r2_4 = (void *)(*(void **)((u8 *)spD0 + ((u32) (((s32) LCRandom() % temp_r4) << 0x18) >> 0x16)));
        ov96_021EB0A4(*(s32 *)((u8 *)sp20 + 0x1C), *(s32 *)((u8 *)temp_r2_4 + 8), *(s32 *)((u8 *)temp_r2_4 + 0xC), &sp34, &sp30);
        **(s32 **)((u8 *)sp20 + 0x18) = sp34 << 0xC;
        *(s32 *)((u8 *)(*(s32 **)((u8 *)sp20 + 0x18)) + 4) = (s32) (sp30 << 0xC);
        *(s32 *)((u8 *)(*(s32 **)((u8 *)sp20 + 0x18)) + 8) = 0;
        return;
    }
    ov96_021F380C(0x10, &sp4C[0]);
    ov96_021F3888(sp8, 0x19, spB0, sp20, sp134);
    temp_r4_2 = ov96_021F3B38(0x19, sp134, spD0);
    temp_r2_5 = (void *)(*(void **)((u8 *)spD0 + ((u32) (((s32) LCRandom() % temp_r4_2) << 0x18) >> 0x16)));
    ov96_021EB0A4(*(s32 *)((u8 *)sp20 + 0x1C), *(s32 *)((u8 *)temp_r2_5 + 8), *(s32 *)((u8 *)temp_r2_5 + 0xC), &sp2C, &sp28);
    **(s32 **)((u8 *)sp20 + 0x18) = sp2C << 0xC;
    *(s32 *)((u8 *)(*(s32 **)((u8 *)sp20 + 0x18)) + 4) = (s32) (sp28 << 0xC);
    *(s32 *)((u8 *)(*(s32 **)((u8 *)sp20 + 0x18)) + 8) = 0;
}
