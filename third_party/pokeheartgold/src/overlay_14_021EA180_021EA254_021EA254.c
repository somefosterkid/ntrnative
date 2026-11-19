#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EA254(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);

s32 sub_02019978(void *, s32);
s32 ov14_021EA254(UnkStruct_ov14_boxapp *arg0) {
    s16 sp0[2];
    s32 temp_r6;
    u16 temp_r0;
    UnkStruct_ov14_boxsys *temp_r0_2;
    UnkStruct_ov14_boxsys *temp_r4;

    temp_r4 = arg0->sys;
    sub_02019978(temp_r4->unk_2F0, 0xF);
    temp_r0 = temp_r4->unk_10;
    switch (temp_r0) {
    case 0:
        ov14_021F29E4(arg0->sys, 9, 9);
        temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
    case 1:
        if (temp_r4->unk_12 == 4) {
            temp_r4->unk_12 = 0U;
            temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
        } else {
            ManagedSprite_GetPositionXY(arg0->sys->sprites[9], &sp0[1], &sp0[0]);
            ManagedSprite_SetPositionXY(arg0->sys->sprites[9], sp0[1], (s16)(sp0[0] + 2));
            temp_r4->unk_12 = (u16)(temp_r4->unk_12 + 1);
        }
        break;
    case 2:
        ov14_021F29E4(arg0->sys, 9, 0xA);
        temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
    case 3:
        if (temp_r4->unk_12 == 4) {
            temp_r4->unk_12 = 0U;
            temp_r4->unk_10 = 0U;
            return 0;
        }
        ManagedSprite_GetPositionXY(arg0->sys->sprites[9], &sp0[1], &sp0[0]);
        ManagedSprite_SetPositionXY(arg0->sys->sprites[9], sp0[1], (s16)(sp0[0] - 2));
        temp_r0_2 = arg0->sys;
        temp_r6 = temp_r0_2->unk_4094[arg0->unk21];
        ManagedSprite_GetPositionXY(temp_r0_2->sprites[temp_r6], &sp0[1], &sp0[0]);
        ManagedSprite_SetPositionXY(arg0->sys->sprites[temp_r6], sp0[1], (s16)(sp0[0] - 2));
        temp_r4->unk_12 = (u16)(temp_r4->unk_12 + 1);
        break;
    }
    return 1;
}
