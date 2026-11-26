#ifndef POKEHEARTGOLD_OVERLAY_01_021FC66C_H
#define POKEHEARTGOLD_OVERLAY_01_021FC66C_H

#include "global.h"
#include "sys_task.h"
#include "field_types_def.h"

struct FishingTask;

s32 ov01_021FC76C(SysTask *arg0);
s32 ov01_021FC778(SysTask *arg0);

s32 ov01_021FC7DC(void *arg0, PlayerAvatar *arg1, LocalMapObject *arg2);

u32 ov01_021FC814(s32 *param_1, void *param_2, void *param_3);

s32 ov01_021FC8E8(struct FishingTask *arg0, void *arg1, void *arg2);

s32 ov01_021FC934(struct FishingTask *arg0, void *arg1, void *arg2);
s32 ov01_021FC968(struct FishingTask *arg0, void *arg1, void *arg2);

u32 ov01_021FC98C(struct FishingTask *param_1, u32 param_2, LocalMapObject *param_3);
u32 ov01_021FC9AC(struct FishingTask *param_1, u32 param_2, LocalMapObject *param_3);

u32 ov01_021FC9E8(struct FishingTask *param_1, u32 param_2, LocalMapObject *param_3);

s32 ov01_021FCA94(struct FishingTask *arg0, void *arg1, void *arg2);
s32 ov01_021FCAA8(struct FishingTask *arg0, void *arg1, void *arg2);

void ov01_021FCB14(void *arg0);

s32 ov01_021FCC2C(void *arg0);

#endif // POKEHEARTGOLD_OVERLAY_01_021FC66C_H
