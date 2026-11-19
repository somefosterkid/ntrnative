#ifndef POKEHEARTGOLD_OVERLAY_12_BATTLE_CONTROLLER_OPPONENT_H
#define POKEHEARTGOLD_OVERLAY_12_BATTLE_CONTROLLER_OPPONENT_H

#include "global.h"
#include "sys_task.h"
#include "battle/battle.h"
#include "filesystem.h"
#include "pokepic.h"

struct TrainerEncounterData;

void ov12_0225D890(SysTask *task, void *data);
void ov12_0225D990(SysTask *task, void *data);
void ov12_0225DA18(SysTask *task, void *data);

void ov12_0225DAD4(SysTask *task, void *data);

void ov12_0225E104(SysTask *arg0, void *arg1);
void ov12_0225E134(SysTask *arg0, void *arg1);

void ov12_0225E154(SysTask *task, void *data);

void ov12_0225E1D4(SysTask *arg0, void *arg1);
void ov12_0225E1FC(SysTask *arg0, void *arg1);

void ov12_0225E250(SysTask *task, void *data);
void ov12_0225E404(SysTask *task, void *data);

void ov12_0225E4CC(SysTask *arg0, void *arg1);
void ov12_0225E4EC(SysTask *arg0, void *arg1);

void ov12_0225E568(SysTask *task, void *data);

void ov12_0225E6FC(SysTask *arg0, void *arg1);
void ov12_0225E740(SysTask *arg0, void *arg1);

void ov12_0225E760(SysTask *task, void *data);
void ov12_0225E830(SysTask *task, void *data);

void ov12_0225F3A4(SysTask *task, void *data);

void ov12_0225F3FC(SysTask *task, void *data);

void ov12_0225F434(SysTask *task, void *data);
void ov12_0225F4E0(SysTask *task, void *data);

void ov12_0225F8AC(SysTask *task, void *data);

void ov12_0225F960(SysTask *arg0, void *arg1);

void ov12_0225F980(SysTask *task, void *data);
void ov12_0225FA44(SysTask *task, void *data);

void ov12_0225FC80(SysTask *arg0, void *arg1);
void ov12_0225FCA0(SysTask *arg0, void *arg1);

void ov12_0225FCC0(SysTask *task, void *data);

void ov12_02260030(SysTask *task, void *data);

void ov12_022609F8(SysTask *task, void *data);

void ov12_02261B2C(BattleSystem *param_1, u32 param_2, s32 param_3, s32 param_4, BOOL param_5, s16 param_6, s16 param_7);

BOOL ov12_02261DC8(struct TrainerEncounterData *arg0, ManagedSprite *unused);

void ov12_02261F38(BattleSystem *arg0, s32 arg1, u32 arg2, Pokepic *arg3, NARC *arg4, s32 arg5, s32 arg6, s32 arg7, u32 arg8);

#endif // POKEHEARTGOLD_OVERLAY_12_BATTLE_CONTROLLER_OPPONENT_H
