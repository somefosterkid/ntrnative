#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F34DC(void *, s32);

void ov14_021F3210(void *arg0, s32 arg1) {
    s16 sp14[2];
    s32 sp10;
    s16 spC;
    s32 sp8;
    s32 sp4;
    s32 sp0;
    UnkStruct_ov14_boxsys *new_var2;
    UnkStruct_ov14_boxsys *temp_r0;
    void *new_var;
    UnkStruct_ov14_boxsys *temp_r0_2;
    s32 temp_r1_2;
    UnkStruct_ov14_boxsys *temp_r1_3;
    u16 var_r4;
    u8 temp_r1;
    void *new_var3;
    sp0 = arg1;
    if (arg1 >= 0) {
        sp8 = 0xB0;
        spC = sp8;
        spC = sp8 - 0xB8;
    } else {
        sp8 = -8;
        spC = 0xB0;
    }
    var_r4 = 0;
    do {
        temp_r0 = ((UnkStruct_ov14_boxapp *)arg0)->sys;
        temp_r1 = temp_r0->unk_4094[var_r4];
        sp4 = (s32)temp_r1;
        temp_r1_2 = temp_r1;
        sp10 = temp_r1_2;
        ManagedSprite_GetPositionXY(temp_r0->sprites[temp_r1_2], &sp14[1], &sp14[0]);
        sp14[1] = (s16)((sp14[1]) + sp0);
        if ((sp14[1]) == sp8) {
            new_var = arg0;
            sp14[1] = spC;
            temp_r0_2 = ((UnkStruct_ov14_boxapp *)arg0)->sys;
            ov14_021F2E78(temp_r0_2, temp_r0_2->unk_458[var_r4], sp4, temp_r0_2->unk_4076[var_r4]);
            temp_r1_3 = ((UnkStruct_ov14_boxapp *)arg0)->sys;
            new_var2 = temp_r1_3;
            ManagedSprite_SetDrawFlag(new_var2->sprites[sp10], (s32)temp_r1_3->unk_4058[var_r4]);
            if ((*((s32 *)(((u8 *)(new_var3 = *((void **)arg0))) + 8))) == 3) {
                ov14_021F34DC(new_var, var_r4);
            }
        }
        ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[sp10], sp14[1], sp14[0]);
        var_r4 += 1;
    } while (((u32)var_r4) < 0x1EU);
}
