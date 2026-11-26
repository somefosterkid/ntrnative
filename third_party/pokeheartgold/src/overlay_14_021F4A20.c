#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

void ov14_021F4A20(struct UnkStruct_ov14_boxapp *arg0, u32 arg1);
struct UnkStruct_ov14_boxapp;

void ov14_021F4A20(UnkStruct_ov14_boxapp *arg0, u32 arg1) {
    if (((s32) arg0->unk25 / 6) == (arg1 / 6U)) {
        ov14_021F2C1C(arg0->sys, (arg1 % 6U) + 0xF, &arg0->sys->unk_40C8[arg1 << 0xA], 0x400);
    }
}
