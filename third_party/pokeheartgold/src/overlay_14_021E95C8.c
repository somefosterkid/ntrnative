#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E9434(void *arg0);
s32 ov14_021E95C8(UnkStruct_ov14_boxapp *app);
s32 ov14_021E9604(UnkStruct_ov14_boxapp *app);
s32 ov14_021E9618(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);
s32 ov14_021E95C8(UnkStruct_ov14_boxapp *arg0) {
    u16 v0;
    UnkStruct_ov14_boxsys *v1;

    v1 = arg0->sys;
    v0 = v1->unk_10;
    switch (v0) {
    case 0:
        if (ov14_021E9434(arg0) == 0) {
            v1->unk_10 = (u16)(v1->unk_10 + 1);
        }
    default:
        break;
    case 1:
        if (sub_02019978(((void *)v1->unk_2F0), 0xC) == 0) {
            v1->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
s32 ov14_021E9604(UnkStruct_ov14_boxapp *arg0) {
    return sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xC);
}
s32 ov14_021E9618(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 v1;
    s32 v2;

    v1 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xC);
    v2 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 6);
    v0 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 7);
    if ((v1 == 0) && (v2 == 0) && (v0 == 0)) {
        return 0;
    }
    return 1;
}
