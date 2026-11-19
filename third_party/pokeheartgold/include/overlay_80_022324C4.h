#ifndef POKEHEARTGOLD_OVERLAY_80_022324C4_H
#define POKEHEARTGOLD_OVERLAY_80_022324C4_H

#include "global.h"
#include "sys_task.h"

struct BattleCastle;

void ov80_02232824(struct BattleCastle *param_1, s32 param_2);

void ov80_022329B4(struct BattleCastle *arg0);

u8 ov80_02232E58(struct BattleCastle *arg0);

SysTask *ov80_02232F00(SysTaskFunc callback, void *param, int priority);

#endif // POKEHEARTGOLD_OVERLAY_80_022324C4_H
