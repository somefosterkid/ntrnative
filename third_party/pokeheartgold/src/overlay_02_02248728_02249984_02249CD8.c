#include "global.h"

#include "screen_fade.h"
#include "sys_task_api.h"
#include "overlay_02_hmcut_in.h"

s32 ov02_02249CD8(HMCutIn *arg0);

s32 ov02_02249CD8(HMCutIn *arg0) {
    if (IsPaletteFadeFinished() != 0) {
        arg0->state += 1;
    }
    return 0;
}

void ov02_02249D5C(SysTask *task, void *data);

void ov02_02249CF0(HMCutIn *arg0);

void ov02_02249CF0(HMCutIn *arg0) {
    arg0->resourcesLoaded = 0;
    arg0->resourceSourcesFreed = 0;
    arg0->resourceTransferTask = SysTask_CreateOnVBlankQueue(ov02_02249D5C, arg0, 0x80U);
}

void ov02_02249E58(SysTask *task, void *data);

void ov02_02249D18(HMCutIn *arg0);

void ov02_02249D18(HMCutIn *arg0) {
    arg0->resourcesLoaded = 0;
    arg0->resourceSourcesFreed = 0;
    arg0->resourceTransferTask = SysTask_CreateOnVBlankQueue(ov02_02249E58, arg0, 0x80U);
}

void ov02_02249D40(HMCutIn *arg0);

void ov02_02249D40(HMCutIn *arg0) {
    SysTask *sysTask;

    sysTask = arg0->resourceTransferTask;
    if (sysTask != NULL) {
        SysTask_Destroy(sysTask);
        arg0->resourceTransferTask = NULL;
    }
}
