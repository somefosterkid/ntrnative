#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E7D8C(void *arg0, s32 arg1, s32 arg2);

void ov14_021E7D7C(UnkStruct_ov14_boxsys *param_1) {
    sub_020195C0(param_1->unk_2F0);
    return;
}

s32 sub_02019688(UnkStruct_0201956C *, s32, s32, s32, s32);

void ov14_021E7D8C(void *arg0, s32 arg1, s32 arg2) {
    sub_02019688(arg0, arg1, 0x13, arg2, 1);
}
