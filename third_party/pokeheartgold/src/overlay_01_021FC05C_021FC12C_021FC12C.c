#include "global.h"

#include "field/field_3d_object_task.h"

#include "overlay_01_021FC05C.h"

s32 ov01_021FC12C(Field3dObjectTask *task) {
    HoneyScentFadeData *fade = Field3dObjectTask_GetData(task);
    return fade->done;
}
