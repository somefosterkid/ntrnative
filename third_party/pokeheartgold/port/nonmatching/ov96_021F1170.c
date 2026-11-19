#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"
s32 ov96_021E8228(PokeathlonCourseData *, u8, u8, s32, s32);
s32 ov96_021F0F04(void *, u8 *);
s32 ov96_021F1CC0(PokeathlonCourseData *);
s32 ov96_021F208C(u8, u8, void *, s32, void *);
s32 ov96_021F20C4(void *, u8, u8, u8);
s32 ov96_021F21EC(void *);
s32 ov96_021F2834(void *);
s32 ov96_021F2A00(void *);
s32 ov96_021F2A84(void *, s32);
s32 ov96_021F2F7C(s32, s32, s32);
s8 ov96_021F32C4(u8);
s32 ov96_021F342C(s32, u8);

typedef struct Ov96CopyBlock { u32 words[9]; } Ov96CopyBlock; /* invented: 0x24-byte block copy */
void ov96_021F1170(PokeathlonCourseData *arg0, s32 arg1) {
    s32 sp28;
    s32 sp24;
    void *sp20;
    u8 *sp1C;
    u8 *sp18;
    u8 *temp_r0_4;
    s32 sp10;
    void *spC;
    void *sp8;
    PokeathlonCourseData *sp4;
    u8 *sp14;
    s32 temp_r0_10;
    s32 temp_r0_5;
    s32 temp_r0_7;
    s32 temp_r1_2;
    s32 temp_r1_4;
    s32 temp_r2_3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r5;
    s32 var_r1;
    s32 var_r2;
    s32 var_r5_2;
    s32 var_r7;
    s8 var_r1_2;
    s32 temp_r0;
    u8 *var_r3;
    u8 *var_r5;
    u8 *var_r5_3;
    u8 temp_r0_2;
    u8 temp_r0_6;
    u8 temp_r1;
    u8 temp_r2;
    u8 temp_r5_2;
    u8 temp_r7;
    u8 var_r6;
    s32 var_r6_2;
    void *temp_r0_11;
    void *temp_r0_8;
    void *temp_r0_9;
    void *temp_r1_5;
    void *temp_r2_2;
    void *temp_r2_4;
    void *temp_r3;
    void *temp_r4;
    void *temp_r6;
    void *temp_r6_2;
    void *var_r5_4;
    void *var_r7_2;

    sp4 = arg0;
    sp14 = PokeathlonCourse_GetDataCopyArea(arg0);
    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    if (ov96_021E5F24(arg0) != 0) {
        return;
    }
    sp1C = ov96_021E8A20(sp14 + 0x28);
    if (*(s32 *)((u8 *)temp_r4 + 0x7EC) == 1) {
        var_r6 = 0;
        temp_r0 = (s32) PokeathlonCourse_GetParticipantCount(sp4);
        var_r7 = 0;
        sp10 = temp_r0;
        if (temp_r0 > 0) {
            var_r5 = sp14 + 0x50;
            do {
                if (*(u8 *)((u8 *)(ov96_021E8A20(var_r5)) + 8) != 0) {
                    var_r6 += 1;
                }
                var_r7 += 1;
                var_r5 += 0x28;
            } while (var_r7 < sp10);
        }
        if (var_r6 == sp10) {
            *(u8 *)((u8 *)temp_r4 + 0x726) = 0U;
            *(u8 *)((u8 *)temp_r4 + 0x74A) = 0U;
            *(s32 *)((u8 *)temp_r4 + 0x7EC) = 0;
        }
    } else if (*(u8 *)((u8 *)temp_r4 + 0x728) != 0) {
        *(u8 *)((u8 *)temp_r4 + 0x728) -= 1;
        temp_r1 = *(u8 *)((u8 *)temp_r4 + 0x728);
        if (temp_r1 == 0) {
            *(s32 *)((u8 *)temp_r4 + 0x7EC) = 1;
        } else if (temp_r1 == 0x3C) {
            *(s8 *)((u8 *)temp_r4 + 0x72A) = 1;
            *(u8 *)((u8 *)temp_r4 + 0x727) += 1;
        }
    } else if (*(u8 *)((u8 *)temp_r4 + 0x74A) != 0) {
        *(u8 *)((u8 *)temp_r4 + 0x726) = 0x2CU;
        if (*(u8 *)((u8 *)temp_r4 + 0x749) == 0) {
            if (*(u8 *)((u8 *)temp_r4 + 0x74D) == 0) {
                temp_r2 = *(u8 *)((u8 *)temp_r4 + 0x74F);
                if ((u32) temp_r2 >= (u32) *(u8 *)((u8 *)temp_r4 + 0x74E)) {
                    ov96_021F2A00(temp_r4);
                    *(u8 *)((u8 *)temp_r4 + 0x728) = 0x5AU;
                } else {
                    *(u8 *)((u8 *)temp_r4 + 0x74B) = (u8) *(u8 *)((u8 *)((temp_r4 + temp_r2)) + 0x750);
                    *(s8 *)((u8 *)temp_r4 + 0x74C) = ov96_021F32C4(*(u8 *)((u8 *)((temp_r4 + *(u8 *)((u8 *)temp_r4 + 0x74F))) + 0x75C));
                    temp_r7 = (u8) ((s32) *(u8 *)((u8 *)temp_r4 + 0x74B) / 3);
                    temp_r6 = temp_r4 + 0x6E0;
                    temp_r5 = temp_r7 * 4;
                    *(s32 *)(temp_r6 + temp_r5) = *(s32 *)(temp_r6 + temp_r5) + *(u8 *)((u8 *)((temp_r4 + *(u8 *)((u8 *)temp_r4 + 0x74F))) + 0x75C);
                    *(u8 *)((u8 *)temp_r4 + 0x74F) += 1;
                    *(u8 *)((u8 *)temp_r4 + 0x74D) = 6U;
                    ov96_021E8228(sp4, temp_r7, (u8) ((s32) *(u8 *)((u8 *)temp_r4 + 0x74B) % 3), 3, 1);
                }
            } else {
                *(u8 *)((u8 *)temp_r4 + 0x74D) -= 1;
            }
        } else {
            *(u8 *)((u8 *)temp_r4 + 0x749) -= 1;
        }
    } else {
        temp_r0_2 = *(u8 *)((u8 *)temp_r4 + 0x727);
        if ((u32) temp_r0_2 < 1U) {
            var_r2 = 8;
        } else if ((u32) temp_r0_2 < 3U) {
            var_r2 = 6;
        } else {
            var_r2 = 4;
        }
        *(u8 *)((u8 *)temp_r4 + 0x725) += 1;
        if ((s32) *(u8 *)((u8 *)temp_r4 + 0x725) >= var_r2) {
            var_r1 = 0;
            *(u8 *)((u8 *)temp_r4 + 0x725) = 0U;
            *(u8 *)((u8 *)temp_r4 + 0x726) += 1;
            if ((u32) *(u8 *)((u8 *)temp_r4 + 0x726) >= 0x2BU) {
                *(s8 *)((u8 *)temp_r4 + 0x72B) = 1;
                *(s8 *)((u8 *)temp_r4 + 0x748) = 1;
                *(u8 *)((u8 *)temp_r4 + 0x74A) = 1U;
                *(u8 *)((u8 *)temp_r4 + 0x749) = 0x14U;
                do {
                    temp_r2_2 = temp_r4 + var_r1;
                    *(s8 *)((u8 *)temp_r2_2 + 0x720) = 0xC;
                    var_r1 += 1;
                } while (var_r1 < 4);
                var_r5_2 = 0;
                sp20 = temp_r4;
                sp20 = temp_r4 + 0x20;
                do {
                    temp_r1_2 = var_r5_2 % 3;
                    temp_r2_3 = (var_r5_2 / 3) * 0x1B0;
                    temp_r6_2 = sp20 + temp_r2_3 + (0x90 * temp_r1_2);
                    var_r5_2 += 1;
                    temp_r3 = temp_r6_2 + 0x28;
                    temp_r2_4 = temp_r6_2 + 0x1C;
                    *(VecFx32 *)((u8 *)temp_r6_2 + 0x1C) = *(VecFx32 *)((u8 *)temp_r6_2 + 0x28);
                    *(s32 *)((u8 *)temp_r6_2 + 0x34) = 0;
                    *(s32 *)((u8 *)temp_r6_2 + 0x38) = 0;
                    *(u16 *)((u8 *)temp_r6_2 + 0x8E) = (u16) *(u16 *)((u8 *)temp_r6_2 + 0x8C);
                    *(s8 *)((u8 *)temp_r6_2 + 0x44) = 0;
                    *(s8 *)((u8 *)temp_r6_2 + 0x45) = 0;
                    *(s8 *)((u8 *)temp_r6_2 + 0x46) = 0;
                    *(s8 *)((u8 *)temp_r6_2 + 0x47) = 0;
                } while (var_r5_2 < 0xC);
            }
        }
    }
    if ((u32) *(u8 *)((u8 *)temp_r4 + 0x727) >= 6U) {
        var_r1_2 = 1;
    } else {
        var_r1_2 = 0;
    }
    *(s8 *)((u8 *)temp_r4 + 0x729) = var_r1_2;
    var_r5_3 = ov96_021E8A20(sp14 + 0x50);
    var_r3 = ov96_021E8A20(sp14);
    *(Ov96CopyBlock *)var_r5_3 = *(Ov96CopyBlock *)var_r3;
    if (*(u8 *)((u8 *)temp_r4 + 0x74A) != 0) {

    } else {
        var_r6_2 = 0;
        sp14 += 0x50;
        var_r5_4 = temp_r4;
        var_r7_2 = temp_r4 + 0x20;
loop_43:
        temp_r0_4 = ov96_021E8A20(sp14);
        sp18 = temp_r0_4;
        if (*(s32 *)temp_r0_4 != 0) {
            temp_r1_4 = *(s32 *)((u8 *)var_r5_4 + 0x6F0);
            if ((temp_r1_4 != 0) && (*(s32 *)((u8 *)var_r5_4 + 0x6F4) != 0)) {
                *(s32 *)((u8 *)var_r5_4 + 0x6F0) = 0;
            } else if ((temp_r1_4 == 0) && (*(s32 *)((u8 *)var_r5_4 + 0x6F4) == 0)) {
                *(s32 *)((u8 *)var_r5_4 + 0x6F0) = 1;
                *(s32 *)((u8 *)var_r5_4 + 0x6F4) = 1;
            }
        } else {
            *(s32 *)((u8 *)var_r5_4 + 0x6F0) = 0;
            *(s32 *)((u8 *)var_r5_4 + 0x6F4) = 0;
        }
        if (*(s32 *)((u8 *)var_r5_4 + 0x6F0) != 0) {
            if (ov96_021F2A84(temp_r4, var_r6_2) == 0) {
                temp_r0_5 = ov96_021F20C4(temp_r4, var_r6_2, *(u8 *)((u8 *)sp18 + 4), *(u8 *)((u8 *)sp18 + 5));
                sp24 = temp_r0_5;
                if (temp_r0_5 != 0xC) {
                    temp_r3_2 = (temp_r0_5 % 3) * 0x90;
                    *(s8 *)((u8 *)((var_r7_2 + temp_r3_2)) + 0x46) = 2;
                    *(s8 *)((u8 *)((temp_r4 + var_r6_2)) + 0x720) = (s8) sp24;
                    *(s8 *)((u8 *)((var_r7_2 + temp_r3_2)) + 0x47) = 3;
                }
            }
        } else if (*(s32 *)((u8 *)var_r5_4 + 0x6F4) != 0) {
            temp_r1_5 = temp_r4 + var_r6_2;
            temp_r0_6 = *(u8 *)((u8 *)temp_r1_5 + 0x720);
            spC = temp_r1_5;
            if (temp_r0_6 != 0xC) {
                temp_r0_7 = 0x90 * (u8) ((s32) temp_r0_6 % 3);
                sp28 = temp_r0_7;
                temp_r0_8 = var_r7_2 + temp_r0_7;
                sp8 = temp_r0_8;
                ov96_021F208C(*(u8 *)((u8 *)sp18 + 4), *(u8 *)((u8 *)sp18 + 5), sp8 + 0x28, 0, temp_r0_8 + 0x1C);
                if (ov96_021F2F7C(*(s32 *)(var_r7_2 + temp_r0_7), *(s32 *)((u8 *)sp8 + 0x1C), *(s32 *)((u8 *)sp8 + 0x20)) != 0) {
                    *(s8 *)((u8 *)((temp_r4 + *(u8 *)((u8 *)spC + 0x720))) + 0x7E0) = 1;
                } else {
                    *(s8 *)((u8 *)((temp_r4 + *(u8 *)((u8 *)spC + 0x720))) + 0x7E0) = 0;
                }
            }
        } else {
            temp_r0_9 = temp_r4 + var_r6_2;
            if (*(u8 *)((u8 *)temp_r0_9 + 0x720) != 0xC) {
                *(u8 *)((u8 *)temp_r0_9 + 0x720) = 0xCU;
            }
        }
        var_r6_2 += 1;
        sp14 += 0x28;
        var_r5_4 += 0xC;
        var_r7_2 += 0x1B0;
        if ((s32) var_r6_2 < 4) {
            goto loop_43;
        }
        temp_r0_10 = ov96_021F342C(*(s32 *)((u8 *)temp_r4 + 0x770), (u8) *(u16 *)((u8 *)temp_r4 + 0x732));
        if (temp_r0_10 != 0xC) {
            temp_r5_2 = (u8) (temp_r0_10 / 3);
            if ((u32) temp_r5_2 >= (u32) PokeathlonCourse_GetParticipantCount(sp4)) {
                temp_r0_11 = temp_r4 + 0x20 + (0x1B0 * temp_r5_2);
                temp_r3_3 = (temp_r0_10 % 3) * 0x90;
                *(s8 *)((u8 *)((temp_r0_11 + temp_r3_3)) + 0x46) = 2;
                *(s8 *)((u8 *)((temp_r0_11 + temp_r3_3)) + 0x47) = 3;
            } else {
                GF_AssertFail();
            }
        }
    }
    ov96_021F1CC0(sp4);
    ov96_021F21EC(temp_r4);
    ov96_021F2834(temp_r4);
    ov96_021F0F04(temp_r4, sp1C);
}
