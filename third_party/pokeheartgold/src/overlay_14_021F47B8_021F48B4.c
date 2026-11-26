#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F48B4(struct UnkStruct_ov14_boxapp *arg0);

extern s32 ov14_021F8068;

void ov14_021F48B4(UnkStruct_ov14_boxapp *arg0) {
    s16 sp0[3];
    u16 temp_r4;

    temp_r4 = (u16)((s32) arg0->unk25 % 6);
    ManagedSprite_GetPositionXY(arg0->sys->sprites[7], &sp0[2], &sp0[1]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[7], (s16) * ((u8 *)&ov14_021F8068 + temp_r4), sp0[1]);
    ManagedSprite_GetPositionXY(arg0->sys->sprites[temp_r4 + 0xF], &sp0[0], &sp0[1]);
    ManagedSprite_GetPositionXY(arg0->sys->sprites[8], &sp0[2], &sp0[1]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[8], sp0[0], sp0[1]);
    ov14_021F46F4(arg0->sys);
}
