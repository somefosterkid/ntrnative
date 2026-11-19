#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021E8514(void *arg0);
s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);
s32 ov14_021EA0B8(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);
s32 ov14_021EA0B8(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 v4;
    s32 v5;
    s32 v6;
    u16 v2;
    u32 v1;
    UnkStruct_ov14_boxsys *v3;

    v3 = arg0->sys;
    v2 = v3->unk_10;
    switch (v2) {
    case 0:
        v4 = sub_02019978(v3->unk_2F0, 8);
        v6 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 9);
        v0 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xA);
        if ((v4 == 0) && (v6 == 0) && (v0 == 0)) {
            v3->unk_10 = (u16)(v3->unk_10 + 1);
        }
    default:
        break;
    case 1:
        v5 = ov14_021E9F20(arg0);
        v1 = ov14_021E8514(((u8 *)arg0->sys->unk_2F0));
        if ((v5 == 0) && (v1 == 0)) {
            v3->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
