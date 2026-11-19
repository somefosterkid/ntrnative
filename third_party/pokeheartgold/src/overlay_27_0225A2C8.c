#include "global.h"

#include "overlay_manager.h"
#include "error_handling.h"
#include "overlay_27.h"
#include "overlay_27_ov27task.h"
#include "sys_task.h"

struct OverlayManager;

BOOL ov27_0225A2C8(OverlayManager *appMan, int *state);

BOOL ov27_0225A2C8(OverlayManager *appMan, int *state) {
    return 1;
}

void ov27_0225A2CC(SysTask *task) {
    Ov27Task *v0;

    v0 = SysTask_GetData(task);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    v0->unk_51C_5 = 1;
}

void ov27_0225A2EC(SysTask *task, s32 arg1) {
    Ov27Task *v0;
    v0 = SysTask_GetData(task);
    if (v0 == 0L) {
        GF_AssertFail();
    }
    if ((arg1 != 1) && (arg1 != 2)) {
        GF_AssertFail();
    }
    v0->unk_51C_6 = arg1;
}
