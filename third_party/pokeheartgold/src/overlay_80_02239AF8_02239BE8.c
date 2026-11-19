#include "global.h"

#include "overlay_80_02239AF8.h"
#include "sys_task.h"

SysTask *Sprite_DeleteAndFreeResources(SysTaskFunc callback, void *param, u32 priority);

SysTask *ov80_02239BE8(SysTaskFunc callback, void *param, int priority) {
    return Sprite_DeleteAndFreeResources(callback, param, priority);
}
