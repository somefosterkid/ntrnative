#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8514(void *);
s32 ov14_021E9434(UnkStruct_ov14_boxapp *app);
s32 ov14_021E9450(UnkStruct_ov14_boxapp *app);
s32 ov14_021E94A8(UnkStruct_ov14_boxapp *app);
s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);
s32 ov14_021E80A8(void *);
s32 ov14_021E9518(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);
s32 ov14_021E9434(UnkStruct_ov14_boxapp *arg0) {
    if (ov14_021E8514(((u8 *)arg0->sys->unk_2F0)) != 0) {
        return 1;
    }
    return 0;
}
s32 ov14_021E9450(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;

    v1 = ov14_021E8514(((u8 *)arg0->sys->unk_2F0));
    v2 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 8);
    v3 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 9);
    v0 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xA);
    if ((v1 == 0) && (v2 == 0) && (v3 == 0) && (v0 == 0)) {
        return 0;
    }
    return 1;
}
s32 ov14_021E94A8(UnkStruct_ov14_boxapp *arg0) {
    return sub_02019978(((u8 *)arg0->sys->unk_2F0), 2);
}
s32 ov14_021E94BC(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;

    v1 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 8);
    v2 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 9);
    v3 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xA);
    v0 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xB);
    if ((v1 == 0) && (v2 == 0) && (v3 == 0) && (v0 == 0)) {
        return 0;
    }
    return 1;
}
s32 ov14_021E9518(UnkStruct_ov14_boxapp *app) {
    if (ov14_021E80A8(app) != 0) {
        return 1;
    }
    return 0;
}
