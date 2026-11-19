#include "global.h"
#include "heap.h"
#include "sys_task_api.h"
#include "pokepic.h"
#include "screen_fade.h"
#include "unk_02005D10.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "overlay_59_cutscene_tasks.h"
#include "overlay_59_poffin_cutscene_mon_sprite.h"
#include "overlay_59_juice_feed_app.h"
#include "unk_02014DA0.h"

void ov59_0223C16C(SysTask *arg0, void *arg1);

void ov59_0223C138(Ov59JuiceFeedApp *arg0);

void ov59_0223C138(Ov59JuiceFeedApp *arg0) {
    Ov59FadeTask *task;

    task = Heap_Alloc(*(enum HeapID *)arg0, sizeof(Ov59FadeTask));
    MI_CpuFill8(task, 0U, sizeof(Ov59FadeTask));
    task->work = arg0;
    SysTask_CreateOnMainQueue(ov59_0223C16C, task, 0U);
    arg0->taskCount = (u8) (arg0->taskCount + 1);
}

s32 ov59_0223C3C0(void *, s32);

s32 ov59_0223C3DC(void *);

void ov59_0223C16C(SysTask *arg0, void *arg1) {
    Ov59FadeTask *task = arg1;
    u8 temp_r0;
    u8 temp_r1;
    u8 temp_r1_2;
    Ov59JuiceFeedApp *temp_r5;

    temp_r0 = task->step;
    temp_r5 = task->work;
    switch (temp_r0) {
    case 0:
        ov59_0223C3C0(temp_r5->monSprite, 2);
        PlaySE(0x923U);
        task->timer = 0x14U;
        task->step = (u8) (task->step + 1);
        return;
    case 1:
        temp_r1 = task->timer;
        task->timer = (u8) (temp_r1 - 1);
        if (temp_r1 == 0) {
            Pokepic_StartPaletteFade(temp_r5->monSprite->unk_20, 0, 0x10, 4, 0x7FFF);
            ov59_0223C3C0(temp_r5->monSprite, 1);
            ov59_0223C3C0(temp_r5->monSprite, 0);
            task->timer = 0x19U;
            task->step = (u8) (task->step + 1);
            return;
        }
        return;
    case 2:
        temp_r1_2 = task->timer;
        task->timer = (u8) (temp_r1_2 - 1);
        if (temp_r1_2 == 0) {
            PlaySE(0x924U);
            task->step = (u8) (task->step + 1);
            return;
        }
        break;
    case 3:
        if ((Pokepic_ResumePaletteFade(temp_r5->monSprite->unk_20) == 0) && (ov59_0223C3DC(temp_r5->monSprite) == 0)) {
            sub_020154B8(temp_r5->monSprite->unk_24);
            task->step = (u8) (task->step + 1);
            return;
        }
        break;
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 6, 6, temp_r5->heapId);
        Pokepic_StartPaletteFade(temp_r5->monSprite->unk_20, 0x10, 0, 4, 0x7FFF);
        task->step = (u8) (task->step + 1);
        return;
    default:
        if ((IsPaletteFadeFinished() != 0) && (Pokepic_ResumePaletteFade(temp_r5->monSprite->unk_20) == 0)) {
            temp_r5->taskCount = (u8) (temp_r5->taskCount - 1);
            Heap_Free(task);
            SysTask_Destroy(arg0);
        }
        break;
    }
}

void ov59_0223C3F0(void);

s32 ov59_0223C474();

s32 ov59_0223C584(void *);

s32 ov59_0223C5C4(void *, enum HeapID);

void *ov59_0223C298(enum HeapID arg0);

void *ov59_0223C298(enum HeapID arg0) {
    void *temp_r0;

    temp_r0 = Heap_Alloc(arg0, sizeof(PoffinCutsceneMonSprite));
    MI_CpuFill8(temp_r0, 0U, sizeof(PoffinCutsceneMonSprite));
    *(enum HeapID *)temp_r0 = arg0;
    *(GF3DVramMan **)((u8 *)temp_r0 + 4) = GF_3DVramMan_Create(arg0, 0, 2, 0, 1, ov59_0223C3F0);
    ov59_0223C474();
    ov59_0223C5C4(temp_r0, arg0);
    ov59_0223C584(temp_r0);
    *((PokepicManager **)&((PoffinCutsceneMonSprite *)temp_r0)->spriteMan) = PokepicManager_Create(arg0);
    GfGfx_EngineATogglePlanes(1U, 1U);
    reg_G2_BG0CNT = 1 | (reg_G2_BG0CNT & ~3);
    return temp_r0;
}
