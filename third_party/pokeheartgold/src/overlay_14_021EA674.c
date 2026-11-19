#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E9434(void *);
s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(void *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021EA674(UnkStruct_ov14_boxapp *app);
s32 ov14_021E7AD4(void *, u16, u32, s32);
s32 ov14_021F2A04(void *, s32);
s32 ov14_021EA728(UnkStruct_ov14_boxapp *arg0);


s32 sub_02019978(void *, s32);

s32 ov14_021E7B8C(void *);
s32 ov14_021EA674(UnkStruct_ov14_boxapp *arg0) {
    s32 v1;
    s32 v2;
    s32 v5;
    s32 v6;
    s32 v7;
    u16 v0;
    void *v3;
    void *v4;

    v4 = ((void *)arg0->sys);
    v0 = ((UnkStruct_ov14_boxsys *)v4)->unk_10;
    switch (v0) {
    case 0:
        if (((UnkStruct_ov14_boxsys *)v4)->unk_88C8 != 0) {
            ov14_021F391C(v4, 1);
            ov14_021F29E4(((void *)arg0->sys), 0xB, 2);
        }
        ((UnkStruct_ov14_boxsys *)v4)->unk_10 = (u16)(((UnkStruct_ov14_boxsys *)v4)->unk_10 + 1);
    case 1:
        v5 = ov14_021E9F20(arg0);
        v1 = ov14_021E9434(arg0);
        if ((v5 == 0) && (v1 == 0)) {
            ((UnkStruct_ov14_boxsys *)v4)->unk_10 = (u16)(((UnkStruct_ov14_boxsys *)v4)->unk_10 + 1);
        }
    default:
        break;
    case 2:
        v6 = sub_02019978(((void *)((UnkStruct_ov14_boxsys *)v4)->unk_2F0), 8);
        v7 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 9);
        v2 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xA);
        if ((v6 == 0) && (v7 == 0) && (v2 == 0)) {
            v3 = ((void *)arg0->sys);
            if (((UnkStruct_ov14_boxsys *)v3)->unk_88C8 != 0) {
                ov14_021F391C(v3, 0);
                ov14_021F2A18(((void *)arg0->sys), 0xB, 0);
            }
            ((UnkStruct_ov14_boxsys *)v4)->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}

s32 ov14_021EA728(UnkStruct_ov14_boxapp *arg0) {
    u16 v0;
    u8 v1;
    UnkStruct_ov14_boxsys *v2;

    v2 = arg0->sys;
    v0 = v2->unk_10;
    switch (v0) {
    case 0:
        if (ov14_021F2A04(v2, 0xB) != 1) {
            v1 = arg0->unk21;
            arg0->unk21 = (u8) * (u16 *)v2->unk_C;
            ov14_021E7AD4(arg0, v1, v1, 2);
            v2->unk_10 = 1U;
        }
    default:
        break;
    case 1:
        if (ov14_021E7B8C(arg0) == 0) {
            v2->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
