#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A60(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A74(UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F3190(UnkStruct_ov14_boxsys *arg0, u32 arg1, s32 arg2);

void ov14_021F3190(UnkStruct_ov14_boxsys *arg0, u32 arg1, s32 arg2) {
    u8 v0;

    v0 = arg0->unk_4094[arg1];
    if (arg2 == 0) {
        ov14_021F2A60(arg0, v0, 0);
        ov14_021F2A74(arg0, v0, 0x14);
        return;
    }
    if (arg1 < 0x1EU) {
        ov14_021F2A60(arg0, v0, 3);
    } else {
        ov14_021F2A60(arg0, v0, 1);
    }
    ov14_021F2A74(arg0, v0, 0x74 - (arg1 * 2));
}

void ov14_021F31E0(UnkStruct_ov14_boxsys *arg0, s32 arg1, u32 arg2) {
    u8 v0;

    v0 = arg0->unk_4094[arg1];
    if (arg2 < 0x1EU) {
        ov14_021F2A60(arg0, v0, 3);
    } else {
        ov14_021F2A60(arg0, v0, 1);
    }
    ov14_021F2A74(arg0, v0, 0x14);
}
