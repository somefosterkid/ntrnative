#include "global.h"

#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"

s32 ov92_02260860(void *, s32, u32, s32);

s32 ov92_02260870(void *);

void ov92_0225F338(SysTask *param0, void *param1);

void ov92_0225F338(SysTask *param0, void *param1) {
    UnkStruct_ov116_02262D08 *work = param1;
    f32 sp4;
    f32 sp0;
    f32 var_r0;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r5_3;
    u32 temp_r0;

    if ((IsPaletteFadeFinished() == 0) || (*(u8 *)((u8 *)(work->unk_3C) + 0x34) == 1)) {
        work->unk_30 = 0;
        SysTask_Destroy(param0);
        return;
    }
    temp_r0 = work->unk_00;
    switch (temp_r0) {
    case 0:
        ManagedSprite_SetAnim(work->unk_0C, 7);
        ManagedSprite_SetDrawPriority(work->unk_0C, 0U);
        ManagedSprite_SetAffineOverwriteMode(work->unk_0C, 2U);
        ManagedSprite_GetAffineScale(work->unk_0C, &sp4, &sp0);
        if (sp0 > 0.0f) {
            var_r0 = 0.5f + (4096.0f * sp0);
        } else {
            var_r0 = (4096.0f * sp0) - 0.5f;
        }
        ov92_02260860(&work->unk_10, (s32)var_r0, 0x2000U, 2);
        ManagedSprite_SetPaletteOverrideOffset(work->unk_0C, *work->unk_2C - 1);
        work->unk_00 = work->unk_00 + 1;
    case 1:
        temp_r5 = ov92_02260870(&work->unk_10);
        ManagedSprite_SetAffineScale(work->unk_0C, 1.0f, (f32) work->unk_10.unk_00 / 4096.0f);
        if (temp_r5 == 0) {
            return;
        }
        work->unk_00 = work->unk_00 + 1;
        return;
    case 2:
        ov92_02260860(&work->unk_10, 0x2000, 0x666U, 4);
        work->unk_00 = work->unk_00 + 1;
    case 3:
        temp_r5_2 = ov92_02260870(&work->unk_10);
        ManagedSprite_SetAffineScale(work->unk_0C, 1.0f, (f32) work->unk_10.unk_00 / 4096.0f);
        if (temp_r5_2 != 0) {
            work->unk_00 = work->unk_00 + 1;
            return;
        }
        return;
    case 4:
        if (work->unk_28 != *work->unk_2C) {
            work->unk_00 = 0U;
        } else {
            if (work->unk_04 == 0) {
                ov92_02260860(&work->unk_10, 0x666, 0xCCDU, 2);
            } else {
                ov92_02260860(&work->unk_10, 0xCCD, 0xCCDU >> 1, 4);
            }
            work->unk_00 = work->unk_00 + 1;
        }
        work->unk_28 = (s32) * ((s32 *)work->unk_2C);
        if (*work->unk_2C == 0) {
            work->unk_00 = 0xFFU;
            return;
        }
        break;
    case 5:
        temp_r5_3 = ov92_02260870(&work->unk_10);
        ManagedSprite_SetAffineScale(work->unk_0C, 1.0f, (f32) work->unk_10.unk_00 / 4096.0f);
        if (temp_r5_3 != 0) {
            work->unk_00 = 4U;
            work->unk_04 = (s32)(1 ^ work->unk_04);
            return;
        }
        break;
    default:
        ManagedSprite_SetAffineScale(work->unk_0C, 1.0f, 1.0f);
        ManagedSprite_SetAnim(work->unk_0C, 5);
        ManagedSprite_SetDrawPriority(work->unk_0C, 2U);
        ManagedSprite_SetAffineOverwriteMode(work->unk_0C, 0U);
        work->unk_30 = 0;
        SysTask_Destroy(param0);
        break;
    }
}
