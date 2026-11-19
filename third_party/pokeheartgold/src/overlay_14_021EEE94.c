#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A04(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F2A18(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021EEE94(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021EEE94(UnkStruct_ov14_boxapp *arg0) {
    if (ov14_021F2A04(arg0->sys, 0xB) == 1) {
        return 0x7B;
    }
    ov14_021F391C(arg0->sys, 0);
    ov14_021F2A18(arg0->sys, 0xB, 0);
    ov14_021F2A18(arg0->sys, 9, 1);
    if ((u32) arg0->unk21 >= 0x1EU) {
        return 0x8B;
    }
    return 0x75;
}
