#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

s32 ov96_021FC698(void *, u32, Vec32 *);

extern Vec32 ov96_0221C428;

void ov96_021FB6C8(UnkStruct_ov96_021F94A8 *arg0, UnkStruct_ov96_021FAB24 *arg1);

void ov96_021FB6C8(UnkStruct_ov96_021F94A8 *arg0, UnkStruct_ov96_021FAB24 *arg1) {
    Vec32 spC = ov96_0221C428;
    s32 temp_r1;
    s32 var_r0;
    u8 temp_r0;

    temp_r1 = arg1->unk_28;
    if ((temp_r1 != 1) && (arg1->unk_8 == 0)) {
        if (temp_r1 == 2) {
            var_r0 = 0x18;
        } else {
            temp_r0 = arg1->unk_9;
            switch (temp_r0) {
            case 0:
                var_r0 = 8;
                break;
            case 1:
                var_r0 = 6;
                break;
            case 2:
                var_r0 = 6;
                break;
            case 3:
                var_r0 = 6;
                break;
            case 4:
                var_r0 = 4;
                break;
            default:
                GF_AssertFail();
                var_r0 = 8;
                break;
            }
        }
        arg1->unk_6A = (u16)(arg1->unk_6A + 1);
        if ((s32) arg1->unk_6A >= var_r0) {
            Vec32 sp0 = { 0, 0, 0 };
            arg1->unk_6A = 0U;
            sp0.x = (((&spC.x)[arg1->unk_18] + (((s32)LCRandom() % 7) - 3)) << 0xC);
            sp0.y = 0x188000;
            ov96_021FC698(arg0->unk_DC, arg1->unk_18, &sp0);
        }
    }
}
