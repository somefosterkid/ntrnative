#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E7D8C(void *arg0, s32 arg1, s32 arg2);

void ov14_021E7D7C(u8 *param_1) {
    sub_020195C0(*(UnkStruct_0201956C **)(param_1 + 0x2f0));
    return;
}

s32 sub_02019688(UnkStruct_0201956C *, s32, s32, s32, s32);

void ov14_021E7D8C(void *arg0, s32 arg1, s32 arg2) {
    sub_02019688(arg0, arg1, 0x13, arg2, 1);
}
