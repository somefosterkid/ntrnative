#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EAB54(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021EAB54(UnkStruct_ov14_boxapp *arg0) {
    s16 sp0[2];
    u16 temp_r0;
    UnkStruct_ov14_boxsys *temp_r4;

    temp_r4 = arg0->sys;
    temp_r0 = temp_r4->unk_10;
    switch (temp_r0) {
    case 0:
        ov14_021F29E4(temp_r4, 9, 9);
        temp_r4->unk_10 = 1U;
    case 1:
        if (temp_r4->unk_12 == 4) {
            temp_r4->unk_12 = 0U;
            temp_r4->unk_10 = 2U;
        } else {
            ManagedSprite_GetPositionXY(arg0->sys->sprites[11], &sp0[1], &sp0[0]);
            ManagedSprite_SetPositionXY(arg0->sys->sprites[11], sp0[1], (s16)(sp0[0] + 2));
            temp_r4->unk_12 = (u16)(temp_r4->unk_12 + 1);
        }
        break;
    case 2:
        ov14_021F29E4(temp_r4, 9, 8);
        ov14_021F391C(arg0->sys, 1);
        ov14_021F29E4(arg0->sys, 0xB, 2);
        temp_r4->unk_10 = 3U;
        break;
    case 3:
        if (ov14_021F2A04(temp_r4, 0xB) == 0) {
            ov14_021F391C(arg0->sys, 0);
            ov14_021F2A18(arg0->sys, 0xB, 0);
            temp_r4->unk_10 = 4U;
        }
        break;
    case 4:
        ov14_021F29E4(temp_r4, 9, 8);
        temp_r4->unk_10 = 0U;
        return 0;
    }
    return 1;
}
