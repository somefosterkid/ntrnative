#include "global.h"

#include "error_handling.h"
#include "overlay_01_021EFB38.h"
#include "overlay_01_unk_struct_ov5_021de6bc.h"
#include "linear_interpolation_task.h"

s32 ov01_021EFE34(LinearInterpolationTaskS32 *, s32, s32, s32);

void ov01_021F0788(UnkStruct_ov5_021DE6BC *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, Window *arg6, s32 arg7, s32 arg8, u8 arg9) {
    if (arg0->unk_2E != 0) {
        GF_AssertFail();
    }
    ov01_021EFE34(&arg0->unk_00, arg1, arg2, arg5);
    ov01_021EFE34(&arg0->unk_14, arg3, arg4, arg5);
    arg0->unk_28 = arg6;
    arg0->unk_2C = (s8)arg7;
    arg0->unk_2D = (s8)arg8;
    arg0->unk_2F = (u8)arg9;
    arg0->unk_2E = 1U;
}
