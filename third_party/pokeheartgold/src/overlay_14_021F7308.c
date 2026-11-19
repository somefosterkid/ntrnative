#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);

void ov14_021F7308(void *arg0, s32 nextInput) {
    if ((nextInput >= 0x25) && (nextInput <= 0x2A)) {
        ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_43C = nextInput;
    }
    if ((nextInput != 0x2B) && (nextInput != 0x2C)) {
        GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32)(u8)nextInput);
        ov14_021F6B28(arg0, nextInput);
    }
}

s32 ov14_021F7340(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;

    if (ov14_021E85E4(arg0->sys->unk_2F0) == 0) {
        GridInputHandler_ClearEnabledFlag(arg0->sys->unk_2C, 0x2DU);
    }
    GridInputHandler_ClearEnabledFlag(arg0->sys->unk_2C, 0x2BU);
    GridInputHandler_ClearEnabledFlag(arg0->sys->unk_2C, 0x2CU);
    v0 = GridInputHandler_HandleInput_AllowHold(arg0->sys->unk_2C);
    GridInputHandler_SetAllEnabled(arg0->sys->unk_2C);
    return v0;
}

s32 ov14_021F7388(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 nextInput;

    nextInput = GridInputHandler_GetNextInput(arg0->sys->unk_2C);
    v0 = GridInputHandler_HandleInput_AllowHold(arg0->sys->unk_2C);
    if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
        if (((u32)v0 >= 8U) && ((u32)v0 <= 0xCU)) {
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, nextInput);
            ov14_021F6B28(arg0, nextInput);
            return -1;
        }
    } else {
        if ((v0 == 7) || (v0 == -2)) {
            return 0xD;
        }
    }
    return v0;
}
