#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F6844(void *, s32, s32);
s32 ov14_021F6AC0(void *, s32, s32);
struct UnkStruct_ov14_boxapp;

s32 ov14_021ED7DC(UnkStruct_ov14_boxapp *arg0) {
    u8 temp_r3;
    UnkStruct_ov14_boxsys *temp_r1;

    temp_r1 = arg0->sys;
    temp_r3 = *(u8 *)((u8 *)temp_r1 + 0x44D);
    *(s32 *)((u8 *)temp_r1 + 0x43C) = (s32)(temp_r3 % 4);
    ov14_021F6AC0(arg0, 0xA, arg0->sys->unk_43C);
    ov14_021F29E4(arg0->sys, 9, 0xE);
    ov14_021F6844(arg0, 1, 0x27);
    return 0x42;
}
