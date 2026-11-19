#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "sys_task.h"
#include "sys_task_api.h"

void ov70_02238E58(GTSApplicationState *);

void ov70_02244290(BgConfig *);

void ov70_02244644(GTSApplicationState *);

void ov70_022446A8(GTSApplicationState *);

u32 ov70_02244154(GTSApplicationState *param_1);

u32 ov70_02244154(GTSApplicationState *param_1) {
    if (param_1->unk_12D0.unk_00 != NULL) {
        SysTask_Destroy(param_1->unk_12D0.unk_00);
        param_1->unk_12D0.unk_00 = 0;
        param_1->unk_12D0.unk_04 = 0;
    }
    ov70_022446A8(param_1);
    ov70_02244644(param_1);
    ov70_02244290(param_1->bgConfig);
    param_1->unk_12D0.unk_04 = 0;
    ov70_02238E58(param_1);
    if (param_1->screenId == 0) {
        return 5;
    }
    return 1;
}
