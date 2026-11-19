#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "player_data.h"

extern s32 ov96_021E7C04(s32, u16 (*)[], s32 *);

s32 ov96_021E7CC8(PokeathlonCourseData *, void *, s32 *);

extern u8 _0221A7D8[];

void ov96_021E7A2C(PokeathlonCourseData *arg0, void *arg1);

void ov96_021E7A2C(PokeathlonCourseData *arg0, void *arg1) {
    u16 sp20[16];
    s32 sp40[40];
    u8 var_r7;
    u8 *sp1C;
    void *sp14;
    s32 sp18;
    void *temp_r1;
    s32 sp10;
    void *sp4;
    PokeathlonCourseData *sp0;
    PlayerProfile *temp_r0_5;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r5;
    u16 *temp_r0_6;
    u16 *temp_r2_3;
    u16 temp_r3_5;
    u32 temp_r2;
    u8 temp_r0;
    u8 temp_r0_7;
    u8 var_r1;
    u8 var_r1_2;
    u8 var_r5;
    u8 var_r5_2;
    void *spC;
    void *temp_r0_2;
    void *temp_r0_4;
    s32 sp8;
    void *temp_r6;

    sp0 = arg0;
    sp4 = arg1;
    sp10 = (s32)(u8)ov96_021E5F24(arg0);
    sp8 = 0;
loop_1:
    temp_r0 = (u8) * (s32 *)((u8 *)(&sp0->field_0[sp8]) + 0x3D8);
    temp_r1 = (void *)(sp4 + (temp_r0 * 0xA4));
    sp14 = temp_r1;
    temp_r2 = *(u32 *)((u8 *)temp_r1 + 0x28);
    if (temp_r2 < 0x98967FU) {
        *(s32 *)((u8 *)sp14 + 0x28) = (s32)(temp_r2 + 1);
    }
    sp18 = (s32)_0221A7D8[temp_r0];
    var_r1 = 0;
    do {
        temp_r3 = var_r1 * 8;
        temp_r5 = var_r1 << 5;
        temp_r0_2 = (u8 *)sp40 + temp_r5;
        *(u16 *)((u8 *)sp40 + temp_r5) = *(u16 *)(sp14 + temp_r3);
        var_r5 = 0;
loop_5:
        temp_r3_2 = var_r5 * 2;
        *(u16 *)((u8 *)(temp_r0_2 + temp_r3_2) + 2) = (u16) * (u16 *)((u8 *)(sp14 + temp_r3 + temp_r3_2) + 2);
        var_r5 += 1;
        if ((u32)var_r5 < 3U) {
            goto loop_5;
        }
        temp_r6 = sp14 + (var_r1 * 0x18);
        var_r5_2 = 0;
        *(s32 *)((u8 *)temp_r0_2 + 8) = (s32) * (s32 *)((u8 *)temp_r6 + 0x2C);
loop_7:
        temp_r3_3 = var_r5_2 * 2;
        *(u16 *)((u8 *)(temp_r0_2 + temp_r3_3) + 0xC) = (u16) * (u16 *)((u8 *)(temp_r6 + temp_r3_3) + 0x30);
        var_r5_2 += 1;
        if ((u32)var_r5_2 < 8U) {
            goto loop_7;
        }
        *(u8 *)((u8 *)temp_r0_2 + 0x1C) = (u8) * (u8 *)((u8 *)temp_r6 + 0x40);
        var_r1 += 1;
    } while ((u32)var_r1 < 5U);
    spC = ((u8 *)sp0) + (sp8 * 2);
    var_r7 = 0;
    do {
        temp_r0 = ((sp10 + var_r7) % 4);
        *(u16 *)(&sp20[0]) = *(u16 *)((u8 *)(spC + (temp_r0 * 8)) + 0x8D4);
        sp1C = &sp0->participants[0].field_04;
        temp_r0_4 = sp1C + (temp_r0 * 0x7C);
        for (var_r5 = 0; var_r5 < 3; var_r5++) {
            (&sp20[1])[var_r5] = (*(u16 *)(temp_r0_4 + var_r5 * 0x28 + 2) << 10) + *(u16 *)(temp_r0_4 + var_r5 * 0x28);
        }
        temp_r0_5 = PokeathlonCourse_GetPlayerProfileFromData(sp0, (s32)temp_r0);
        *(u32 *)&sp20[4] = PlayerProfile_GetTrainerID(temp_r0_5);
        temp_r0_6 = (unsigned short *)(PlayerProfile_GetNamePtr(temp_r0_5));
        var_r1_2 = 0;
loop_13:
        temp_r3_5 = temp_r0_6[var_r1_2];
        temp_r2_3 = &(&sp20[0])[var_r1_2];
        *(u16 *)((u8 *)temp_r2_3 + 0xC) = temp_r3_5;
        var_r1_2 += 1;
        if ((u32)var_r1_2 < 8U) {
            goto loop_13;
        }
        *(u8 *)((u8 *)(&sp20[0]) + 0x1C) = PlayerProfile_GetLanguage(temp_r0_5);
        ov96_021E7C04(sp18, (u16(*)[]) & sp20[0], sp40);
        var_r7 += 1;
    } while ((u32)var_r7 < 4U);
    ov96_021E7CC8(sp0, sp14, sp40);
    temp_r0_7 = sp8 + 1;
    sp8 = (s32)temp_r0_7;
    if ((u32)temp_r0_7 < 4U) {
        goto loop_1;
    }
}
