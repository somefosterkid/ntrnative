#include "global.h"

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F6C94(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    u8 py[2];
    s32 v0;

    v0 = arg1;
    if ((u32)(v0 - 6) <= 1U) {
        v0 = (s32) arg0->unk25 % 6;
        arg0->sys->unk_43C = v0;
    }
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8)v0);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, v0), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16) py[1], (s16) py[0]);
    ov14_021F2A18(arg0->sys, 9, 1);
    if ((v0 >= 0) && (v0 <= 5)) {
        ov14_021F29E4(arg0->sys, 9, 0xE);
        return;
    }
    ov14_021F29E4(arg0->sys, 9, 8);
}
