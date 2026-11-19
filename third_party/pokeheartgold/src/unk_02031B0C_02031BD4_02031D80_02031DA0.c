#include "global.h"

s32 sub_02032588(void *, u8 *, u16);

void sub_02031DA0(u8 *arg0, void *arg1);

void sub_02031DA0(u8 *arg0, void *arg1) {
    u8 sp4[12];
    u8 sp0;
    void *var_r3;
    s32 var_r4;
    s32 var_r5;
    s32 var_r7;
    s32 var_r0;
    u8 temp_r0;
    u8 temp_r2;
    u8 temp_r3;
    u8 temp_r4;
    void *temp_ip;
    void *var_r0_2;
    void *var_r1;
    void *var_r1_2;

    MI_CpuFill8(arg1, 0U, 0xEU);
    MI_CpuFill8(&sp4[2], 0U, 0xAU);
    var_r0 = 0;
    sp0 = 0;
    var_r1 = &sp4[2];
    do {
        temp_r2 = *(u8 *)((u8 *)(arg0 + var_r0) + 3);
        *(s8 *)var_r1 = var_r0;
        *(u8 *)((u8 *)var_r1 + 1) = temp_r2;
        if (temp_r2 != 0) {
            sp0++;
        }
        var_r0 += 1;
        var_r1 += 2;
    } while ((s32)var_r0 < 5);
    if (sp0 != 0) {
        var_r0_2 = &sp4[2];
        var_r7 = 0;
        temp_ip = var_r0_2;
        do {
            var_r5 = var_r7 + 1;
            if (var_r5 < 5) {
                var_r1_2 = temp_ip + (var_r5 * 2);
                do {
                    temp_r4 = *(u8 *)((u8 *)var_r1_2 + 1);
                    temp_r3 = *(u8 *)((u8 *)var_r0_2 + 1);
                    if (((u32)temp_r3 <= (u32)temp_r4) && ((temp_r3 != temp_r4) || ((u32) * (u8 *)var_r0_2 >= (u32) * (u8 *)var_r1_2))) {
                        *(u8 *)(sp4) = (u8) * (u8 *)var_r0_2;
                        *(u8 *)((u8 *)(sp4) + 1) = (u8) * (u8 *)((u8 *)var_r0_2 + 1);
                        *(u8 *)var_r0_2 = (u8) * (u8 *)var_r1_2;
                        *(u8 *)((u8 *)var_r0_2 + 1) = (u8) * (u8 *)((u8 *)var_r1_2 + 1);
                        *(u8 *)var_r1_2 = (u8) * (u8 *)(sp4);
                        *(u8 *)((u8 *)var_r1_2 + 1) = (u8) * (u8 *)((u8 *)(sp4) + 1);
                    }
                    var_r1_2 += 2;
                    var_r5 += 1;
                } while (var_r5 < 5);
            }
            var_r7 += 1;
            var_r0_2 += 2;
        } while (var_r7 < 5);
    }
    var_r4 = 0;
    var_r3 = &sp4[2];
    var_r5 = sp4[11];
    do {
        *(u8 *)((u8 *)(arg1 + var_r4) + 6) = (u8) * (u8 *)var_r3;
        if (var_r5 == *(u8 *)((u8 *)var_r3 + 1)) {
            ((u8 *)arg1)[0xB]++;
        }
        var_r4 += 1;
        var_r3 += 2;
    } while (var_r4 < 5);
    ((u8 *)arg1)[0xC] = sp0;
    switch (sp0) {
    case 0:
        *(u16 *)arg1 = (6 | (*(u16 *)arg1 & ~0xF));
        return;
    case 1:
        sub_02032588(arg1, &sp4[2], 0);
        *(u16 *)((u8 *)arg1 + 2) = (6 | (*(u16 *)((u8 *)arg1 + 2) & ~0xF));
        break;
    case 2:
        sub_02032588(arg1, &sp4[2], 0);
        sub_02032588(arg1 + 2, &sp4[4], 1);
        *(u16 *)((u8 *)arg1 + 4) = (6 | (*(u16 *)((u8 *)arg1 + 4) & ~0xF));
        break;
    case 3:
        sub_02032588(arg1, &sp4[2], 0);
        sub_02032588(arg1 + 2, &sp4[4], 1);
        sub_02032588(arg1 + 4, &sp4[6], 2);
        break;
    case 4:
    case 5:
        if ((s32)(*(u8 *)((u8 *)(sp4) + 3) - *(u8 *)((u8 *)(sp4) + 9)) <= 0xC) {
            *(u16 *)arg1 = (5 | (*(u16 *)arg1 & ~0xF));
            *(u16 *)arg1 = ((*(u16 *)arg1 & 0xFFFF00FF) | ((u32)(sp4[3] << 24) >> 16));
            *(u16 *)((u8 *)arg1 + 2) = ((*(u16 *)((u8 *)arg1 + 2) & ~0xF) | 6);
            *(u16 *)((u8 *)arg1 + 2) = ((*(u16 *)((u8 *)arg1 + 2) & 0xFFFF00FF) | ((u32)(sp4[5] << 24) >> 16));
            *(u16 *)((u8 *)arg1 + 4) = ((*(u16 *)((u8 *)arg1 + 4) & ~0xF) | 6);
            *(u16 *)((u8 *)arg1 + 4) = ((*(u16 *)((u8 *)arg1 + 4) & 0xFFFF00FF) | ((u32)(sp4[7] << 24) >> 16));
            *(s8 *)((u8 *)arg1 + 0xC) = 1;
            temp_r0 = *(u8 *)((u8 *)(sp4) + 3);
            if ((u32)temp_r0 > 0x14U) {
                *(u16 *)arg1 = (0x30 | (*(u16 *)arg1 & ~0xF0));
            } else {
                if ((u32)temp_r0 > 7U) {
                    *(u16 *)arg1 = (0x20 | ((*(u16 *)arg1) & ~0xF0));
                } else {
                    *(u16 *)arg1 = (0x10 | ((*(u16 *)arg1) & ~0xF0));
                }
            }
        } else {
            sub_02032588(arg1, &sp4[2], 0);
            sub_02032588(arg1 + 2, &sp4[4], 1);
            if (sp0 == 4) {
                sub_02032588(arg1 + 4, &sp4[10], 2);
            } else {
                *(u16 *)((u8 *)arg1 + 4) = (5 | (*(u16 *)((u8 *)arg1 + 4) & ~0xF));
                *(u16 *)((u8 *)arg1 + 4) = (0x10 | (*(u16 *)((u8 *)arg1 + 4) & ~0xF0));
                *(u16 *)((u8 *)arg1 + 4) = ((*(u16 *)((u8 *)arg1 + 4) & 0xFFFF00FF) | ((u32)(sp4[7] << 24) >> 16));
            }
        }
        break;
    }
    ((u8 *)arg1)[0xC] = sp0;
    return;
}
