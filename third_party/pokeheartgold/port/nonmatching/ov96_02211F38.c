/* PORTOK hk42 */
#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"
#include "unk_02005D10.h"
#include "pokeathlon/pokeathlon.h"
#include "unk_02005D10.h"
void ov96_02214418(void *arg0);
void ov96_02214490(void *arg0);
void *ov96_021E60C0(PokeathlonCourseData *, s32, s32);
s32 ov96_021E8BAC(s32);
s32 ov96_021EAA04(s32, u8);
s32 ov96_021EAA20(s32);
s32 ov96_021EAC0C(s32, u8);
s32 ov96_021EAC5C(s32, s32);
s32 ov96_021EB01C(s32, s32, s32, s32);
s32 ov96_021EB06C(s32, u8, u8, s32 *, s32 *);
s32 ov96_021EB52C(s32, s32, s32);
s32 ov96_021EB564(s32, s32);
s32 ov96_021EB570(s32, s32);
s32 ov96_021EB57C(s32);
s32 ov96_021EB588(s32, s32 *);
s32 ov96_021EB630(s32, s32);
s32 ov96_0221236C(PokeathlonCourseData *);
s32 ov96_022123B0(void *, u8, s32, u8, s32, s32);
s32 ov96_02213F5C(u8);
s32 ov96_02213FF4(void *, u8, s32, u8);
s32 ov96_022144C0(void *, void *);
s32 ov96_0221457C(void *, u8 *, s32);
u8 ov96_02214904(s32);
s32 ov96_0221490C(s32, s32);
s32 ov96_0221497C(s32, u8);

void ov96_02211F38(PokeathlonCourseData *arg0) {
    void *temp_r0_10;
    s32 sp4C;
    s32 sp44;
    s32 sp48;
    void *sp40;
    void *sp38;
    s32 sp3C;
    void *temp_r0;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp18;
    void *sp14;
    u32 sp10;
    u8 *spC;
    PokeathlonCourseData *sp8;
    s32 temp_r0_11;
    s32 temp_r0_9;
    s32 temp_r2_2;
    s32 var_r2;
    s32 var_r4;
    void *var_r4_3;
    s32 var_r5_2;
    u16 temp_r2_3;
    s32 var_r7;
    s32 temp_r2;
    u8 *temp_r0_5;
    u8 *temp_r3;
    u8 *temp_r6;
    u8 *temp_r7_2;
    u8 temp_r0_3;
    u8 temp_r0_6;
    u8 temp_r0_7;
    u8 temp_r0_8;
    u8 temp_r7;
    void *var_r4_2;
    u32 var_r5;
    s32 sp20;
    s32 sp1C;
    void *temp_r5;

    sp8 = arg0;
    temp_r0 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    sp38 = temp_r0;
    sp10 = (s32) (u8) ov96_021E5F24(arg0);
    temp_r6 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0xF0);
    ov96_02214418(temp_r0);
    var_r4 = 0;
    sp40 = temp_r0;
    var_r7 = 0;
    sp40 = temp_r0 + 0x5C;
loop_1:
    sp18 = var_r4 / 3;
    temp_r2 = var_r4 % 3;
    temp_r5 = sp40 + (sp18 * 0x174) + (0x7C * temp_r2);
    ov96_021E8BAC(ov96_021EAA20(*(s32 *)temp_r5));
    temp_r0_3 = 3 & ((s32) *(s32 *)((u8 *)temp_r6 + 0x20) >> var_r7);
    sp28 = (s32) temp_r0_3;
    if (sp28 - 3 == 0) {
        ov96_021EB52C(*(s32 *)((u8 *)temp_r5 + 4), 1, 1);
        ov96_021EB570(*(s32 *)((u8 *)temp_r5 + 4), 0xC);
    } else {
        if (sp28 == 2) {
            ov96_021EB52C(*(s32 *)((u8 *)temp_r5 + 4), 1, 1);
            ov96_021EB570(*(s32 *)((u8 *)temp_r5 + 4), 0x10);
            if (IsSEPlaying(0x890U) == 0) {
                PlaySE(0x890U);
            }
        } else {
            ov96_021EB52C(*(s32 *)((u8 *)temp_r5 + 4), 1, 0);
        }
    }
    temp_r0_5 = &temp_r6[var_r4];
    spC = temp_r0_5;
    temp_r0_6 = *(u8 *)(&temp_r6[var_r4]);
    sp34 = (s32) temp_r0_6;
    temp_r0_7 = *(u8 *)((u8 *)temp_r0_5 + 0xC);
    sp30 = (s32) temp_r0_7;
    sp2C = (s32) (u8) (3 & ((s32) *(s32 *)((u8 *)temp_r6 + 0x1C) >> var_r7));
    {
        s32 sp68[3] = {0, 0, 0};
    sp68[0] = temp_r0_6 << 0xC;
    sp68[1] = temp_r0_7 << 0xC;
    sp44 = var_r4 / 3;
    temp_r0_8 = *(u8 *)((u8 *)(ov96_021E60C0(sp8, sp44, var_r4 % 3)) + 7);
    switch (temp_r0_8) {
    case 1:
        sp68[1] -= 0x8000;
        break;
    case 2:
        sp68[1] -= 0x8000;
        break;
    case 3:
        sp68[1] -= 0x10000;
        break;
    default:
        GF_AssertFail();
        break;
    }
    ov96_021EB588(*(s32 *)((u8 *)temp_r5 + 4), sp68);
    }
    if (sp28 == 1) {
        if (*(u8 *)((u8 *)temp_r5 + 0x63) == 0) {
            s32 sp5C[3] = {0, 0, 0};
            temp_r0_9 = ov96_021EAA04(*(s32 *)((u8 *)sp38 + 0x748), (u8) var_r4);
            sp24 = (s32) *(u8 *)((u8 *)temp_r5 + 0x62);
            ov96_021EB06C(temp_r0_9, *(u8 *)spC, *(u8 *)((u8 *)spC + 0xC), &sp4C, &sp48);
            sp5C[0] = sp4C << 0xC;
            sp5C[1] = sp48 << 0xC;
            temp_r0_10 = temp_r5 + (sp24 * 4);
            sp14 = temp_r0_10;
            ov96_021EB588(*(s32 *)((u8 *)temp_r0_10 + 8), sp5C);
            ov96_021EB52C(*(s32 *)((u8 *)sp14 + 8), 1, 1);
            ov96_021EB564(*(s32 *)((u8 *)sp14 + 8), 0xE);
            *(u8 *)((u8 *)temp_r5 + 0x62) = (u8) ((s32) (*(u8 *)((u8 *)temp_r5 + 0x62) + 1) % 3);
            *(u8 *)((u8 *)temp_r5 + 0x63) = 5U;
            if (sp18 == sp10) {
                PlaySE(0x8CCU);
            }
        } else {
            (*(u8 *)((u8 *)temp_r5 + 0x63))--;
        }
    }
    temp_r0_11 = ov96_021EAA04(*(s32 *)((u8 *)sp38 + 0x748), (u8) var_r4);
    if (sp28 == 2) {
        ov96_021EAC5C(temp_r0_11, 0x14);
    } else {
        ov96_021EAC0C(temp_r0_11, (u8) (sp2C + 1));
    }
    ov96_021EB01C(temp_r0_11, sp34, sp30, 1);
    var_r4 += 1;
    var_r7 += 2;
    if (var_r4 < 0xC) {
        goto loop_1;
    }
    ov96_022144C0(sp38 + 0x5C + (0x174 * sp10), temp_r6 + 0xC + (sp10 * 3));
    ov96_0221457C(sp38, temp_r6, sp10);
    temp_r2_2 = *(s32 *)((u8 *)temp_r6 + 0x1C);
    sp1C = (s32) (u8) ((temp_r2_2 >> 0x1E) & 1);
    sp3C = (s32) (u8) ((temp_r2_2 >> 0x1F) & 1);
    var_r5 = 0;
    sp20 = 0;
    var_r4_2 = sp38 + 0x62C;
loop_26:
    {
        s32 sp50[3] = {0, 0, 0};
    temp_r7 = 3 & ((s32) (u8) ((s32) *(s32 *)((u8 *)temp_r6 + 0x1C) >> 0x18) >> sp20);
    if (temp_r7 == 1) {
        ov96_021EB52C(*(s32 *)var_r4_2, 1, 0);
        ov96_021EB52C(*(s32 *)((u8 *)var_r4_2 + 4), 1, 0);
        if ((var_r5 == 0) && (sp1C != 0)) {
            var_r2 = 1;
        } else {
            var_r2 = 0;
        }
        temp_r3 = &temp_r6[var_r5];
        ov96_022123B0(sp38, var_r5, var_r2, *(u8 *)((u8 *)temp_r3 + 0x18), (s32) *(u8 *)((u8 *)temp_r3 + 0x1A), sp10);
    } else {
        *(s8 *)((u8 *)var_r4_2 + 0x3F) = 0;
        if (var_r5 == 1) {
            if (sp3C != 0) {
                ov96_021EB52C(*(s32 *)var_r4_2, 1, 1);
            } else {
                ov96_021EB52C(*(s32 *)var_r4_2, 1, 0);
            }
        } else {
            ov96_021EB52C(*(s32 *)var_r4_2, 1, 1);
        }

        if (temp_r7 == 2) {
            ov96_021EB52C(*(s32 *)((u8 *)var_r4_2 + 4), 1, 1);
            if (*(u8 *)((u8 *)var_r4_2 + 0x44) != 2) {
                PlaySE(0x8C7U);
            }
        } else {
            ov96_021EB52C(*(s32 *)((u8 *)var_r4_2 + 4), 1, 0);
        }
        if ((var_r5 == 0) && (sp1C != 0)) {
            ov96_021EB570(*(s32 *)var_r4_2, 0xB);
        } else {
            ov96_021EB570(*(s32 *)var_r4_2, 0xA);
        }
    }
    *(u8 *)((u8 *)var_r4_2 + 0x44) = temp_r7;
    temp_r7_2 = &temp_r6[var_r5];
    sp50[0] = *(u8 *)((u8 *)temp_r7_2 + 0x18) << 0xC;
    sp50[1] = ov96_02213F5C(*(u8 *)((u8 *)temp_r7_2 + 0x1A));
    ov96_02213FF4(sp38, var_r5, sp1C, *(u8 *)((u8 *)temp_r7_2 + 0x1A));
    ov96_021EB588(*(s32 *)var_r4_2, sp50);
    ov96_021EB588(*(s32 *)((u8 *)var_r4_2 + 4), sp50);

    if ((u32) *(u8 *)((u8 *)temp_r7_2 + 0x1A) < 0xC0U) {
        ov96_021EB630(*(s32 *)var_r4_2, 0x2E);
    } else {
        ov96_021EB630(*(s32 *)var_r4_2, 4);
    }
    var_r5 += 1;
    var_r4_2 += 0x4C;
    sp20 += 2;
    }
    if ((s32) var_r5 < 2) {
        goto loop_26;
    }
    var_r4_3 = sp38;
    var_r5_2 = 0;
    do {
        if (ov96_021EB57C(*(s32 *)((u8 *)var_r4_3 + 0x6D4)) == 0) {
            ov96_021EB52C(*(s32 *)((u8 *)var_r4_3 + 0x6E4), 1, 0);
        }
        var_r5_2 += 1;
        var_r4_3 += 4;
    } while (var_r5_2 < 4);
    ov96_0221236C(sp8);
    temp_r2_3 = ov96_02214904(*(s32 *)((u8 *)sp38 + 0x750));
    ov96_0221497C(*(s32 *)((u8 *)sp38 + 0x750), temp_r2_3);
    ov96_0221490C(*(s32 *)((u8 *)sp38 + 0x750), *(s32 *)((u8 *)sp38 + 0x738));
    ov96_02214490(sp38);
}
