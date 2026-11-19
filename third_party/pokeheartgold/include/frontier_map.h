#ifndef POKEHEARTGOLD_FRONTIER_MAP_H
#define POKEHEARTGOLD_FRONTIER_MAP_H

#include "global.h"
#include "sys_task.h"
#include "constants/heap.h"

void FrontierMap_Free(void *arg0);

void FrontierMap_VBlank(void *param);

void ov80_02238AAC(SysTask *task, void *data);

void ov80_02238AB0(SysTask *param0, void *param1);

void FrontierMap_Update(SysTask *param0, void *param1);


void FrontierMap_LoadPaletteData(void **param_1);

void ov80_02238FA0(void *arg0);

void ov80_022392DC(enum HeapID arg0);

void ov80_02239384(void *arg0);

void ov80_022395E8(void *arg0, s32 arg1, s32 *arg2, s32 *arg3);

#endif // POKEHEARTGOLD_FRONTIER_MAP_H
