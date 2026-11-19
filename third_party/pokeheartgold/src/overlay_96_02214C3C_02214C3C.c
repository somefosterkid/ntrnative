#include "global.h"

#include "error_handling.h"
#include "overlay_96_app_02210C98.h"
#include "pokeathlon/pokeathlon.h"

void ov96_021E8228(PokeathlonCourseData *data, u32 player, u32 slot, s32 arg3, s32 arg4);

s32 ov96_021EAF8C(void *);

s32 ov96_02214DBC(Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *mover, s32 action, s32 area);

s32 ov96_02215614(s32, s32, s32, s32);

s32 ov96_02215650(s32, s32, s32, s32, s32, s32);

s32 ov96_022156E8(VecFx32 *, s32, VecFx32 *, s32);

s32 ov96_02214C3C(PokeathlonCourseData *arg0, u8 arg1, u8 arg2, Ov96App_02210C98_Unit *arg3, Ov96App_02210C98_Mover *arg4);

s32 ov96_02214C3C(PokeathlonCourseData *arg0, u8 arg1, u8 arg2, Ov96App_02210C98_Unit *arg3, Ov96App_02210C98_Mover *arg4) {
    u8 sp20[2];
    VecFx32 *sp1C;
    s32 sp18;
    s32 sp14;
    s32 temp_r1;
    Ov96App_02210C98_Mover *temp_r1_2;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r4;
    s32 temp_r7;
    s32 var_r0;
    s32 var_r7;
    u8 temp_r0;
    u8 temp_r2_3;
    u8 temp_r3_4;
    u8 var_r4;
    Ov96App_02210C98_Mover *temp_r5;

    if (arg3->unk_78 != 0) {
        return 0;
    }
    if (arg3->unk_48 != 0) {
        return 0;
    }
    temp_r3 = arg3->unk_30.x;
    temp_r4 = arg3->unk_30.y;
    sp14 = ov96_02215614(0x80, 0x60, (temp_r3 / 0x1000), (temp_r4 / 0x1000));
    var_r4 = 0;
    sp1C = &arg3->unk_30;
    do {
        temp_r5 = &arg4[var_r4];
        if ((temp_r5->unk_39 == 0) && (temp_r5->unk_38 != 0)) {
            temp_r2 = temp_r5->unk_8.x;
            temp_r7 = temp_r5->unk_8.y;
            var_r7 = ov96_02215614(0x80, 0x60, (temp_r2 / 0x1000), (temp_r7 / 0x1000));
        } else {
            var_r7 = 4;
        }
        if (var_r7 != 4) {
            sp18 = 0;
            temp_r0 = temp_r5->unk_38;
            if (temp_r0 == 1) {
                sp18 = 8;
            } else if (temp_r0 == 2) {
                sp18 = 0xC;
            } else {
                GF_AssertFail();
            }
            if (ov96_022156E8(sp1C, ov96_021EAF8C(arg3->unk_0) << 0xC, &temp_r5->unk_8, sp18 << 0xC) != 0) {
                sp20[var_r4] = 1;
            } else if (sp14 == var_r7) {
                temp_r1 = temp_r5->unk_8.x;
                temp_r2_2 = temp_r5->unk_8.y;
                temp_r3_2 = arg3->unk_30.x;
                temp_r3_3 = arg3->unk_30.y;
                if (ov96_02215650((temp_r1 / 0x1000), (temp_r2_2 / 0x1000), (temp_r3_2 / 0x1000), (temp_r3_3 / 0x1000), sp14, arg1) != 0) {
                    sp20[var_r4] = 4;
                } else {
                    sp20[var_r4] = 3;
                }
            } else {
                sp20[var_r4] = 2;
            }
        } else {
            sp20[var_r4] = 0;
        }
        var_r4 += 1;
    } while ((u32)var_r4 < 2U);
    temp_r3_4 = sp20[1];
    temp_r2_3 = sp20[0];
    if ((u32)temp_r2_3 >= (u32)temp_r3_4) {
        var_r0 = ov96_02214DBC(arg3, arg4, temp_r2_3, sp14);
    } else {
        temp_r1_2 = arg4 + 1;
        arg4 = temp_r1_2;
        var_r0 = ov96_02214DBC(arg3, temp_r1_2, temp_r3_4, sp14);
    }
    if (arg3->unk_78 == 1) {
        ov96_021E8228(arg0, arg1, arg2, 6, 1);
    }
    return var_r0;
}
