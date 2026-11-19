#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E783C(void *, s32, s32);
s32 ov14_021E7930(void *, u32);
s32 ov14_021F0234(void *, s32 *, s32);
s32 ov14_021F2DE8(void *, u32);

extern s32 ov14_021E92AC;

extern s32 ov14_021E9370;

s32 ov14_021EE5E8(UnkStruct_ov14_boxapp *arg0) {
    s32 *var_r1;
    u8 temp_r1;
    u8 temp_r2;

    temp_r2 = arg0->unk1F;
    temp_r1 = arg0->unk25;
    arg0->unk1F = temp_r1;
    temp_r1 = arg0->unk25;
    if (temp_r2 == temp_r1) {
        return 0x6A;
    }
    if ((u32)temp_r2 > (u32)temp_r1) {
        ov14_021F2DE8(arg0, arg0->unk1F);
        ov14_021E783C(arg0, ov14_021E7930(arg0, arg0->unk1F), 0);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 0);
        var_r1 = &ov14_021E92AC;
    } else {
        ov14_021F2DE8(arg0, arg0->unk1F);
        ov14_021E783C(arg0, ov14_021E7930(arg0, arg0->unk1F), 1);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 0);
        var_r1 = &ov14_021E9370;
    }
    return ov14_021F0234(arg0, var_r1, 0x6A);
}
