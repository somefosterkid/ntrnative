#include "global.h"

#include "constants/heap.h"

#include "struct_defs/personal_narc_structs.h"

#include "gf_rtc.h"
#include "math_util.h"
#include "pokemon.h"

extern s32 ov112_021F4138;

void ov112_021E8B74(void *arg0, u16 *arg1, s32 arg2, s32 arg3, s32 arg4);

void ov112_021E8B74(void *arg0, u16 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp14;
    int new_var;
    s32 sp10;
    BASE_STATS *spC;
    s32 sp8;
    u32 sp4;
    u16 *sp0;
    s32 temp_r1;
    s32 var_r1;
    s32 var_r7;
    u16 temp_r0_3;
    s32 temp_r0_2;
    u16 temp_r0;
    void *var_r2;
    void *var_r3;
    void *var_r4;
    void *var_r5;
    void *var_r6;
    sp0 = arg1;
    sp4 = arg2;
    sp10 = 0xC0 * arg3;
    var_r5 = arg0;
    SetLCRNGSeed(GF_RTC_TimeToSec());
    var_r7 = 0;
    sp8 = 0;
    do {
        temp_r0 = LCRandom();
        temp_r0_2 = sp8 + (temp_r0 % 2);
        var_r4 = &ov112_021F4138;
        var_r6 = var_r4 + sp10;
        temp_r1 = 0x14 * temp_r0_2;
        var_r6 += 8;
        sp14 = temp_r1;
        var_r4 = var_r6 + temp_r1;
        *((u8 *)(arg4 + var_r7)) = temp_r0_2;
        spC = AllocAndLoadMonPersonal_HandleAlternateForm((s32)(*((u16 *)(var_r6 + sp14))), (s32)(*((u8 *)(((u8 *)var_r4) + 6))), HEAP_ID_154);
        var_r1 = 0;
        var_r2 = var_r4;
        var_r3 = var_r5;
        *((u16 *)var_r5) = (u16)(*((u16 *)(var_r6 + sp14)));
        *((u16 *)(((u8 *)var_r5) + 2)) = (u16)(*((u16 *)(((u8 *)var_r4) + 4)));
loop_2:
        temp_r0_3 = *((u16 *)(((u8 *)var_r2) + 8));

        var_r1 = var_r1 + 1;
        var_r2 += 2;
        *((u16 *)(((u8 *)var_r3) + 4)) = temp_r0_3;
        var_r3 += 2;
        if (var_r1 < 4) {
            goto loop_2;
        }
        *((u8 *)(((u8 *)var_r5) + 0xC)) = (u8)(*((u16 *)(((u8 *)var_r4) + 2)));
        *((u8 *)(((u8 *)var_r5) + 0xD)) = (u8)(((*((u8 *)(((u8 *)var_r5) + 0xD))) & (~0x1F)) | (0x1F & (*((u8 *)(((u8 *)var_r4) + 6)))));
        *((u8 *)(((u8 *)var_r5) + 0xD)) = (u8)(((*((u8 *)(((u8 *)var_r5) + 0xD))) & (~0x60)) | (((u32)((*((u8 *)(((u8 *)var_r4) + 7))) << 0x1E)) >> 0x19));
        *((u8 *)(((u8 *)var_r5) + 0xD)) = (u8)((*((u8 *)(((u8 *)var_r5) + 0xD))) & (~0x80));
        *((u8 *)(((u8 *)var_r5) + 0xE)) = (u8)((1 & ((u8)GetPersonalAttr(spC, 0x1C))) | ((*((u8 *)(((u8 *)var_r5) + 0xE))) & (~1)));
        *sp0 = *((u16 *)(((u8 *)var_r4) + 0x10));
        *((u8 *)(sp4 + var_r7)) = (u8)(*((u16 *)(((u8 *)var_r4) + 0x12)));
        FreeMonPersonal(spC);
        new_var = 1;
        sp8 += 2;
        var_r5 += 0x10;
        sp0 += new_var;
        var_r7 += new_var;
    } while (var_r7 < 3);
}
