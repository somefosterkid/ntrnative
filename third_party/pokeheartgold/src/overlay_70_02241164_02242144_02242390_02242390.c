#include "global.h"

#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02238F9C(Sprite *, s32, s32);

s32 ov70_02242164(UnkStruct_ov70_02242E58 *, s32);

s32 ov70_02242364(UnkStruct_ov70_02242E58 *arg0, s32 arg1);

extern s32 ov70_02245DC0;

extern s32 ov70_02245DC1;

extern s32 ov70_02245DC2;

extern s32 ov70_02245DC3;

extern u8 ov70_02245E26[];

extern u8 ov70_02245E27[];

extern s32 ov70_02245E84[];

s32 ov70_02242390(UnkStruct_ov70_02242E58 *arg0, u8 *flags);

s32 ov70_02242390(UnkStruct_ov70_02242E58 *arg0, u8 *flags) {
    Sprite *new_var;
    s32 temp_r1;
    s32 temp_r0;
    s32 var_r0;
    temp_r1 = arg0->unk_48;
    if (temp_r1 < 3) {
        arg0->unk_7E = (u8)temp_r1;
    }
    if (0x40 & gSystem.newAndRepeatedKeys) {
        arg0->unk_48 = (s32)(*((u8 *)((&ov70_02245DC0) + (arg0->unk_48))));
    } else if (0x80 & gSystem.newAndRepeatedKeys) {
        arg0->unk_48 = (s32)(*((u8 *)((&ov70_02245DC1) + (arg0->unk_48))));
    } else if (0x20 & gSystem.newAndRepeatedKeys) {
        arg0->unk_48 = (s32)(*((u8 *)((&ov70_02245DC2) + (arg0->unk_48))));
    } else if (gSystem.newAndRepeatedKeys & 0x10) {
        arg0->unk_48 = (s32)(*((u8 *)((&ov70_02245DC3) + (arg0->unk_48))));
    }
    if ((temp_r1 == 3) && (((s32)(arg0->unk_48)) < 3)) {
        arg0->unk_48 = (s32)(arg0->unk_7E);
    }
    if (temp_r1 != (arg0->unk_48)) {
        PlaySE(0x5DCU);
        temp_r0 = arg0->unk_48;
        if (temp_r0 == 3) {
            ov70_02238F9C(arg0->unk_C, 0xC0, 0x88);
            Sprite_SetAnimCtrlSeq(arg0->unk_C, 0x30);
        } else {
            temp_r1 = temp_r0 * 2;
            ov70_02238F9C(arg0->unk_C, (ov70_02245E26[temp_r1] + 0x10) * 8, ov70_02245E27[temp_r1] * 8);
            new_var = arg0->unk_C;
            Sprite_SetAnimCtrlSeq(new_var, 0x2F);
        }
    }
    var_r0 = ov70_02242164(arg0, 5);
    if (var_r0 != (-1)) {
        var_r0 = ov70_02242364(arg0, var_r0);
        if (var_r0 != (-1)) {
            if (((var_r0 == (-2)) || (flags == NULL)) || ((flags + var_r0)[ov70_02245E84[arg0->unk_3C]] != 0)) {
                PlaySE(0x5DCU);
                return var_r0;
            }
            goto block_31;
        }
        return var_r0;
    }
    if (1 & gSystem.newKeys) {
        var_r0 = ov70_02242364(arg0, arg0->unk_48);
        if (var_r0 != (-1)) {
            if (((var_r0 == (-2)) || (flags == NULL)) || ((flags + var_r0)[ov70_02245E84[arg0->unk_3C]] != 0)) {
                PlaySE(0x5DCU);
                return var_r0;
            }
            goto block_31;
        }
        return var_r0;
    }
    if (2 & gSystem.newKeys) {
        PlaySE(0x5DCU);
        return -2;
    }
block_31:
    var_r0 = -1;

    return var_r0;
}
