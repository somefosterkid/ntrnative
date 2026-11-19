#include "global.h"

#include "unk_02019BA4.h"
#include "sprite_system.h"
#include "unk_02020A0C.h"
#include "field_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);
s32 ov14_021F70C0(FieldSystem *arg0);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F70A4(void *arg0, u8 arg1) {
    unsigned char new_var;

    new_var = (s32)arg1;
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, new_var);
    ov14_021F6B28(arg0, arg1);
}

s32 ov14_021F70C0(FieldSystem *arg0) {
    s32 v0;

    if (ov14_021E85E4(((UnkStruct_ov14_boxsys *)arg0->areaDataManager)->unk_2F0) == 0) {
        GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x2DU);
    }
    v0 = GridInputHandler_HandleInput_AllowHold(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C));
    GridInputHandler_SetAllEnabled(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C));
    return v0;
}

void ov14_021F70F4(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    u8 py[2];
    s32 v1;
    UnkStruct_ov14_boxsys *v0;

    v1 = arg1;
    if ((u32)(v1 - 0x2B) <= 1U) {
        v1 = ((s32) arg0->unk25 % 6) + 0x25;
        arg0->sys->unk_43C = v1;
    }
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8)v1);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, v1), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16) py[1], (s16) py[0]);
    ov14_021F2A18(arg0->sys, 9, 1);
    v0 = arg0->sys;
    if (*((u8 *)&v0->unk_44B) == 0) {
        if ((v1 >= 0x25) && (v1 <= 0x2A)) {
            ov14_021F29E4(v0, 9, 0xE);
            return;
        }
        ov14_021F29E4(v0, 9, 8);
    }
}
