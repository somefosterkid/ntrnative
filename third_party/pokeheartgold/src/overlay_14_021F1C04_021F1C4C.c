#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6AA0(void *, s32, s32);
s32 ov14_021E88F8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F1C04(void *);
void ov14_021F1C4C(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);

s32 ov14_021EAB54(UnkStruct_ov14_boxapp *app);

void ov14_021F1C4C(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    s16 sp0[2];

    arg0->sys->unk_44C = arg1;
    arg0->sys->unk_44B = 0;
    ManagedSprite_GetPositionXY(arg0->sys->sprites[9], &sp0[1], &sp0[0]);
    ManagedSprite_SetPositionXY(arg0->sys->sprites[11], sp0[1], (s16)(sp0[0] + 8));
    if (arg0->sys->unk_44C == 0xFF) {
        ov14_021F1C04(arg0);
        return;
    }
    if (ov14_021E6AA0(arg0, arg0->unk21, arg1) == 0) {
        ov14_021F1C04(arg0);
        return;
    }
    ov14_021E88F8(*(void **)&arg0->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021EAB54, 0x8A);
}
