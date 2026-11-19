#include "global.h"

#include "sys_task.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E80A8();
s32 ov14_021E8514(void *);
s32 ov14_021E952C(UnkStruct_ov14_boxapp *app);
s32 ov14_021E9554(UnkStruct_ov14_boxapp *app);
s32 ov14_021E9590(UnkStruct_ov14_boxapp *app);
s32 ov14_021E95B4(UnkStruct_ov14_boxapp *app);
s32 ov14_021E9518(UnkStruct_ov14_boxapp *app);
s32 ov14_021E954C(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);
s32 ov14_021E952C(UnkStruct_ov14_boxapp *arg0) {
    if (ov14_021E80A8() == 0) {
        return 0;
    }
    ov14_021E8514(((u8 *)arg0->sys->unk_2F0));
    return 1;
}
s32 ov14_021E954C(UnkStruct_ov14_boxapp *app) {
    return ov14_021E9518(app);
}
s32 ov14_021E9554(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 v1;

    v0 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0);
    v1 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xF);
    ov14_021F47B8(arg0, 8);
    if ((v0 == 0) && (v1 == 0)) {
        return 0;
    }
    return 1;
}
s32 ov14_021E9590(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;

    v0 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0);
    ov14_021F47B8(arg0, -8);
    return v0;
}
s32 ov14_021E95B4(UnkStruct_ov14_boxapp *arg0) {
    return sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xA);
}
