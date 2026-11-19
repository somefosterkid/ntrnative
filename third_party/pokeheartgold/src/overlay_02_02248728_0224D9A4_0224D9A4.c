#include "global.h"

#include "field/field_3d_object_task.h"

#include "field_system.h"
#include "overlay_02_field3d_effects.h"

void ov02_0224D880(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D914(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D950(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D98C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

Field3dObjectTask *ov02_0224D9A4(FieldSystem *fieldSystem);

Field3dObjectTask *ov02_0224D9A4(FieldSystem *fieldSystem) {
    static const Field3dObjectTaskTemplate v0 = {
        1024,
        sizeof(Ov02App_0224D880),
        ov02_0224D880,
        ov02_0224D914,
        ov02_0224D950,
        ov02_0224D98C
    };
    Field3dObjectTask *v1 = Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &v0);
    return v1;
}
