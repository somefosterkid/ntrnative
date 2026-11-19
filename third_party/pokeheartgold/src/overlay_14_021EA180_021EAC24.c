#include "global.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7AE4(UnkStruct_ov14_boxapp *, s32, s32);
s32 ov14_021F29E4(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021EAC24(UnkStruct_ov14_boxapp *arg0);
struct UnkStruct_ov14_boxsys;

s32 ov14_021E7B98(void *);

s32 sub_02019978(void *, s32);

s32 ov14_021EAC24(UnkStruct_ov14_boxapp *arg0) {
    u16 v0;
    UnkStruct_ov14_boxsys *v1;

    v1 = arg0->sys;
    sub_02019978(v1->unk_2F0, 0xF);
    v0 = v1->unk_10;
    switch (v0) {
    case 0:
        ov14_021F29E4(arg0->sys, 9, 9);
        ov14_021E7AE4(arg0, arg0->sys->unk_88CA, 2);
        v1->unk_10 = 1U;
    default:
        break;
    case 1:
        if (ov14_021E7B98(arg0) == 0) {
            v1->unk_10 = 2U;
        }
        break;
    case 2:
        ov14_021F391C(arg0->sys, 1);
        ov14_021F29E4(arg0->sys, 0xB, 2);
        v1->unk_10 = 3U;
        break;
    case 3:
        if (ov14_021F2A04(arg0->sys, 0xB) == 0) {
            ov14_021F391C(arg0->sys, 0);
            ov14_021F2A18(arg0->sys, 0xB, 0);
            v1->unk_10 = 4U;
        }
        break;
    case 4:
        ov14_021F29E4(arg0->sys, 9, 8);
        v1->unk_10 = 0U;
        return 0;
    }
    return 1;
}
