#include "global.h"

#include "sprite_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

s32 ov14_021F2F88(s32, void *, s32 *, u32);

s32 ov14_021F4940(void *, s32, void *, s32 *);

void ov14_021E6B48(struct UnkStruct_ov14_boxapp *arg0, void *arg1);

void ov14_021E6B48(UnkStruct_ov14_boxapp *arg0, void *arg1) {
    s16 sp0[2];
    s32 temp_r0;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    int temp_r2;
    s32 var_r0;
    temp_r0 = *((s32 *)&arg0->sys);
    ManagedSprite_GetPositionXY(*((ManagedSprite **)(((u8 *)(temp_r0 + ((*((u8 *)(((u8 *)(temp_r0 + (*((s32 *)(((u8 *)arg1) + 4))))) + 0x4094))) * 4))) + 0x2FC)), arg1 + 0x1C, arg1 + 0x1E);
    temp_r2 = *((s32 *)(((u8 *)arg1) + 8));
    if (!(0x80 & temp_r2)) {
        ov14_021F2F88(temp_r2, &sp0[1], (s32 *)sp0, arg0->unk22);
    } else {
        ov14_021F4940(*((void **)&arg0->sys), 0x7F & temp_r2, &sp0[1], (s32 *)sp0);
    }
    if (((s32)(*((s16 *)(((u8 *)arg1) + 0x1C)))) <= ((s32)(*((s16 *)(((u8 *)(&sp0)) + 2))))) {
        *((s16 *)(((u8 *)arg1) + 0x18)) = 1;
        var_r0 = (*((s16 *)(((u8 *)(&sp0)) + 2))) - (*((s16 *)(((u8 *)arg1) + 0x1C)));
    } else {
        *((s16 *)(((u8 *)arg1) + 0x18)) = -1;
        var_r0 = (*((s16 *)(((u8 *)arg1) + 0x1C))) - (*((s16 *)(((u8 *)(&sp0)) + 2)));
    }
    temp_r1 = var_r0 << 0x10;
    *((s32 *)(((u8 *)arg1) + 0x10)) = (s32)(((s32)(temp_r1 + (((u32)(temp_r1 >> 2)) >> 0x1D))) >> 3);
    if (((s32)(*((s16 *)(((u8 *)arg1) + 0x1E)))) <= ((s32)(*((s16 *)(&sp0))))) {
        *((s16 *)(((u8 *)arg1) + 0x1A)) = 1;
        temp_r1_2 = ((*((s16 *)(&sp0))) - (*((s16 *)(((u8 *)arg1) + 0x1E)))) << 0x10;
        *((s32 *)(((u8 *)arg1) + 0x14)) = (s32)(((s32)(temp_r1_2 + (((u32)(temp_r1_2 >> 2)) >> 0x1D))) >> 3);
        return;
    }
    *((s16 *)(((u8 *)arg1) + 0x1A)) = -1;
    temp_r1_3 = ((*((s16 *)(((u8 *)arg1) + 0x1E))) - (*((s16 *)(&sp0)))) << 0x10;
    *((s32 *)(((u8 *)arg1) + 0x14)) = (s32)(((s32)(temp_r1_3 + (((u32)(temp_r1_3 >> 2)) >> 0x1D))) >> 3);
}
