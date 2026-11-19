#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F5FBC(void *, s32);
s32 ov14_021F68C0(void *, s32, s32);

s32 ov14_021EED28(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 sp0[2];
    u16 var_r4;
    if (ov14_021F2A04(*((void * *)&arg0->sys), 0xB, arg2, arg3) == 0) {
        var_r4 = arg0->unk21;
        if (((u32)var_r4) >= 0x1EU) {
            var_r4 = (u16)(var_r4 - 0x1E);
        }
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (u8)var_r4);
        DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, (s32)var_r4), &sp0[1], &sp0[0]);
        ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16)(sp0[1]), (s16)(sp0[0]));
        ov14_021F391C(*((void * *)&arg0->sys), 0);
        ov14_021F2A18(*((void * *)&arg0->sys), 0xB, 0);
        ov14_021F68C0(arg0, 3, 0x25);
        ov14_021F5FBC(arg0, 0);
        *((s16 *)&arg0->sys->unk_88C8) = 0;
        *((s32 *)&arg0->unk30) = 0x77;
        return 6;
    }
    return 0x79;
}
