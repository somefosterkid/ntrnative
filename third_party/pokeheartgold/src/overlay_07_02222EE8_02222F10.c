#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "sys_task.h"
#include "overlay_07_palette_fade.h"
#include "sys_task_api.h"

void ov07_02222E74(SysTask *task, void *param);

PaletteFadeContext *ov07_02222F10(PaletteData *param_1, enum HeapID param_2, u32 param_3, u16 param_4, u16 param_5, u8 param_6, s8 param_7, u8 param_8, u8 param_9, u16 param_10, u32 param_11);

PaletteFadeContext *ov07_02222F10(PaletteData *param_1, enum HeapID param_2, u32 param_3, u16 param_4, u16 param_5, u8 param_6, s8 param_7, u8 param_8, u8 param_9, u16 param_10, u32 param_11) {
    PaletteFadeContext *data;
    SysTask *pSVar1;
    data = Heap_Alloc(param_2, sizeof(PaletteFadeContext));
    if (data == 0L) {
        GF_AssertFail();
    }
    data->paletteData = param_1;
    data->bufferID = param_3;
    data->index = param_4;
    data->count = param_5;
    data->color = param_10;
    data->startFrac = param_8;
    data->endFrac = param_9;
    data->fraction = param_8;
    data->stepFrames = param_6;
    data->timer = param_6;
    param_7 = (data->startFrac < data->endFrac) ? (param_7) : (-param_7);
    data->step = param_7;
    data->active = 1;
    pSVar1 = SysTask_CreateOnMainQueue(ov07_02222E74, data, param_11);
    data->task = pSVar1;
    return data;
}
