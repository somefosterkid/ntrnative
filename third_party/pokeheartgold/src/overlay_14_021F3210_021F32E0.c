#include "global.h"

#include "sprite_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F32E0(struct UnkStruct_ov14_boxapp *arg0);


extern u8 ov14_021F808C;

void ov14_021F32E0(UnkStruct_ov14_boxapp *arg0) {
    s8 sp4[2];
    s16 temp_r5;
    s16 temp_r7;
    s32 temp_r1;
    void *sp0;
    u8 temp_r6;
    u16 var_r4;
    sp0 = arg0;
    sub_02019B1C(arg0->sys->unk_2F0, 1, &sp4[1], &sp4[0]);
    var_r4 = 0;
    temp_r7 = sp4[0] * 8;
    temp_r5 = sp4[1] * 8;
    do {
        temp_r1 = var_r4 * 2;
        temp_r6 = *((s32 *)&((UnkStruct_ov14_boxapp *)sp0)->sys);
        ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)sp0)->sys->sprites[((UnkStruct_ov14_boxapp *)sp0)->sys->unk_4094[0x1E + var_r4]], (s16)(temp_r5 + (*((u8 *)((&ov14_021F808C) + temp_r1)))), (s16)(temp_r7 + (*((u8 *)(((u8 *)((&ov14_021F808C) + temp_r1)) + 1)))));
        var_r4 += 1;
    } while (((u32)var_r4) < 6U);
}
