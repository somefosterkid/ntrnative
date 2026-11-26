#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

#include "unk_02019BA4.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);
s32 ov14_021F74B0(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021F74B0(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 nextInput;

    nextInput = GridInputHandler_GetNextInput(arg0->sys->unk_2C);
    v0 = GridInputHandler_HandleInput_AllowHold(arg0->sys->unk_2C);
    if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
        if (v0 == 0x24) {
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, nextInput);
            ov14_021F6B28(arg0, nextInput);
            return -1;
        }
    } else {
        if ((u32)(v0 - 0x21) <= 1U) {
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, nextInput);
            ov14_021F6B28(arg0, nextInput);
            return -1;
        }
        if ((v0 == 0x23) || (v0 == -2)) {
            return 0x25;
        }
    }
    return v0;
}
