#ifndef POKEHEARTGOLD_OVERLAY_80_0223AC24_H
#define POKEHEARTGOLD_OVERLAY_80_0223AC24_H

#include "global.h"
#include "bg_window.h"
#include "constants/heap.h"
#include "sys_task.h"

void ov80_0223AC24(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 arg4);

void ov80_0223AF60(void *arg0);

struct ScreenShakeEffect;
void ov80_0223B424(struct ScreenShakeEffect *screenShake, enum HeapID heapID);

void ov80_0223B468(SysTask *param0, void *param1);
void ov80_0223B484(SysTask *param0, void *param1);
void ov80_0223B468(SysTask *param0, void *param1);
void ov80_0223B484(SysTask *param0, void *param1);

void ov80_0223B528(void *arg0);

SysTask *ov80_0223B53C(SysTaskFunc callback, void *param, int priority);

#endif // POKEHEARTGOLD_OVERLAY_80_0223AC24_H
