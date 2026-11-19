#include "global.h"

#include "sys_task.h"
#include "field/field_3d_object_task.h"

void Field3dObjectTask_Delete(Field3dObjectTask *task);

void ov02_0224DE08(Field3dObjectTask *task);

void ov02_0224DE08(Field3dObjectTask *task) {
    Field3dObjectTask_Delete(task);
}
