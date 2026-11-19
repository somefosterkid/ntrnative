#include "global.h"
#include "unk_02013534.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A18(void *, s32, s32);
void ov14_021F43F4(void *arg0, s32 arg1);

void ov14_021F43F4(void *arg0, s32 arg1) {
    ov14_021F2A18(arg0, 0, arg1);
    ov14_021F2A18(arg0, 1, arg1);
}

void ov14_021F4410(UnkStruct_ov14_boxsys *param_1) {
    sub_020136B4(param_1->textObjs[0].textObj, -0x30, -8);
    return;
}
