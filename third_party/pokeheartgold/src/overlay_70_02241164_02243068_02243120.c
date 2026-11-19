#include "global.h"
#include "overlay_70_gtsapplication_state.h"

#include "error_handling.h"
#include "sprite.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02241DB4(UnkStruct_ov70_02242E58 *, s32, s32, s32);

extern s32 ov70_02245F5C;

s32 ov70_02243120(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov70_02243120(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 temp_r1;
    s32 var_r0;

    ov70_02241DB4(arg0, 4, arg2, arg3);
    Sprite_SetDrawFlag(arg0->unk_C, 0);
    temp_r1 = arg0->unk_3C;
    if ((s32)temp_r1 >= 0) {
        if ((s32)temp_r1 > 8) {
            GF_AssertFail();
        }
        if (*(s32 *)(&ov70_02245F5C + arg0->unk_3C * 2) == 1) {
            arg0->unk_3E = 0;
            var_r0 = 0xC;
        } else {
            var_r0 = 8;
        }
    } else {
        return -2;
    }
    arg0->unk_4C = var_r0;
    return -1;
}
