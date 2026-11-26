#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"

#include "overlay_14_unk_struct_ov14_boxapp.h"

#include "unk_02019BA4.h"
#include "field_system.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);

u32 ov14_021F6E8C(UnkStruct_ov14_boxapp *arg0) {
    s32 nextInput;
    UnkStruct_ov14_boxapp *new_var;
    u32 v0;
    UnkStruct_ov14_boxsys *v1;
    nextInput = GridInputHandler_GetNextInput(arg0->sys->unk_2C);
    new_var = arg0;
    v0 = (u32)GridInputHandler_HandleInput_AllowHold(new_var->sys->unk_2C);
    v1 = arg0->sys;
    if (v0 < 0x27U) {
        if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
            if ((v0 >= 0x22U) && (v0 <= 0x26U)) {
                GridInputHandler_SetNextInput(arg0->sys->unk_2C, nextInput);
                ov14_021F6B28(arg0, nextInput);
                return -1U;
            }
        } else if (v0 == 0x21) {
            return 0x26U;
        }
        return v0;
    }
    if ((ov14_021E8544(arg0->sys->unk_2F0) == 1) && ((v0 == 0x21) || (v0 == (-2U)))) {
        return 0x26U;
    }
    return v0;
}
