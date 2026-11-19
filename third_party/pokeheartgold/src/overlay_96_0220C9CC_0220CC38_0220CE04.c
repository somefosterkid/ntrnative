#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96rival.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021E8228(PokeathlonCourseData *, u32, u32, s32, s32);

s32 ov96_0220CD84(Ov96Rival *, s32);

s32 ov96_0220D014(Ov96Rival *, s32, u16);

s32 ov96_0220D33C(u32, u32);

void ov96_0220CE04(Ov96Rival *arg0, s32 arg1);

void ov96_0220CE04(Ov96Rival *arg0, s32 arg1) {
    s32 sp4;
    u32 *var_r4;
    Ov96Rival *var_r5;
    s32 temp_r0_2;
    s32 temp_r1;
    u16 var_r6;
    u32 temp_r2;
    u32 var_r7;
    u8 temp_r0_3;
    var_r5 = arg0;
    sp4 = arg1;
    var_r6 = 0;
    if (var_r5 == 0L) {
        GF_AssertFail();
    }
    var_r7 = var_r5->stat[var_r5->unk_14 >> 0x1E] << 0x17;
    var_r7 = var_r7 >> 0x17;
    if (sp4 != 0) {
        var_r7 += 0xA0;
        var_r7 = (u32)((s16)var_r7);
    }
    var_r4 = &var_r5->stat[3];
loop_5:
    *var_r4 -= var_r7;

    temp_r0_2 = var_r5->stat[3];
    if (temp_r0_2 <= 0) {
        var_r7 = (u32)((s16)(0 - temp_r0_2));
        var_r5->stat[3] = 0x64;
        ov96_021E8228(var_r5->course, (u8)(var_r5->unk_18 >> 0x1E), (u8)(var_r5->unk_14 >> 0x1E), 3, 1);
        var_r6 += 1;
        goto loop_5;
    }
    temp_r2 = var_r5->unk_14;
    var_r5->unk_14 = (u32)((temp_r2 & (~0xFF)) | ((u8)(((u8)temp_r2) + var_r6)));
    temp_r1 = var_r5->unk_1C;
    if ((((u32)(temp_r1 << 0x16)) >> 0x1F) == 0) {
        if ((((u32)(temp_r1 << 0x17)) >> 0x1F) != 0) {
            var_r5->stat[var_r5->unk_14 >> 0x1E] = (0xFC03FFFF & var_r5->stat[var_r5->unk_14 >> 0x1E]) | (((u32)(ov96_0220D33C(((u32)(var_r5->stat[var_r5->unk_14 >> 0x1E] << 6)) >> 0x18, (u32)((u8)temp_r1)) << 0x18)) >> 6);
            if ((((u32)(var_r5->stat[var_r5->unk_14 >> 0x1E] << 6)) >> 0x18) == 0) {
                ov96_021E8228(var_r5->course, (u8)(var_r5->unk_18 >> 0x1E), (u8)(var_r5->unk_14 >> 0x1E), 1, 1);
                ov96_0220CD84(var_r5, 2);
                return;
            }
        }
        ov96_0220D014(var_r5, sp4, var_r6);
        goto block_12;
    }
block_12:
    if (var_r6 != 0) {
        temp_r0_3 = (u8)(var_r5->unk_14);
        if ((temp_r0_3 != 0) && ((temp_r0_3 % 10U) == 0)) {
            ov96_0220CD84(var_r5, 3);
        }
    }

    var_r5->unk_1C = (s32)((var_r5->unk_1C | 0x100) & (~0xFF));
    ov96_021E8228(var_r5->course, (u8)(var_r5->unk_18 >> 0x1E), (u8)(var_r5->unk_14 >> 0x1E), 4, 1);
}
#else
s32 ov96_021E8228();

s32 ov96_0220CD84(s32 *, s32);

s32 ov96_0220D014(s32 *, s32, u16);

s32 ov96_0220D33C(u32, u16);

void ov96_0220CE04(Ov96Rival *arg0, s32 arg1);

void ov96_0220CE04(Ov96Rival *arg0, s32 arg1) {
    s32 spC;
    s32 sp8;
    s32 sp4;
    s32 *var_r4;
    s32 *var_r5;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r1;
    u16 var_r6;
    u32 temp_r2;
    u32 var_r7;
    u8 temp_r0_3;
    var_r5 = (s32 *)arg0;
    sp4 = arg1;
    var_r6 = 0;
    if (var_r5 == 0L) {
        GF_AssertFail();
    }
    sp8 = (s32)(var_r5 + 1);
    temp_r0 = (((u32)(*((u32 *)(((u8 *)var_r5) + 0x14)))) >> 0x1E) * 4;
    spC = temp_r0;
    var_r7 = (s32)((*((u32 *)(sp8 + temp_r0))) << 0x17);
    var_r7 = var_r7 >> 0x17;
    if (sp4 != 0) {
        var_r7 += 0xA0;
        var_r7 = (u32)((s16)var_r7);
    }
    var_r4 = var_r5;
    var_r4 += 4;
loop_5:
    *var_r4 -= var_r7;

    temp_r0_2 = *((u32 *)(((u8 *)var_r5) + 0x10));
    if (temp_r0_2 <= 0) {
        var_r7 = (u32)((s16)(0 - temp_r0_2));
        *((s32 *)(((u8 *)var_r5) + 0x10)) = 0x64;
        ov96_021E8228(*((s32 *)var_r5), (u8)(((u32)(*((u32 *)(((u8 *)var_r5) + 0x18)))) >> 0x1E), (u8)(((u32)(*((u32 *)(((u8 *)var_r5) + 0x14)))) >> 0x1E), 3, 1);
        var_r6 += 1;
        goto loop_5;
    }
    temp_r2 = *((u32 *)(((u8 *)var_r5) + 0x14));
    *((u32 *)(((u8 *)var_r5) + 0x14)) = (u32)((temp_r2 & (~0xFF)) | ((u8)(((u8)temp_r2) + var_r6)));
    temp_r1 = *((s32 *)(((u8 *)var_r5) + 0x1C));
    if ((((u32)(temp_r1 << 0x16)) >> 0x1F) == 0) {
        if ((((u32)(temp_r1 << 0x17)) >> 0x1F) != 0) {
            *((s32 *)(sp8 + spC)) = (0xFC03FFFF & (*((s32 *)(sp8 + spC)))) | (((u32)(ov96_0220D33C(((u32)((*((s32 *)(sp8 + spC))) << 6)) >> 0x18, (u32)((u8)temp_r1)) << 0x18)) >> 6);
            if ((((u32)((*((s32 *)(sp8 + spC))) << 6)) >> 0x18) == 0) {
                ov96_021E8228(*((s32 *)var_r5), (u8)(((u32)(*((u32 *)(((u8 *)var_r5) + 0x18)))) >> 0x1E), (u8)(((u32)(*((u32 *)(((u8 *)var_r5) + 0x14)))) >> 0x1E), 1, 1);
                ov96_0220CD84(var_r5, 2);
                return;
            }
        }
        ov96_0220D014(var_r5, sp4, var_r6);
        goto block_12;
    }
block_12:
    if (var_r6 != 0) {
        temp_r0_3 = (u8)(*((u32 *)(((u8 *)var_r5) + 0x14)));
        if ((temp_r0_3 != 0) && ((temp_r0_3 % 10U) == 0)) {
            ov96_0220CD84(var_r5, 3);
        }
    }

    *((s32 *)(((u8 *)var_r5) + 0x1C)) = (s32)(((*((s32 *)(((u8 *)var_r5) + 0x1C))) | 0x100) & (~0xFF));
    ov96_021E8228(*((s32 *)var_r5), (u8)(((u32)(*((u32 *)(((u8 *)var_r5) + 0x18)))) >> 0x1E), (u8)(((u32)(*((u32 *)(((u8 *)var_r5) + 0x14)))) >> 0x1E), 4, 1);
}
#endif
