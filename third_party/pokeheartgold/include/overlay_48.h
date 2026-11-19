#ifndef POKEHEARTGOLD_OVERLAY_48_H
#define POKEHEARTGOLD_OVERLAY_48_H

#include "global.h"
#include "overlay_manager.h"
#include "sys_task.h"

s32 ov48_022589FC(OverlayManager *arg0, int *state);

void ov48_02258BF4(MtxFx33 *param0, const VecFx32 *param1);
void ov48_02258C6C(MtxFx33 *param0, const VecFx32 *param1);

void ov48_02258CE4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5);

void ov48_02258F0C(VecFx32 *param0);
void ov48_02259090(void *param0);

u32 ov48_022592E0(const VecFx32 *param0, const VecFx32 *param1);

s32 ov48_02259AD0(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov48_0225A288(void *arg0);

void ov48_0225A680(SysTask *param0, void *param1);

u16 ov48_0225B038(u32 param0);
u16 ov48_0225B050(u32 param0);

#endif // POKEHEARTGOLD_OVERLAY_48_H
