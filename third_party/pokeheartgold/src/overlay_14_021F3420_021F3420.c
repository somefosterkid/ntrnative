#include "global.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A9C(UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F3420(void *arg0, s32 arg1, u32 arg2, u32 arg3, s32 arg4);

s32 ov14_021E6070(void *, u32, s32, void *);

void ov14_021F3420(void *arg0, s32 arg1, u32 arg2, u32 arg3, s32 arg4) {
    UnkStruct_ov14_boxsys *temp;
    s32 ret;
    u32 var_r4;

    var_r4 = arg2;
    if (arg4 == 1) {
        if (var_r4 < arg3) {
            do {
                ret = ov14_021E6070(arg0, var_r4, 6, 0);
                if (ret == 0) {
                    temp = ((UnkStruct_ov14_boxapp *)arg0)->sys;
                    ov14_021F2A9C(temp, temp->unk_4094[var_r4], 1);
                } else {
                    temp = ((UnkStruct_ov14_boxapp *)arg0)->sys;
                    ov14_021F2A9C(temp, temp->unk_4094[var_r4], 0);
                }
                var_r4 += 1;
            } while (var_r4 < arg3);
        }
    } else if (var_r4 < arg3) {
        do {
            temp = ((UnkStruct_ov14_boxapp *)arg0)->sys;
            ov14_021F2A9C(temp, temp->unk_4094[var_r4], arg1);
            var_r4 += 1;
        } while (var_r4 < arg3);
    }
}
