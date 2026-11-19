#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);

u32 ov14_021F6BC0(UnkStruct_ov14_boxapp *arg0) {
    s32 nextInput;
    u32 v0;
    void *v1;
    nextInput = GridInputHandler_GetNextInput(arg0->sys->unk_2C);
    v0 = (u32)GridInputHandler_HandleInput_AllowHold(arg0->sys->unk_2C);
    v1 = *((void * *)&arg0->sys);
    if (v0 < 0xCU) {
        if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
            if ((v0 >= 7U) && (v0 <= 0xBU)) {
                GridInputHandler_SetNextInput(arg0->sys->unk_2C, nextInput);
                ov14_021F6B28(arg0, nextInput);
                return -1U;
            }
        } else if (v0 == 6) {
            return 0xBU;
        }
        return v0;
    }
    if ((ov14_021E8544(arg0->sys->unk_2F0) == 1) && ((v0 == 6) || (v0 == (-2U)))) {
        return 0xBU;
    }
    return v0;
}
