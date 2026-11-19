#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_120.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200FA24.h"

void ov120_0225F714(SysTask *task, EncounterEffect *effect) {
    Ov120StepWork *work;
    u16 plttData;
    BOOL finished;

    work = effect->param;
    switch (effect->state) {
    case 0:
        work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov120StepWork));
        effect->param = work;
        memset(work, 0, sizeof(Ov120StepWork));
        work = effect->param;
        work->window = AllocWindows(HEAP_ID_FIELD1, 1);
        AddWindowParameterized(effect->fieldSystem->bgConfig, work->window, 3, 0, 0, 0x20, 0x20, 0, 0);
        plttData = 0;
        BG_LoadPlttData(3, &plttData, 2, 0x1e);
        FillWindowPixelBuffer(work->window, 0);
        ScheduleWindowCopyToVram(work->window);
        ov01_021F05C4(&work->spriteEnv, 2, 1);
        ov120_0225F9D4(effect->narc, &work->sprite, &work->spriteEnv, &work->spriteRes, 0);
        GfGfx_EngineATogglePlanes(0x10, 1);
        work->hBlankSystem = effect->fieldSystem->unk4->hBlankSystem;
        ov120_0225F6FC(&work->steps[0], ov120_0225F8B0);
        ov120_0225F6FC(&work->steps[1], ov120_0225F90C);
        ov120_0225F6FC(&work->steps[2], ov120_0225F970);
        effect->state = 1;
        break;
    case 1:
        ov01_021EFCF8(1, -16, -16, &effect->effectComplete, 2);
        effect->state = 2;
        break;
    case 2:
        if (effect->effectComplete != FALSE) {
            effect->state = 3;
        }
        break;
    case 3:
        work->stepIndex = 0;
        effect->state = 4;
        break;
    case 4:
        finished = ov120_0225F704(&work->steps[work->stepIndex], work);
        if (finished != FALSE) {
            effect->state = 5;
        }
        break;
    case 5:
        sub_0200FC20(0);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        ClearWindowTilemapAndCopyToVram(work->window);
        RemoveWindow(work->window);
        WindowArray_Delete(work->window, 1);
        Sprite_Delete(work->sprite);
        ov01_021F06EC(&work->spriteEnv, &work->spriteRes);
        ov01_021F05F4(&work->spriteEnv);
        ov01_021EFCDC(effect, task);
        return;
    }
    if (effect->state != 5) {
        SpriteList_RenderAndAnimateSprites(work->spriteEnv.unk_00);
    }
}
