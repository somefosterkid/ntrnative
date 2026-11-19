#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

s32 ov96_021FB514(void *, s32 *);

s32 ov96_021FB56C(void *, s32 *);

s32 ov96_021FB5C8(u8 *, u32, u32);

void ov96_021FB400(PokeathlonCourseData *arg0);

void ov96_021FB400(PokeathlonCourseData *arg0) {
    s32 sp18[65];
    u8 *sp14;
    s32 sp10;
    s32 spC;
    u8 *sp8;
    s32 sp4;
    u8 *sp0;
    u32 temp_r7;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 var_r0;
    s32 var_r1;
    s32 var_r6;
    u8 *temp_r0;
    s32 var_r6_2;
    u8 var_r7;
    u8 var_r4;
    s32 *var_r5_2;
    void *temp_r4;
    s32 *head;

    temp_r0 = PokeathlonCourse_GetDataCopyArea(arg0);
    sp8 = temp_r0;
    sp14 = ov96_021E8A20(temp_r0 + 0x28);
    sp10 = 0;
    head = sp18;
    sp18[0] = 0;
    sp18[1] = (s32)&sp18[5];
    *(s16 *)((u8 *)(sp18) + 8) = 0;
    *(s16 *)((u8 *)(sp18) + 0xA) = 0;
    *(s16 *)((u8 *)(sp18) + 0xC) = 0;
    *(s16 *)((u8 *)(sp18) + 0xE) = 0;
    sp18[4] = 0;
    sp8 = temp_r0 + 0x50;
    sp4 = 0;
    do {
        s32 *var_r5;
        sp0 = ov96_021E8A20(sp8);
        spC = 0;
loop_2:
        temp_r7 = spC + sp4;
        temp_r4 = (u8 *)sp18 + (temp_r7 + 1) * 0x14;
        *(u16 *)((u8 *)temp_r4 + 8) = (u16) * (u16 *)sp0;
        *(u16 *)((u8 *)temp_r4 + 0xC) = (u16) * (u16 *)((u8 *)sp0 + 0xA);
        *(s16 *)((u8 *)temp_r4 + 0xA) = temp_r7;
        *(u16 *)((u8 *)temp_r4 + 0xE) = (u16) * (u16 *)((u8 *)sp0 + 0x10);
        *(s32 *)((u8 *)temp_r4 + 0x10) = 0;
        var_r5 = sp18;
        var_r6 = 0;
        *(s32 *)((u8 *)temp_r4 + 4) = 0;
        if ((s32)temp_r7 > 0) {
loop_3:
            var_r5 = *(s32 **)((u8 *)var_r5 + 4);
            if (*(u16 *)((u8 *)temp_r4 + 0xE) != 0) {
                var_r0 = ov96_021FB514(temp_r4, var_r5);
            } else {
                var_r0 = ov96_021FB56C(temp_r4, var_r5);
            }
            if (var_r0 == 0) {
                var_r6 += 1;
                if (var_r6 < (s32)temp_r7) {
                    goto loop_3;
                }
            }
        }
        if (var_r6 == temp_r7) {
            *(void **)((u8 *)var_r5 + 4) = temp_r4;
            *(s32 **)temp_r4 = var_r5;
        }
        sp0 += 2;
        temp_r0_3 = spC + 1;
        spC = temp_r0_3;
        if (temp_r0_3 < 3) {
            goto loop_2;
        }
        sp8 += 0x28;
        sp4 += 3;
        temp_r0_2 = sp10 + 1;
        sp10 = temp_r0_2;
    } while (temp_r0_2 < 4);
    var_r7 = 0;
    var_r5_2 = sp18;
    var_r4 = 1;
    for (var_r1 = 0; var_r1 < 6; var_r1++) {
        sp14[var_r1 + 0x1C] = 0;
    }
    var_r6_2 = 0;
    do {
        var_r5_2 = *(s32 **)((u8 *)var_r5_2 + 4);
        if (var_r5_2 == NULL) {
            GF_AssertFail();
        }
        if (*(s32 *)((u8 *)var_r5_2 + 0x10) == 0) {
            var_r7 += var_r4;
            var_r4 = 1;
        } else {
            var_r4 += 1;
        }
        ov96_021FB5C8(sp14, (u8) * (u16 *)((u8 *)var_r5_2 + 0xA), var_r7);
        var_r6_2 += 1;
    } while (var_r6_2 < 0xC);
}
