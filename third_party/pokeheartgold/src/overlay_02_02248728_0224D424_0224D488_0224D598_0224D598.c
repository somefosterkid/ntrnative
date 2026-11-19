#include "global.h"

#include "field/field_3d_object_task.h"

#include "field_system.h"
#include "overlay_02_field3d_effects.h"

void ov02_0224D43C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D468(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D488(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D580(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

Field3dObjectTask *ov02_0224D598(FieldSystem *fieldSystem);

Field3dObjectTask *ov02_0224D598(FieldSystem *fieldSystem) {
    static const Field3dObjectTaskTemplate v0 = {
        1024,
        sizeof(UnkStruct_ov6_02248498),
        ov02_0224D43C,
        ov02_0224D468,
        ov02_0224D488,
        ov02_0224D580
    };
    Field3dObjectTask *v1 = Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &v0);
    return v1;
}
