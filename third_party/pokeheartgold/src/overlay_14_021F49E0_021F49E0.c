#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

void ov14_021F49E0(struct UnkStruct_ov14_boxapp *arg0);
struct UnkStruct_ov14_boxapp;

void ov14_021F49E0(UnkStruct_ov14_boxapp *arg0) {
    s32 var_r4;
    s32 var_r5;
    u32 var_r6;

    var_r5 = 6 * ((s32) arg0->unk25 / 6);
    var_r4 = 0xF;
    var_r6 = 0;
    var_r5 <<= 0xA;
    do {
        ov14_021F2C1C(arg0->sys, var_r4, &arg0->sys->unk_40C8[var_r5], 0x400);
        var_r4 += 1;
        var_r5 += 0x400;
        var_r6 += 1;
    } while (var_r6 < 6U);
}
