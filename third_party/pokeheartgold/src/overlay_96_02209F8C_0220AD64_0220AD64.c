#include "global.h"

#include "unk_02005D10.h"
#include "overlay_96_app_0220a87c.h"
#include "unk_struct_ov96_0220b148.h"

s32 ov96_0220AED4(UnkStruct_ov96_0220B148 *);
s32 ov96_0220AF64(UnkStruct_ov96_0220B148 *);
s32 ov96_0220B164(UnkStruct_ov96_0220B148 *);

void ov96_0220AD64(Ov96App_0220A87C *arg0);

void ov96_0220AD64(Ov96App_0220A87C *arg0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    s32 temp_r3;
    u32 *q;
    s32 var_r4_2;
    s32 var_r6;
    u16 temp_r2;
    UnkStruct_ov96_0220B148 **var_r4;
    Ov96App_0220A87C *var_r5;

    var_r5 = arg0;
    if ((u32)((u32)(var_r5->unk_180 << 0x18) >> 0x1C) >= 0xAU) {
        var_r6 = 0;
        var_r4 = &var_r5->unk_154[0];
loop_2:
        if (ov96_0220B164(*var_r4) != 1) {
            var_r4 += 1;
            var_r6 += 1;
            if (var_r6 < 0xA) {
                goto loop_2;
            }
        }
        if (var_r6 == 0xA) {
            PlaySE(0x8BDU);
            var_r5->unk_180 = (s32)(2 | (var_r5->unk_180 & ~0xF));
        }
    } else {
        temp_r2 = (u16)var_r5->unk_17C;
        q = &var_r5->unk_17C;
        *q = (*q & 0xFFFF0000) | (u16)(temp_r2 + 1);
        if ((u32)temp_r2 >= 2U) {
            *q = *q & 0xFFFF0000;
            ov96_0220AED4(var_r5->unk_154[(u32)(var_r5->unk_180 << 0x18) >> 0x1C]);
            PlaySE(0x8C1U);
            temp_r3 = var_r5->unk_180;
            var_r5->unk_180 = (s32)((temp_r3 & ~0xF0) | (u8)((((u32)(temp_r3 << 0x18) >> 0x1C) + 1) * 0x10));
        }
    }
    var_r4_2 = 0;
    do {
        ov96_0220AF64(var_r5->unk_154[var_r4_2]);
        var_r4_2 += 1;
    } while (var_r4_2 < 0xA);
#else
    void *work = arg0;
    s32 temp_r3;
    u32 *q;
    s32 var_r4_2;
    s32 var_r6;
    u16 temp_r2;
    s32 *var_r4;
    void *var_r5;

    var_r5 = work;
    if ((u32)((u32)(*(s32 *)((u8 *)var_r5 + 0x180) << 0x18) >> 0x1C) >= 0xAU) {
        var_r6 = 0;
        var_r4 = (s32 *)var_r5;
loop_2:
        if (ov96_0220B164(*(void **)((u8 *)var_r4 + offsetof(Ov96App_0220A87C, unk_154))) != 1) {
            var_r4 += 1;
            var_r6 += 1;
            if (var_r6 < 0xA) {
                goto loop_2;
            }
        }
        if (var_r6 == 0xA) {
            PlaySE(0x8BDU);
            *(s32 *)((u8 *)var_r5 + 0x180) = (s32)(2 | (*(s32 *)((u8 *)var_r5 + 0x180) & ~0xF));
        }
    } else {
        temp_r2 = (u16) * (u32 *)((u8 *)var_r5 + 0x17C);
        q = (u32 *)((u8 *)var_r5 + 0x17C);
        *q = (*q & 0xFFFF0000) | (u16)(temp_r2 + 1);
        if ((u32)temp_r2 >= 2U) {
            *q = *q & 0xFFFF0000;
            ov96_0220AED4(*(void **)((u8 *)(var_r5 + (((u32)(*(s32 *)((u8 *)var_r5 + 0x180) << 0x18) >> 0x1C) * 4)) + 0x154));
            PlaySE(0x8C1U);
            temp_r3 = *(s32 *)((u8 *)var_r5 + 0x180);
            *(s32 *)((u8 *)var_r5 + 0x180) = (s32)((temp_r3 & ~0xF0) | (u8)((((u32)(temp_r3 << 0x18) >> 0x1C) + 1) * 0x10));
        }
    }
    var_r4_2 = 0;
    do {
        ov96_0220AF64(*(void **)((u8 *)var_r5 + 0x154));
        var_r4_2 += 1;
        var_r5 += 4;
    } while (var_r4_2 < 0xA);
#endif
}
