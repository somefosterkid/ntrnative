#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F7878(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    u8 sp0[2];
    s32 var_r4;
    u8 temp_r1;
    UnkStruct_ov14_boxsys *temp_r0;

    var_r4 = arg1;
    if ((u32)(var_r4 - 4) <= 1U) {
        temp_r0 = arg0->sys;
        temp_r1 = *((u8 *)&temp_r0->unk_44D);
        var_r4 = (temp_r1 % 4);
        temp_r0->unk_43C = var_r4;
    }
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8)var_r4);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, var_r4), &sp0[1], &sp0[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16) sp0[1], (s16) sp0[0]);
    if ((var_r4 >= 0) && (var_r4 <= 3)) {
        ov14_021F29E4(arg0->sys, 9, 0xE);
        return;
    }
    ov14_021F29E4(arg0->sys, 9, 8);
}
