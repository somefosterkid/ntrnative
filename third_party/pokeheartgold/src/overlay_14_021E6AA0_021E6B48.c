#include "global.h"

#include "sprite_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2F88(s32, void *, s32 *, u32);


void ov14_021E6B48(UnkStruct_ov14_boxapp *arg0, MoveEntry *arg1) {
    s16 sp0[2];
    UnkStruct_ov14_boxsys *temp_r0;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    int temp_r2;
    s32 var_r0;

    temp_r0 = arg0->sys;
    ManagedSprite_GetPositionXY(temp_r0->sprites[temp_r0->unk_4094[arg1->idx]], &arg1->x0, &arg1->y0);
    temp_r2 = arg1->arg;
    if (!(0x80 & temp_r2)) {
        ov14_021F2F88(temp_r2, &sp0[1], (s32 *)sp0, arg0->unk22);
    } else {
        ov14_021F4940(arg0->sys, 0x7F & temp_r2, &sp0[1], sp0);
    }
    if ((s32)arg1->x0 <= (s32)sp0[1]) {
        arg1->dx = 1;
        var_r0 = sp0[1] - arg1->x0;
    } else {
        arg1->dx = -1;
        var_r0 = arg1->x0 - sp0[1];
    }
    temp_r1 = var_r0 << 0x10;
    arg1->mulX = (s32)(((s32)(temp_r1 + (((u32)(temp_r1 >> 2)) >> 0x1D))) >> 3);
    if ((s32)arg1->y0 <= (s32)sp0[0]) {
        arg1->dy = 1;
        temp_r1_2 = (sp0[0] - arg1->y0) << 0x10;
        arg1->mulY = (s32)(((s32)(temp_r1_2 + (((u32)(temp_r1_2 >> 2)) >> 0x1D))) >> 3);
        return;
    }
    arg1->dy = -1;
    temp_r1_3 = (arg1->y0 - sp0[0]) << 0x10;
    arg1->mulY = (s32)(((s32)(temp_r1_3 + (((u32)(temp_r1_3 >> 2)) >> 0x1D))) >> 3);
}
