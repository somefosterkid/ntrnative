#include "global.h"

#include "error_handling.h"
#include "overlay_07_battle_anim_system.h"
#include "sys_task_api.h"

void ov07_0221BEA4(s32 arg0, BattleAnimSystem *arg1, SysTask *arg2);

void ov07_0221BEA4(s32 arg0, BattleAnimSystem *arg1, SysTask *task) {
    switch (arg0) {
    case 1:
        arg1->activeAnimTasks = (u16)(arg1->activeAnimTasks - 1);
        break;
    case 2:
        arg1->activeSoundTasks = (u16)(arg1->activeSoundTasks - 1);
        break;
    default:
        GF_AssertFail();
        break;
    }
    SysTask_Destroy(task);
}
