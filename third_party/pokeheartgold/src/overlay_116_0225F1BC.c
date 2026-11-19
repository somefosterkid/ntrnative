#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "field_system_def.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_01_021F467C.h"
#include "overlay_01_encounter_effect.h"
#include "overlay_116.h"
#include "overlay_120.h"
#include "screen_fade.h"
#include "sys_task.h"

void ov116_0225F1BC(SysTask *task, EncounterEffect *effect, const LugiaHoOhEncounterParam *param);

void ov116_0225F1BC(SysTask *task, EncounterEffect *effect, const LugiaHoOhEncounterParam *param) {
    u16 uVar1;
    void *pvVar2;
    fx32 fVar4;
    s32 iVar5;
    BOOL BVar6;
    LugiaHoOhEncounterEffect *a0;

    a0 = effect->param;
    switch (effect->state) {
    case 0:
        pvVar2 = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LugiaHoOhEncounterEffect));
        effect->param = pvVar2;
        memset(pvVar2, 0, sizeof(LugiaHoOhEncounterEffect));
        GfGfx_EngineATogglePlanes(2, 0);
        GfGfx_EngineATogglePlanes(4, 0);
        GfGfx_EngineATogglePlanes(8, 0);
        effect->state = effect->state + 1;
        return;
    case 1:
        ov01_021EFCF8(1, 0x10, 0x10, &effect->effectComplete, 1);
        effect->state = effect->state + 1;
        return;
    case 2:
        if (effect->effectComplete != 0) {
            effect->state = effect->state + 1;
            return;
        }
        break;
    case 3:
        a0->motionBlur = ov01_021F467C(param->blendCoeffA, param->blendCoeffB);
        uVar1 = Camera_GetPerspectiveAngle(effect->fieldSystem->camera);
        ov01_021EFE34(&a0->perspectiveTask, (u32)uVar1, (u32)uVar1 + param->perspectiveDelta, param->perspectiveSteps);
        effect->state = effect->state + 1;
        return;
    case 4:
        iVar5 = ov01_021EFE44(&a0->perspectiveTask);
        Camera_SetPerspectiveAngle((u16)a0->perspectiveTask.currentValue, effect->fieldSystem->camera);
        if (iVar5 == 1) {
            effect->state = effect->state + 1;
            a0->delay = param->delayFrames;
            return;
        }
        break;
    case 5:
        a0->delay = a0->delay - 1;
        if (a0->delay < 0) {
            fVar4 = Camera_GetDistance(effect->fieldSystem->camera);
            ov01_021EFEC8(&a0->distanceTask, fVar4, fVar4 + param->distanceDelta, param->distanceInitialRate, param->distanceSteps);
            effect->state = effect->state + 1;
            return;
        }
        break;
    case 6:
        iVar5 = ov01_021EFF28(&a0->distanceTask);
        Camera_SetDistance(a0->distanceTask.currentValue, effect->fieldSystem->camera);
        if (iVar5 == 1) {
            effect->state = effect->state + 1;
            return;
        }
        break;
    case 7:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7fff, param->fadeFrames, 1, HEAP_ID_FIELD1);
        effect->state = effect->state + 1;
        return;
    case 8:
        BVar6 = IsPaletteFadeFinished();
        if (BVar6 != 0) {
            effect->effectComplete = 0;
            effect->state = effect->state + 1;
            return;
        }
        break;
    case 9:
        sub_0200FBF4(PM_LCD_BOTTOM, 0x7fff);
        ov01_021F46DC(&a0->motionBlur);
        reg_G2_BLDCNT = 0;
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        ov01_021EFCDC(effect, task);
        sub_0200FBF4(PM_LCD_BOTTOM, 0x7fff);
    }
    return;
}
