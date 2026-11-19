#include "global.h"

#include "overlay_80_022340E8.h"
#include "sys_task.h"

SysTask *ov80_022383C0(SysTaskFunc callback, void *param, u32 priority);

SysTask *BattleArcade_SetPartyBeforeBattle(SysTaskFunc callback, void *param, int priority) {
    return ov80_022383C0(callback, param, priority);
}
