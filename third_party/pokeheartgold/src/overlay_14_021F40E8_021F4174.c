#include "global.h"

#include "sprite_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_box_funcs.h"

extern u8 ov14_021F8070;

extern u8 ov14_021F8078;

void ov14_021F4174(UnkStruct_ov14_boxapp *arg0) {
    s16 sp0[2];
    void *temp_r4;
    u16 var_r5;
    u8 temp_r1;
    temp_r1 = arg0->unk21;
    if (temp_r1 != 0xFF) {
        temp_r4 = *((void **)&arg0->sys);
        ManagedSprite_GetPositionXY(*((ManagedSprite **)(((u8 *)(temp_r4 + ((*((u8 *)((u8 *)(temp_r4 + temp_r1) + 0x4094))) * 4))) + 0x2FC)), &sp0[1], &sp0[0]);
        var_r5 = 0;
        do {
            ManagedSprite_SetPositionXY(*((ManagedSprite **)(((u8 *)(temp_r4 + (var_r5 * 4))) + 0x3F0)), (s16)((sp0[1]) + (*((s8 *)((&ov14_021F8070) + var_r5)))), (s16)((sp0[0]) + (*((s8 *)((&ov14_021F8078) + var_r5)))));
            var_r5 += 1;
        } while (((u32)var_r5) < 8U);
    }
}
