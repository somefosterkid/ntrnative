#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F4848(void *);
s32 ov14_021F48B4(void *);
s32 ov14_021F49E0(void *);
s32 ov14_021F57B8(void *);
void ov14_021F1004(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);
struct UnkStruct_ov14_boxsys;
struct UnkStruct_ov14_boxapp;

void ov14_021F1004(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    s32 temp_r0;
    s32 var_r1;
    var_r1 = (arg0->unk25) + (6 * arg1);
    if (var_r1 < 0) {
        var_r1 += 0x12;
    } else if (var_r1 >= 0x12) {
        var_r1 -= 0x12;
    }
    arg0->unk25 = (u8)var_r1;
    ov14_021F49E0(arg0);
    ov14_021F48B4(arg0);
    ov14_021F4848(arg0);
    ov14_021F57B8(arg0);
    temp_r0 = *((s32 *)&arg0->sys);
    if (arg1 > 0) {
        ov14_021F29E4(arg0->sys, 5, 4);
        return;
    }
    ov14_021F29E4(arg0->sys, 4, 2);
}
