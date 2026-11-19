#include "global.h"

int ov89_0225AD00(void *);

s32 ov89_0225AD64(void *, int);

s32 ov89_0225ADA4(u8 *, u8 *, void *, s32);

s32 sub_020181B0(void *, void *);

s32 sub_020182A0(void *, s32);

s32 sub_020182A8(void *, s32, s32, s32);

s32 sub_020182B0(u8 *, s32 *, s32 *, s32 *);

s32 ov89_0225B620(void *arg0, u8 *arg1);

s32 ov89_0225B620(void *arg0, u8 *arg1) {
    s32 spC;
    s32 sp8;
    s32 sp4;
    u8 *sp0;
    s32 var_r6;
    s32 var_r6_2;
    u8 temp_r2;
    void *temp_r4;
    void *var_r5;
    void *var_r5_2;

    sp0 = arg1;
    temp_r4 = (void *)(arg1 + 0x94);
    temp_r2 = *(u8 *)((u8 *)temp_r4 + 0x16D);
    switch (temp_r2) {
    case 0:
        *(u8 *)((u8 *)temp_r4 + 0x16C) = ov89_0225AD00(arg0);
        sub_020182B0(sp0 + 0x1C, &spC, &sp8, &sp4);
        var_r6 = 0;
        var_r5 = temp_r4;
        do {
            sub_020181B0(var_r5, sp0 + 0xC);
            sub_020182A8(var_r5, spC, sp8, sp4);
            sub_020182A0(var_r5, 0);
            var_r6 += 1;
            var_r5 += 0x78;
        } while (var_r6 < 3);
        *(u8 *)((u8 *)temp_r4 + 0x16D) = (u8)(*(u8 *)((u8 *)temp_r4 + 0x16D) + 1);
    case 1:
        sub_020182B0(sp0 + 0x1C, &spC, &sp8, &sp4);
        if ((spC < -0x50000) || (spC > 0x50000) || (sp8 > 0x30000) || (sp8 < -0x30000) || (*(u8 *)((u8 *)temp_r4 + 0x16B) == 1)) {
            var_r6_2 = 0;
            var_r5_2 = temp_r4;
            do {
                sub_020182A0(var_r5_2, 0);
                var_r6_2 += 1;
                var_r5_2 += 0x78;
            } while (var_r6_2 < 3);
            sub_020182A0((void *)(sp0 + 0x1C), 0);
            *(u8 *)((u8 *)temp_r4 + 0x16D) = (u8)(*(u8 *)((u8 *)temp_r4 + 0x16D) + 1);
        } else {
            sub_020182A8((void *)(sp0 + 0x1C), spC, sp8 + (0x50000U >> 5), sp4);
            if (((s32) * (u16 *)((u8 *)temp_r4 + 0x168) % 3) == 0) {
                sub_020182A8(temp_r4 + (0x78 * *(u8 *)((u8 *)temp_r4 + 0x16A)), spC, sp8 + 0x2800, sp4);
                sub_020182A0(temp_r4 + (0x78 * *(u8 *)((u8 *)temp_r4 + 0x16A)), 1);
                *(u8 *)((u8 *)temp_r4 + 0x16A) = (u8)(*(u8 *)((u8 *)temp_r4 + 0x16A) + 1);
                if ((u32) * (u8 *)((u8 *)temp_r4 + 0x16A) >= 3U) {
                    *(u8 *)((u8 *)temp_r4 + 0x16A) = 0U;
                }
            }
            *(u16 *)((u8 *)temp_r4 + 0x168) = (u16)(*(u16 *)((u8 *)temp_r4 + 0x168) + 1);
        }
        break;
    default:
        ov89_0225AD64(arg0, *(u8 *)((u8 *)temp_r4 + 0x16C));
        return 1;
    }
    ov89_0225ADA4(sp0 + 0x1C, sp0, temp_r4 + 0x170, 0);
    return 0;
}
