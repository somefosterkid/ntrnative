#include "global.h"
#include "heap.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "math_util.h"
#include "unk_02005D10.h"
#include "overlay_59_cutscene_tasks.h"
#include "overlay_59_juice_feed_app.h"

void ov59_0223BFC8(SysTask *task, void *data);

void ov59_0223BE70(Ov59JuiceFeedApp *arg0, s32 arg1, s8 arg2) {
    Sprite *temp_r0_5;
    f32 var_r0;
    f32 var_r0_2;
    u8 temp_r0_2;
    u8 temp_r0_3;
    u8 temp_r0_4;
    u8 temp_r7;
    u8 var_r7;
    Ov59ScaleTask *task;

    task = Heap_Alloc(arg0->heapId, sizeof(Ov59ScaleTask));
    MI_CpuFill8(task, 0U, sizeof(Ov59ScaleTask));
    task->work = arg0;
    task->isOut = arg2;
    task->timer = 0x1EU;
    temp_r0_2 = 7 & ((s32) arg0->stars.stars >> (arg1 * 3));
    if (arg2 == 0) {
        var_r7 = temp_r0_2 + arg0->deltaStep + 1;
        task->scale.x = 0x19A;
        task->scale.y = 0x19A;
        task->scale.z = 0;
        temp_r0_3 = task->timer;
        if (temp_r0_3 != 0) {
            var_r0 = 0.5f + (f32) (temp_r0_3 << 0xC);
        } else {
            var_r0 = (f32) (temp_r0_3 << 0xC) - 0.5f;
        }
        task->scaleStep = FX_Div(0x1000, (s32) var_r0);
        task->rotationStep = 0xC;
        task->rotation = 0;
    } else {
        var_r7 = temp_r0_2 - arg0->deltaStep;
        task->scale.x = 0x119A;
        task->scale.y = 0x119A;
        task->scale.z = 0x1000;
        temp_r0_4 = task->timer;
        if (temp_r0_4 != 0) {
            var_r0_2 = 0.5f + (f32) (temp_r0_4 << 0xC);
        } else {
            var_r0_2 = (f32) (temp_r0_4 << 0xC) - 0.5f;
        }
        task->scaleStep = (s32) (0 - FX_Div(0x1000, (s32) var_r0_2));
        task->rotationStep = (s16) (task->rotationStep - 0xC);
        task->rotation = 0x168;
    }
    temp_r7 = var_r7 + (arg1 * 5);
    temp_r0_5 = arg0->sprites[temp_r7 + 8];
    task->sprite = temp_r0_5;
    Sprite_SetAffineScale(temp_r0_5, &task->scale);
    ov59_0223BE44(arg0, temp_r7, (u8) (arg0->stars.color[arg1] + 4), 1);
    SysTask_CreateOnMainQueue(ov59_0223BFC8, task, 0U);
    arg0->taskCount = (u8) (arg0->taskCount + 1);
    arg0->deltaStep = (u8) (arg0->deltaStep + 1);
}

void ov59_0223BFC8(SysTask *arg0, void *arg1) {
    Ov59ScaleTask *task = arg1;
    Ov59JuiceFeedApp *temp_r4;

    temp_r4 = task->work;
    task->rotation = (u16) (task->rotation + task->rotationStep);
    task->scale.x = (s32) (task->scale.x + task->scaleStep);
    task->scale.y = (s32) (task->scale.y + task->scaleStep);
    task->timer = (u8) (task->timer - 1);
    if (task->timer != 0) {
        Sprite_SetAffineZRotation(task->sprite, (u32) GF_DegreeToSinCosIdx(task->rotation));
        Sprite_SetAffineScale(task->sprite, &task->scale);
        return;
    }
    Sprite_SetAffineZRotation(task->sprite, 0U);
    task->scale.x = 0x1000;
    task->scale.y = 0x1000;
    task->scale.z = 0x1000;
    Sprite_SetAffineScale(task->sprite, &task->scale);
    if (task->isOut != 0) {
        Sprite_SetDrawFlag(task->sprite, 0);
    }
    temp_r4->taskCount = (u8) (temp_r4->taskCount - 1);
    Heap_Free(task);
    SysTask_Destroy(arg0);
}

void ov59_0223C09C(SysTask *task, void *data);

void ov59_0223C054(Ov59JuiceFeedApp *arg0) {
    Ov59ShakeTask *task;

    task = Heap_Alloc(arg0->heapId, sizeof(Ov59ShakeTask));
    MI_CpuFill8(task, 0U, sizeof(Ov59ShakeTask));
    task->work = arg0;
    task->counter = 0x1E;
    task->amount = 0xC;
    PlaySE(0x922U);
    SysTask_CreateOnMainQueue(ov59_0223C09C, task, 0U);
    arg0->taskCount = (u8) (arg0->taskCount + 1);
}
