#include "global.h"

#include "field_communication_manager.h"
#include "sys_task_api.h"

void sub_020582CC(SysTask *task, void *data);

void sub_020582CC(SysTask *task, void *data) {
    FieldCommTask v0;

    if (_021D41C8 == NULL) {
        SysTask_Destroy(task);
        return;
    }
    v0 = _021D41C8->task;
    if ((v0 != NULL) && (_021D41C8->pauseTask == 0)) {
        v0();
    }
}

void sub_020582F4(FieldCommTask task, u16 time);

void sub_020582F4(FieldCommTask task, u16 time) {
    _021D41C8->task = task;
    _021D41C8->timer = time;
    return;
}
