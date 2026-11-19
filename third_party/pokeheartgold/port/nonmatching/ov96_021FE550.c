/* PORTOK hk42 */
#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon.h"
s32 ov96_021E60C0(PokeathlonCourseData *, s32, s32 arg2);
s32 ov96_021EAB38(s32, s32);
s32 ov96_021EAC5C(s32, s32);
s32 ov96_021EAF94(s32, s32, u8);
s32 ov96_021EB52C(s32, s32, s32);
s32 ov96_021EB564(s32, s32);
s32 ov96_021EB570(s32, s32);
s32 ov96_021EB588(s32, s32 *);
s32 ov96_021FEECC(s32, s32 *, s32 *, u8 *, s32, s32, u8 *, s32, s32, s32);
s32 ov96_021FFAEC(s32, s32 *, s32);
s32 ov96_021FFB7C(s32 *, u8, s32);
s32 ov96_021FFE38(s32, s32, s32);
s32 ov96_02200950(s32, u8, s32, s32, s32);
s32 ov96_02200A18(s32, u8, u8);
s32 ov96_02200BC8(s32, s32);
s32 sub_0200606C(s32, s32);
s32 sub_02006190(s32);

void ov96_021FE550(PokeathlonCourseData *arg0, s32 arg1, s32 *arg2, void *arg3) {
    void *sp5C;
    s32 sp7C[3];
    s32 sp64[3];
    s32 sp70[3];
    s32 sp60;
    s32 sp54;
    u8 *sp58;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    u16 *sp44;
    s32 sp40;
    void *sp3C;
    s32 sp38;
    u8 *temp_r0;
    s32 sp34;
    s32 sp30;
    u8 *sp2C;
    s32 sp28;
    s32 sp24;
    void *sp20;
    s32 sp1C;
    PokeathlonCourseData *sp18;
    s32 var_r7;
    s32 var_r7_2;
    s32 var_r6;
    s32 temp_r6;
    s32 *var_r5;
    s32 temp_r5;
    s32 var_r4;
    s32 temp_r4;
    s32 var_r4_2;
    s32 *var_r4_3;
    s32 var_r4_4;
    u16 temp_r4_2;
    s32 temp_r4_3;
    s32 temp_r3_2;
    s32 var_r2;
    u8 *temp_r1_2;
    u8 *var_r0;
    s32 temp_r0_4;
    u8 *temp_r0_2;
    u8 temp_r0_3;

    sp18 = arg0;
    sp1C = arg1;
    var_r5 = arg2;
    sp20 = arg3;
    temp_r0 = ov96_021E8A20(arg3 + 0xF0);
    sp58 = temp_r0;
    sp5C = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    sp60 = *(u16 *)(temp_r0 + (sp1C * 2));
    sp7C[0] = 0;
    sp7C[1] = 0;
    sp7C[2] = 0;
    if ((u8) (1 & ((s32) *(s32 *)((u8 *)temp_r0 + 0x14) >> 0x19)) != 0) {
        temp_r4 = sp60 - *(u16 *)((u8 *)sp58 + 0x1A);
        if ((double) temp_r4 >= 768.0) {
            var_r4 = (s32) (80.0 + (1024.0 - (double) temp_r4));
        } else {
            var_r4 = 0x50 - temp_r4;
        }
        sp7C[0] = var_r4 << 0xC;
        sp7C[1] = *(u8 *)((u8 *)sp58 + 0x1C) << 0xC;
        ov96_021EB588(*(s32 *)((u8 *)((sp5C + (*(u8 *)((u8 *)sp5C + 0x3C3) * 4))) + 0x3CC), sp7C);
        ov96_021EB52C(*(s32 *)((u8 *)((sp5C + (*(u8 *)((u8 *)sp5C + 0x3C3) * 4))) + 0x3CC), 1, 1);
        ov96_021EB564(*(s32 *)((u8 *)((sp5C + (*(u8 *)((u8 *)sp5C + 0x3C3) * 4))) + 0x3CC), 9);
        *(u8 *)((u8 *)sp5C + 0x3C3) = (u8) (((*(u8 *)((u8 *)sp5C + 0x3C3) + 1) % 4));
        if ((var_r4 >= 0) && (var_r4 < 0x100)) {
            sub_0200606C(0x8A3, 5);
        }
    }
    if (sp1C == 0) {
        sp28 = 1;
    } else {
        sp28 = 0;
    }
    sp30 = 0;
    sp44 = (u16 *)sp58;
    sp40 = 0;
    sp3C = sp20;
    sp3C = sp20 + 0x50;
loop_11:
    if (sp1C == sp30) {
        sp24 = 1;
    } else {
        sp24 = 0;
    }
    temp_r0_2 = &sp58[sp30];
    sp2C = temp_r0_2;
    temp_r0_3 = *(u8 *)((u8 *)temp_r0_2 + 0xC);
    sp38 = (s32) temp_r0_3;
    sp34 = (s32) (u8) (temp_r0_3 & 3);
    temp_r4_2 = *sp44;
    var_r6 = temp_r4_2 - sp60;
    if ((double) sp60 > 724.0) {
        if ((u32) temp_r4_2 < 0x12CU) {
            var_r6 = (s32) ((1024.0 - (double) sp60) + (double) temp_r4_2);
        }
    } else if ((sp60 < 0x64) && ((double) temp_r4_2 > 924.0)) {
        var_r6 = (s32) (0.0 - ((double) sp60 + (1024.0 - (double) temp_r4_2)));
    }
    temp_r6 = var_r6 + 0x50;
    temp_r1_2 = &sp58[sp30];
    sp50 = (s32) (u8) (1 & ((s32) *(u8 *)((u8 *)temp_r1_2 + 0x10) >> 7));
    sp48 = ((s32) *(s32 *)((u8 *)sp58 + 0x14) >> ((u32) ((sp34 + sp40) << 0x18) >> 0x17)) & 3;
    sp4C = (s32) (u8) ((sp38 >> 2) & 3);
    sp54 = *(s32 *)(((u8 *)var_r5) + (sp34 * 4));
    sp70[0] = temp_r6 << 0xC;
    sp70[1] = *(u8 *)((u8 *)temp_r1_2 + 8) << 0xC;
    sp70[2] = 0;
    if ((s32) PokeathlonCourse_GetParticipantCount(sp18) <= sp30) {
        var_r4_2 = 1;
        var_r0 = ov96_021E8A20(sp3C);
    } else {
        var_r4_2 = 0;
        var_r0 = ov96_021E8A20(sp20);
    }
    if (sp1C == sp30) {
        ov96_021FEECC(*(s32 *)((u8 *)sp5C + 0x3E0), var_r5, sp70, sp58, (s32) (u8) sp30, sp34, var_r0, sp28, 1, 0);
    } else {
        ov96_021FEECC(*(s32 *)((u8 *)sp5C + 0x3E0), var_r5, sp70, sp58, (s32) (u8) sp30, sp34, var_r0, sp28, 0, var_r4_2);
    }
    ov96_02200950(*(s32 *)((u8 *)sp5C + 0x3E0), (u8) sp30, sp34, sp50, sp48);
    if ((*(s32 *)((u8 *)var_r5 + 0xC8) == 0) && (temp_r6 >= -0x20) && (temp_r6 <= 0x120)) {
        ov96_021EAB38(sp54, 1);
    } else {
        ov96_021EAB38(sp54, 0);
    }
    var_r7 = 0;
    var_r4_3 = var_r5;
    do {
        ov96_021EAF94(*var_r4_3, temp_r6, *(u8 *)((u8 *)sp2C + 8));
        var_r7 += 1;
        var_r4_3 += 1;
    } while (var_r7 < 3);
    if (sp30 == ov96_021E5F24(sp18)) {
        var_r2 = 1;
    } else {
        var_r2 = 0;
    }
    ov96_021FFB7C(var_r5, *(u8 *)((u8 *)sp2C + 8), var_r2);
    ov96_021FFAEC(ov96_021E60C0(sp18, sp30, sp34), sp70, *(s32 *)((u8 *)var_r5 + 0x6C));
    *(VecFx32 *)((u8 *)sp64) = *(VecFx32 *)((u8 *)sp70);
    sp64[0] -= 0x18000;
    ov96_021EB588(*(s32 *)((u8 *)var_r5 + 0x78), sp64);
    if (sp50 == 0) {
        var_r4_4 = 0;
        var_r7_2 = 0;
        switch (sp4C) {
        case 2:
            if (sp4C != *(u8 *)((u8 *)var_r5 + 0xA7)) {
                ov96_021EB52C(*(s32 *)((u8 *)var_r5 + 0x78), 1, 1);
                ov96_021EB564(*(s32 *)((u8 *)var_r5 + 0x78), 4);
            }
            ov96_021EAC5C(sp54, 8);
            var_r4_4 = 1;
            if (((u32) *(u8 *)((u8 *)var_r5 + 0xD2) <= 1U) && (*(u8 *)((u8 *)var_r5 + 0xD3) == 0)) {
                ov96_021FFE38(temp_r6, 0x8A7, sp24);
                *(u8 *)((u8 *)var_r5 + 0xD2) = 2U;
                *(u8 *)((u8 *)var_r5 + 0xD3) = 1U;
            }
            break;
        case 3:
            if (sp4C != *(u8 *)((u8 *)var_r5 + 0xA7)) {
                ov96_021EB52C(*(s32 *)((u8 *)var_r5 + 0x78), 1, 1);
                ov96_021EB564(*(s32 *)((u8 *)var_r5 + 0x78), 5);
            }
            ov96_021EAC5C(sp54, 8);
            var_r4_4 = 1;
            if (((u32) *(u8 *)((u8 *)var_r5 + 0xD2) <= 1U) && (*(u8 *)((u8 *)var_r5 + 0xD3) == 0)) {
                ov96_021FFE38(temp_r6, 0x8A7, sp24);
                *(u8 *)((u8 *)var_r5 + 0xD2) = 3U;
                *(u8 *)((u8 *)var_r5 + 0xD3) = 1U;
            }
            break;
        default:
            ov96_021EB52C(*(s32 *)((u8 *)var_r5 + 0x78), 1, 0);
            *(u8 *)((u8 *)var_r5 + 0xD2) = (u8) sp4C;
            *(u8 *)((u8 *)var_r5 + 0xD3) = 0U;
            break;
        }
        switch (sp48) {
        case 1:
            ov96_021EB52C(*(s32 *)((u8 *)var_r5 + 0x6C), 1, 1);
            ov96_021EB570(*(s32 *)((u8 *)var_r5 + 0x6C), 6);
            break;
        case 2:
            if (sp48 != *(u8 *)((u8 *)var_r5 + 0xA6)) {
                ov96_021EB52C(*(s32 *)((u8 *)var_r5 + 0x6C), 1, 1);
                ov96_021EB564(*(s32 *)((u8 *)var_r5 + 0x6C), 7);
            }
            ov96_021EAC5C(sp54, 0x15);
            var_r7_2 = 1;
            if (sp24 != 0) {
                if (sub_02006190(3) == 0) {
                    ov96_021FFE38(temp_r6, 0x890, sp24);
                }
            } else if (sub_02006190(4) == 0) {
                ov96_021FFE38(temp_r6, 0x890, sp24);
            }
            break;
        default:
            ov96_021EB52C(*(s32 *)((u8 *)var_r5 + 0x6C), 1, 0);
            break;
        }
        if ((var_r4_4 == 0) && (var_r7_2 == 0)) {
            ov96_021EAC5C(sp54, 0);
        }
    } else {
        ov96_021EB52C(*(s32 *)((u8 *)var_r5 + 0x78), 1, 0);
        ov96_021EB52C(*(s32 *)((u8 *)var_r5 + 0x6C), 1, 0);
    }
    *(u8 *)((u8 *)var_r5 + 0xA7) = (u8) sp4C;
    *(u8 *)((u8 *)var_r5 + 0xA6) = (u8) sp48;
    var_r5 = (void *)((u8 *)var_r5 + (0xD4));
    sp44 += 1;
    sp40 += 3;
    sp3C += 0x28;
    temp_r0_4 = sp30 + 1;
    sp30 = temp_r0_4;
    if (temp_r0_4 < 4) {
        goto loop_11;
    }
    temp_r4_3 = ov96_02200BC8(*(s32 *)((u8 *)sp5C + 0x3E0), 0);
    temp_r5 = *(s32 *)((u8 *)sp58 + 0x14);
    temp_r3_2 = sp1C * 3;
    ov96_02200A18(*(s32 *)((u8 *)sp5C + 0x3E0), (u8) ((temp_r5 >> ((u32) ((temp_r4_3 + temp_r3_2) << 0x18) >> 0x17)) & 3), (u8) (3 & (temp_r5 >> ((u32) ((ov96_02200BC8(*(s32 *)((u8 *)sp5C + 0x3E0), 1) + temp_r3_2) << 0x18) >> 0x17))));
}
