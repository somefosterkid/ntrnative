#include "global.h"

#include "sys_task_api.h"
#include "overworld_anim_manager.h"

OverworldAnimManager *sub_02068B0C(OverworldAnimManagerList *managerList, const OverworldAnimManagerFuncs *funcs, const VecFx32 *initialPos, int userInt, const void *userData, int sysTaskPriority) {
    OverworldAnimManagerExtendedFuncs funcsEx;
    funcsEx.baseFuncs = *funcs;
    funcsEx.unused1Func = sub_02068DD8;
    funcsEx.unused2Func = sub_02068DDC;

    return sub_02068A08(managerList, &funcsEx, initialPos, userInt, userData, sysTaskPriority);
}

void sub_02068B48(OverworldAnimManager *manager) {
    GF_ASSERT(manager != NULL);

    if (sub_02068CCC(manager) != 0) {
        sub_02068D50(manager);
        manager->list->activeCount--;

        SysTask *task = sub_02068DA0(manager);

        if (task != NULL) {
            SysTask_Destroy(task);
        }

        sub_02068C6C(manager);
    }
}
