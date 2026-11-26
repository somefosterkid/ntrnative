#include "global.h"

#include "sprite_system.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F68C0(void *, s32, s32);

s32 ov14_021EEED4(UnkStruct_ov14_boxapp *arg0) {
    u8 sp0[2];
    u16 var_r4;
    var_r4 = arg0->unk21;
    if (((u32)var_r4) >= 0x1EU) {
        var_r4 = (u16)(var_r4 - 0x1E);
    }
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (u8)var_r4);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, (s32)var_r4), &sp0[1], &sp0[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16)(sp0[1]), (s16)(sp0[0]));
    PlaySE(0x5F3U);
    ov14_021F68C0(arg0, 5, 0x25);
    *((s32 *)&arg0->unk30) = 0x77;
    return 6;
}
