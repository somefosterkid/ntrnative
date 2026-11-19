#ifndef POKEHEARTGOLD_OVERLAY_80_022310C4_H
#define POKEHEARTGOLD_OVERLAY_80_022310C4_H

#include "global.h"
#include "sys_task.h"
#include "save.h"

SysTask *ov80_022313C0(SysTaskFunc callback, void *param, int priority);

u8 ov80_022317C0(void *arg0);

u16 ov80_022318D0(SaveData *saveData, u8 challengeType, u8 type, u16 *type1Rank, u16 *type2Rank);
u16 ov80_022318D0(SaveData *saveData, u8 challengeType, u8 type, u16 *type1Rank, u16 *type2Rank);
void ov80_02231930(SaveData *saveData, u8 challengeType, u8 type, u8 value);

#endif // POKEHEARTGOLD_OVERLAY_80_022310C4_H
