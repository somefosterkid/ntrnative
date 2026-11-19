#include "global.h"

#include "sys_task_api.h"
#include "overworld_anim_manager.h"

OverworldAnimManager *sub_02068A08(OverworldAnimManagerList *managerList, const OverworldAnimManagerExtendedFuncs *funcs, const VecFx32 *initialPos, int userInt, const void *userData, int sysTaskPriority) {
    int i = 0;
    int count = sub_02068CC4(managerList);
    OverworldAnimManager *iter = sub_02068CC8(managerList);

    do {
        if (!sub_02068CCC(iter)) {
            break;
        }

        i++;
        iter++;
    } while (i < count);

    if (i >= count) {
        return NULL;
    }

    sub_02068CD8(iter);
    sub_02068D8C(iter, userInt);
    sub_02068D94(iter, userData);
    sub_02068DA4(iter, managerList);

    if (initialPos != NULL) {
        sub_02068DA8(iter, initialPos);
    } else {
        VecFx32 zeroPos = { 0, 0, 0 };
        sub_02068DA8(iter, &zeroPos);
    }

    sub_02068D78(iter, funcs->baseFuncs.ctxSize);
    sub_02068CF4(iter, funcs->baseFuncs.initFunc);
    sub_02068D48(iter, funcs->baseFuncs.exitFunc);
    sub_02068D10(iter, funcs->baseFuncs.tickFunc);
    sub_02068D2C(iter, funcs->baseFuncs.renderFunc);
    sub_02068D64(iter, funcs->unused1Func);
    sub_02068D6C(iter, funcs->unused2Func);

    SysTask *tickSysTask = sub_02068BDC(iter, sysTaskPriority);

    if (tickSysTask == NULL) {
        sub_02068C6C(iter);
        return NULL;
    }

    sub_02068D9C(iter, tickSysTask);

    if (!sub_02068CFC(iter)) {
        SysTask_Destroy(tickSysTask);
        sub_02068C6C(iter);
        return NULL;
    }

    sub_02068CE4(iter, OVERWORLD_ANIM_MANAGER_TICKING_FLAG);
    managerList->activeCount++;

    return iter;
}
