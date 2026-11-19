#include "global.h"

#include "sys_task_api.h"
#include "overlay_72_wfc_connect.h"

s32 ov72_02238AEC(void *);

s32 ov72_02238FFC(NintendoWfcConnect *);

s32 ov72_02239098(NintendoWfcConnect *);

s32 ov72_0223897C(NintendoWfcConnect *arg0);

s32 ov72_0223897C(NintendoWfcConnect *arg0) {
    SysTask *task;
    s32 v0;

    task = arg0->paletteCycle.task;
    if (task != NULL) {
        SysTask_Destroy(task);
        arg0->paletteCycle.task = NULL;
        arg0->paletteCycle.active = 0;
    }
    ov72_02239098(arg0);
    ov72_02238FFC(arg0);
    ov72_02238AEC(arg0->bgConfig);
    arg0->paletteCycle.active = 0;
    v0 = arg0->nextScreen;
    arg0->screen = v0;
    if (v0 == 0) {
        return 5;
    }
    return 1;
}
