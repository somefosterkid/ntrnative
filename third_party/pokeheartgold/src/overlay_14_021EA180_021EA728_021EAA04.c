#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021EAA04(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);
s32 ov14_021EAA04(UnkStruct_ov14_boxapp *arg0) {
    s16 sp0[2];
    u16 temp_r0;
    UnkStruct_ov14_boxsys *temp_r4;

    temp_r4 = arg0->sys;
    sub_02019978(temp_r4->unk_2F0, 0xF);
    temp_r0 = temp_r4->unk_10;
    switch ((u32)temp_r0) {
    case 0:
        ov14_021F391C(arg0->sys, 1);
        ov14_021F29E4(arg0->sys, 0xB, 1);
        ov14_021F2A18(arg0->sys, 0xB, 1);
        temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
        break;
    case 1:
        if (ov14_021F2A04(arg0->sys, 0xB) == 0) {
            ov14_021F391C(arg0->sys, 0);
            temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
        }
        break;
    case 2:
        ov14_021F29E4(arg0->sys, 9, 9);
        temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
    case 3:
        if (temp_r4->unk_12 == 4) {
            temp_r4->unk_12 = 0U;
            temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
        } else {
            ManagedSprite_GetPositionXY(arg0->sys->sprites[9], &sp0[1], &sp0[0]);
            ManagedSprite_SetPositionXY(arg0->sys->sprites[9], sp0[1], (s16)(sp0[0] + 2));
            temp_r4->unk_12 = (u16)(temp_r4->unk_12 + 1);
        }
        break;
    case 4:
        ov14_021F29E4(arg0->sys, 9, 0xA);
        temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
    case 5:
        if (temp_r4->unk_12 == 4) {
            temp_r4->unk_12 = 0U;
            temp_r4->unk_10 = 0U;
            return 0;
        }
        ManagedSprite_GetPositionXY(arg0->sys->sprites[9], &sp0[1], &sp0[0]);
        ManagedSprite_SetPositionXY(arg0->sys->sprites[9], sp0[1], (s16)(sp0[0] - 2));
        ManagedSprite_GetPositionXY(arg0->sys->sprites[11], &sp0[1], &sp0[0]);
        ManagedSprite_SetPositionXY(arg0->sys->sprites[11], sp0[1], (s16)(sp0[0] - 2));
        temp_r4->unk_12 = (u16)(temp_r4->unk_12 + 1);
        break;
    }
    return 1;
}
