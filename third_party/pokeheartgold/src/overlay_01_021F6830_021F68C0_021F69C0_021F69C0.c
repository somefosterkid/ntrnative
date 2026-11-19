#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021F6830.h"
#include "screen_fade.h"

void ov01_021F6968(SubAppTaskData *data);

void ov01_021F69A4(SubAppTaskData *data);

void ov01_021F69C0(SysTask *task, void *taskData) {
    SubAppTaskData *data = taskData;
    s32 iVar1;
    BOOL BVar2;

    switch (data->state) {
    case 0:
        ov01_021F6968(data);
        data->state = 1;
        return;
    case 2:
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 2, 1, HEAP_ID_FIELD1);
        data->state = 3;
        return;
    case 3:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            data->state = 4;
            return;
        }
        break;
    case 4:
        ov01_02206C60[data->appIdx].start(data->fieldSystem->bgConfig, data->appTask);
        data->state = 5;
        return;
    case 5:
        iVar1 = ov01_02206C60[data->appIdx].main(data->fieldSystem->bgConfig);
        if (iVar1 == 1) {
            ov01_021F69A4(data);
            data->appIdx = data->nextAppIdx;
            data->state = 6;
            return;
        }
        break;
    case 6:
        ov01_021F6968(data);
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 2, 1, HEAP_ID_FIELD1);
        data->state = 7;
        return;
    case 7:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            data->state = 1;
        }
    }
    return;
}
