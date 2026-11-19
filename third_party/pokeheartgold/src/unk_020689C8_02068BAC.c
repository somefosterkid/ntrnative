#include "global.h"

#include "sys_task_api.h"
#include "overworld_anim_manager.h"

void sub_02068BAC(OverworldAnimManagerList *managerList) {
    int count = sub_02068CC4(managerList);
    OverworldAnimManager *iter = sub_02068CC8(managerList);

    do {
        u32 flags = OVERWORLD_ANIM_MANAGER_ACTIVE_FLAG | OVERWORLD_ANIM_MANAGER_TICKING_FLAG;

        if (sub_02068CEC(iter, flags) == flags) {
            sub_02068D34(iter);
        }

        iter++;
        count--;
    } while (count != 0);
}

SysTask *sub_02068BDC(OverworldAnimManager *manager, int priority) {
    SysTask *sysTask = SysTask_CreateOnMainQueue(sub_02068BFC, manager, priority);
    GF_ASSERT(sysTask != NULL);

    return sysTask;
}
