#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "heap.h"
#include "sprite_system.h"
#include "sys_task_api.h"

void ov59_02239C90(SysTask *task, void *data);

extern s32 ov59_0223C6C4;

extern s32 ov59_0223C6C6;

void ov59_0223919C(Ov59ApricornApp *arg0, s32 arg1, s32 arg2);

void ov59_0223919C(Ov59ApricornApp *arg0, s32 arg1, s32 arg2) {
    s16 x;
    s16 y;
    f32 sp4;
    f32 sp0;
    Sprite *var_r0;
    f32 var_r0_4;
    f32 var_r0_5;
    f32 var_r0_6;
    f32 var_r0_7;
    s16 temp_r0_2;
    s16 temp_r0_3;
    s16 temp_r5;
    s16 temp_r6;
    s32 var_r0_2;
    s32 var_r0_3;
    u32 var_r1;
    s32 var_r1_2;
    s32 var_r1_3;
    u8 temp_r0_4;
    u8 temp_r0_5;
    Ov59SpriteMoveTask *temp_r0;

    temp_r0 = Heap_Alloc(arg0->heapId, sizeof(Ov59SpriteMoveTask));
    MI_CpuFill8(temp_r0, 0U, sizeof(Ov59SpriteMoveTask));
    temp_r0->app = arg0;
    temp_r0->selected = (u8) arg0->selected;
    if (arg1 == 0) {
        var_r0 = arg0->sprites[arg0->selected + 2];
        temp_r0->sprite = var_r0;
    } else {
        var_r0 = arg0->sprites[1];
        temp_r0->sprite = var_r0;
    }
    Sprite_GetPositionXY(temp_r0->sprite, &x, &y);
    if (arg2 == 0) {
        temp_r0->targetX = (s16)((arg1 * 4) + 0xC0);
        var_r0_2 = (-0xA * arg1) + 0x60;
    } else {
        temp_r0->targetX = (s16)(*(u16 *)(&ov59_0223C6C4 + temp_r0->selected) + (arg1 * 4));
        var_r0_2 = *(u16 *)(&ov59_0223C6C6 + temp_r0->selected) + (-0xA * arg1);
    }
    temp_r0->targetY = var_r0_2;
    temp_r5 = temp_r0->targetX - x;
    temp_r6 = temp_r0->targetY - y;
    if ((s32)temp_r6 < 0) {
        var_r0_3 = 0 - temp_r6;
    } else {
        var_r0_3 = temp_r6;
    }
    if ((s32)temp_r5 < 0) {
        var_r1 = 0 - temp_r5;
    } else {
        var_r1 = temp_r5;
    }
    if ((s32)var_r1 >= (s32)var_r0_3) {
        if ((s32)temp_r5 < 0) {
            var_r1_2 = 0 - temp_r5;
        } else {
            var_r1_2 = temp_r5;
        }
        temp_r0->numSteps = (u8)((s32)(var_r1_2 + ((u32)(var_r1_2 >> 2) >> 0x1D)) >> 3);
    } else {
        if ((s32)temp_r6 < 0) {
            var_r1_3 = 0 - temp_r6;
        } else {
            var_r1_3 = temp_r6;
        }
        temp_r0->numSteps = (u8)((s32)(var_r1_3 + ((u32)(var_r1_3 >> 2) >> 0x1D)) >> 3);
    }
    temp_r0_2 = x;
    if ((s32)temp_r0_2 > 0) {
        var_r0_4 = 0.5f + (f32)(temp_r0_2 << 0xC);
    } else {
        var_r0_4 = (f32)(temp_r0_2 << 0xC) - 0.5f;
    }
    temp_r0->x = (s32)var_r0_4;
    temp_r0_3 = y;
    if ((s32)temp_r0_3 > 0) {
        var_r0_5 = 0.5f + (f32)(temp_r0_3 << 0xC);
    } else {
        var_r0_5 = (f32)(temp_r0_3 << 0xC) - 0.5f;
    }
    temp_r0->y = (s32)var_r0_5;
    temp_r0->step = 0;
    temp_r0_4 = temp_r0->numSteps;
    if (temp_r0_4 != 0) {
        sp4 = 0.5f + (f32)(temp_r0_4 << 0xC);
    } else {
        sp4 = (f32)(temp_r0_4 << 0xC) - 0.5f;
    }
    if ((s32)temp_r5 > 0) {
        var_r0_6 = 0.5f + (f32)(temp_r5 << 0xC);
    } else {
        var_r0_6 = (f32)(temp_r5 << 0xC) - 0.5f;
    }
    temp_r0->dx = FX_Div((s32)var_r0_6, (s32)sp4);
    temp_r0_5 = temp_r0->numSteps;
    if (temp_r0_5 != 0) {
        sp0 = 0.5f + (f32)(temp_r0_5 << 0xC);
    } else {
        sp0 = (f32)(temp_r0_5 << 0xC) - 0.5f;
    }
    if ((s32)temp_r6 > 0) {
        var_r0_7 = 0.5f + (f32)(temp_r6 << 0xC);
    } else {
        var_r0_7 = (f32)(temp_r6 << 0xC) - 0.5f;
    }
    temp_r0->dy = FX_Div((s32)var_r0_7, (s32)sp0);
    SysTask_CreateOnMainQueue(ov59_02239C90, temp_r0, 0U);
    arg0->animCount = (u8)(arg0->animCount + 1);
}
