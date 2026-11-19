#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8514(void *);
s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);

s32 ov14_021EA068(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021EA068(UnkStruct_ov14_boxapp *arg0) {
    s32 v1;
    s32 v2;
    u16 v0;
    UnkStruct_ov14_boxsys *v3;

    v3 = arg0->sys;
    v0 = v3->unk_10;
    switch (v0) {
    case 0:
        if (sub_02019978(v3->unk_2F0, 0xA) == 0) {
            v3->unk_10 = (u16)(v3->unk_10 + 1);
        }
    default:
        break;
    case 1:
        v2 = ov14_021E8514(v3->unk_2F0);
        v1 = ov14_021E9F20(arg0);
        if ((v2 == 0) && (v1 == 0)) {
            v3->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
