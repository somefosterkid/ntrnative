#include "global.h"
#include "overlay_14.h"

#include "sys_task.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F6654(void *, s32);
s32 ov14_021E884C(UnkStruct_ov14_boxsys *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021F5EB4(void *, s32);

s32 ov14_021F2778(void *arg0) {
    ov14_021F6654(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0x27);
    ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 0);
    if (((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 == 3) {
        ov14_021F3488(arg0, 0x81, 0);
    } else {
        ov14_021F3488(arg0, 1, 0);
    }
    ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_43C = (s32)((s32) ((UnkStruct_ov14_boxapp *)arg0)->unk25 % 6);
    ((UnkStruct_ov14_boxapp *)arg0)->unk2C = 9;
    return 0x43;
}

s32 ov14_021F27CC(UnkStruct_ov14_boxapp *arg0) {
    return ov14_021F13B0(arg0);
}

s32 ov14_021EA130(UnkStruct_ov14_boxapp *app);

void ov14_021F27D4(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021E8328(arg0->sys->unk_2F0);
    ov14_021E884C(arg0->sys);
    ov14_021F40E8(arg0, 0);
    ov14_021F0234(arg0, ov14_021EA130, 0x59);
}

void ov14_021F2810(void *arg0) {
    ov14_021F1414(arg0);
}

void ov14_021F2818(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F5EB4(arg0, 0);
    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021E8328(arg0->sys->unk_2F0);
    ov14_021E884C(arg0->sys);
    ov14_021F40E8(arg0, 0);
    ov14_021F0234(arg0, ov14_021EA130, 0x70);
}
