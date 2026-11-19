#include "global.h"

#include "field/field_3d_object_task.h"

#include "field_system.h"
#include "overlay_02_field3d_effects.h"

void ov02_0224DCB0(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DD38(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DD4C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DD8C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DDC8(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

Field3dObjectTask *ov02_0224DDE0(FieldSystem *fieldSystem);

Field3dObjectTask *ov02_0224DDE0(FieldSystem *fieldSystem) {
    static const Field3dObjectTaskTemplate v0 = {
        1024,
        sizeof(Ov02App_0224DCB0),
        ov02_0224DCB0,
        ov02_0224DD4C,
        ov02_0224DD8C,
        ov02_0224DDC8
    };
    Field3dObjectTask *v1 = Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &v0);
    return v1;
}

Field3dObjectTask *ov02_0224DDF4(FieldSystem *fieldSystem);

Field3dObjectTask *ov02_0224DDF4(FieldSystem *fieldSystem) {
    static const Field3dObjectTaskTemplate v0 = {
        1024,
        sizeof(Ov02App_0224DCB0),
        ov02_0224DD38,
        ov02_0224DD4C,
        ov02_0224DD8C,
        ov02_0224DDC8
    };
    Field3dObjectTask *v1 = Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &v0);
    return v1;
}
