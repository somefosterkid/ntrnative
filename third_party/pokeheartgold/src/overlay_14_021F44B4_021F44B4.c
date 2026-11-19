#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"
struct UnkStruct_ov14_boxsys;

void ov14_021F44B4(UnkStruct_ov14_boxsys *arg0, s32 arg1) {
    s16 sp0[2];
    u32 var_r4_2;
    u32 var_r7;

    var_r7 = 4;
    do {
        ManagedSprite_GetPositionXY(arg0->sprites[var_r7], &sp0[1], &sp0[0]);
        ManagedSprite_SetPositionXY(arg0->sprites[var_r7], sp0[1], (s16)(sp0[0] + arg1));
        var_r7 += 1;
    } while (var_r7 <= 8U);
    ov14_021F4410(arg0);
    var_r4_2 = 0;
    do {
        ManagedSprite_GetPositionXY(arg0->sprites[0x15 + var_r4_2], &sp0[1], &sp0[0]);
        ManagedSprite_SetPositionXY(arg0->sprites[0x15 + var_r4_2], sp0[1], (s16)(sp0[0] + arg1));
        var_r4_2 += 1;
    } while (var_r4_2 < 4U);
}
