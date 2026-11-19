#ifndef POKEHEARTGOLD_OVERLAY_47_H
#define POKEHEARTGOLD_OVERLAY_47_H

#include "global.h"
#include "sys_task.h"
#include "bg_window.h"
#include "constants/heap.h"

SysTask *ov47_02258A14(SysTaskFunc callback, void *param, int priority);

SysTask *ov47_02258DC8(SysTaskFunc callback, void *param, void *priority);

SysTask *ov47_02259480(SysTaskFunc callback, void *param, void *priority);

void ov47_02259C3C(Window *param_1, u32 param_2, void **param_3, enum HeapID param_4);

#endif // POKEHEARTGOLD_OVERLAY_47_H
