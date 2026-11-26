#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"
struct UnkStruct_ov14_boxsys;

void ov14_021F47B8(void *arg0, s32 arg1) {
    s16 sp0[2];
    s32 var_r4;
    s32 var_r4_2;
    u32 var_r6;
    u32 var_r6_2;

    var_r6 = 4;
    var_r4 = 0x10;
    do {
        ManagedSprite_GetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[var_r6], &sp0[1], &sp0[0]);
        ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[var_r6], sp0[1], (s16)(sp0[0] + arg1));
        var_r6 += 1;
        var_r4 += 4;
    } while (var_r6 <= 8U);
    ov14_021F46F4(((UnkStruct_ov14_boxapp *)arg0)->sys);
    var_r6_2 = 0;
    var_r4_2 = 0;
    do {
        ManagedSprite_GetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[0xF + var_r6_2], &sp0[1], &sp0[0]);
        ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[0xF + var_r6_2], sp0[1], (s16)(sp0[0] + arg1));
        var_r6_2 += 1;
        var_r4_2 += 4;
    } while (var_r6_2 < 6U);
}
