#include "global.h"

#include "field/field_3d_object_task.h"
#include "overlay_02_0224D0C8.h"

s32 ov02_0224D424(Field3dObjectTask *task);

s32 ov02_0224D424(Field3dObjectTask *task) {
    Field3dEffectData *data = Field3dObjectTask_GetData(task);
    if (data->state == 1) {
        return 1;
    }
    return 0;
}
