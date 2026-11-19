#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void ov96_021E8228(PokeathlonCourseData *, u32, u32, s32, s32);

void ov96_021F64A0(s32 (*arg0)[0x80], s32 arg1, s32 *arg2);

void ov96_021F6B28(UnkStruct_ov96_021F5018_Ball *arg0, UnkStruct_ov96_021F5018_Row *arg1);

void ov96_021F6BE4(UnkStruct_ov96_021F5018 *param_1, s32 param_2, u8 param_3, s32 param_4);

void ov96_021F7130(UnkStruct_ov96_021F5018_Ball *arg0);

s32 sub_0200606C(s32, s32);

extern u8 ov96_0221DC18[4];

void ov96_021F6798(PokeathlonCourseData *arg0, UnkStruct_ov96_021F5018 *arg1, s32 arg2);

void ov96_021F6798(PokeathlonCourseData *arg0, UnkStruct_ov96_021F5018 *arg1, s32 arg2) {
    s32 sp1C[128];
    UnkStruct_ov96_021F5018_Row *st;
    s32 r7;
    s32 spC;
    s32 idx;
    PokeathlonCourseData *sp4;

    sp4 = arg0;
    st = &arg1->unk_FAC[arg2];
    if (st->unk_14 == 0) {
        return;
    }
    spC = (u8)ov96_021E5F24(arg0);
    r7 = arg1->unk_FA0[arg2];
    switch (arg2) {
    case 0:
        idx = 6;
        break;
    case 1:
        idx = 5;
        break;
    case 2:
        idx = 3;
        break;
    default:
        GF_AssertFail();
        return;
    }
    ov96_021F64A0(arg1->unk_1A0, idx, sp1C);
    if (st->unk_10 != 0) {
        if (st->unk_18 <= 0) {
            GF_AssertFail();
            st->unk_10 = 0;
            st->unk_08 = sp1C[r7];
            st->unk_18 = 0;
            st->unk_14 = 0;
            st->unk_00 = 0;
            arg1->unk_90[arg2].unk_28 = 1;
            ov96_021F6BE4(arg1, arg2, r7, 0);
            return;
        }
        st->unk_08 += st->unk_00;
        if (arg1->unk_90[arg2].unk_18 != 0) {
            ov96_021F6B28(&arg1->unk_90[arg2], st);
        }
        if (st->unk_08 > sp1C[r7]) {
            st->unk_00 = (s32)((double)st->unk_00 - 128.0);
            if (-st->unk_00 > st->unk_18) {
                st->unk_00 = -st->unk_18;
            }
        } else if (st->unk_08 <= sp1C[r7]) {
            if (st->unk_00 == 0) {
                st->unk_10 = 0;
                st->unk_08 = sp1C[r7];
                st->unk_18 = 0;
                st->unk_14 = 0;
                ov96_021F6BE4(arg1, arg2, r7, 0);
            } else if (st->unk_00 < 0) {
                st->unk_10 = 0;
                ov96_021F7130(&arg1->unk_90[arg2]);
                if ((double)st->unk_00 > -128.0) {
                    st->unk_00 = -0x80;
                }
                st->unk_18 = st->unk_00;
            } else {
                st->unk_10 = 0;
                st->unk_08 = sp1C[r7];
                st->unk_18 = 0;
                st->unk_14 = 0;
                st->unk_00 = 0;
                ov96_021F6BE4(arg1, arg2, r7, 0);
            }
        }
    } else {
        if (st->unk_18 > 0) {
            GF_AssertFail();
        }
        st->unk_08 += st->unk_00;
        if ((double)st->unk_08 < -6144.0) {
            st->unk_08 = -0x1800;
            st->unk_00 = 0;
        }
        if (st->unk_08 >= sp1C[r7]) {
            ov96_021F6BE4(arg1, arg2, r7, 0);
            if (arg1->unk_90[arg2].unk_28 != 0) {
                st->unk_00 = (st->unk_00 * arg1->unk_90[arg2].unk_2C) / 12;
            }
            if (st->unk_00 > 0x898) {
                st->unk_00 = 0x898;
            }
            st->unk_00 = (st->unk_00 * 9) / 10;
            if (st->unk_00 == 0) {
                if (arg1->unk_90[arg2].unk_28 != 0) {
                    sub_0200606C(0x5F3, ov96_0221DC18[3]);
                }
                st->unk_10 = 0;
                st->unk_08 = sp1C[r7];
                st->unk_18 = 0;
                st->unk_14 = 0;
                ov96_021F6BE4(arg1, arg2, r7, 0);
            } else if (st->unk_00 > 0) {
                if ((double)st->unk_00 <= 1408.0 && arg1->unk_90[arg2].unk_28 == 0) {
                    st->unk_10 = 0;
                    st->unk_08 = sp1C[r7];
                    st->unk_18 = 0;
                    st->unk_00 = 0;
                    st->unk_14 = 0;
                    ov96_021F6BE4(arg1, arg2, r7, 0);
                } else {
                    st->unk_10 = 1;
                    st->unk_18 = st->unk_00;
                    arg1->unk_90[arg2].unk_28 = 0;
                    sub_0200606C(0x656, ov96_0221DC18[3]);
                    ov96_021E8228(sp4, spC, arg2, 2, 1);
                }
            }
        } else if (st->unk_08 < sp1C[r7]) {
            if (st->unk_00 < 0) {
                st->unk_00 += 819.2;
                if (st->unk_00 >= 0) {
                    st->unk_00 = 0;
                }
            } else if (st->unk_00 >= 0) {
                st->unk_00 += 341.3333333333333;
                if (st->unk_18 >= 0) {
                    GF_AssertFail();
                    st->unk_10 = 0;
                    st->unk_08 = sp1C[r7];
                    st->unk_18 = 0;
                    st->unk_14 = 0;
                    st->unk_00 = 0;
                    ov96_021F6BE4(arg1, arg2, r7, 0);
                    return;
                } else if (st->unk_00 > -st->unk_18) {
                    st->unk_00 = -st->unk_18;
                }
            }
        }
    }
    if (st->unk_10 == 0 || st->unk_14 == 0 || st->unk_00 == 0) {
        arg1->unk_90[arg2].unk_24 = 0;
    }
    if (st->unk_10 == 0 && st->unk_14 != 0) {
        ov96_021F6BE4(arg1, arg2, r7, st->unk_08);
    }
    if (st->unk_14 == 0) {
        arg1->unk_90[arg2].unk_28 = 1;
    }
}
