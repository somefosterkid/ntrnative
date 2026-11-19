#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F3B3C(void *);

s32 ov14_021E8514(void *);
s32 ov14_021E9F20(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021EA408(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);
s32 ov14_021EA408(UnkStruct_ov14_boxapp *arg0) {
    s32 temp_r0;
    s32 temp_r6;
    s32 temp_r6_2;
    s32 temp_r7;
    u16 temp_r1;
    UnkStruct_ov14_boxsys *temp_r0_3;
    UnkStruct_ov14_boxsys *temp_r4;
    temp_r4 = arg0->sys;
    temp_r1 = temp_r4->unk_10;
    switch (temp_r1) {
    case 0:
        if ((temp_r4->unk_88C8) != 0) {
            ov14_021F391C(temp_r4, 1);
            ov14_021F29E4(arg0->sys, 0xB, 1);
            ov14_021F2A18(arg0->sys, 0xB, 1);
        }
        temp_r4->unk_10 = (u16)((temp_r4->unk_10) + 1);

    case 1:
        temp_r6 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 8);
        temp_r7 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 9);
        temp_r0 = sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xA);
        if (((temp_r6 == 0) && (temp_r7 == 0)) && (temp_r0 == 0)) {
            temp_r4->unk_10 = (u16)((temp_r4->unk_10) + 1);
        }
        break;

    case 2:
        temp_r6_2 = ov14_021E9F20(arg0);
        temp_r0 = ov14_021E8514(((u8 *)arg0->sys->unk_2F0));
        if ((temp_r6_2 == 0) && (temp_r0 == 0)) {
            temp_r0_3 = arg0->sys;
            if ((temp_r0_3->unk_88C8) != 0) {
                ov14_021F391C(temp_r0_3, 0);
                ov14_021F3B3C(arg0->sys);
            }
            temp_r4->unk_10 = 0U;
            return 0;
        }
        break;

    default:
        break;
    }
    return 1;
}
