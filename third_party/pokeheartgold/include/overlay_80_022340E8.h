#ifndef POKEHEARTGOLD_OVERLAY_80_022340E8_H
#define POKEHEARTGOLD_OVERLAY_80_022340E8_H

#include "global.h"
#include "sys_task.h"
#include "pokemon_types_def.h"

struct BattleArcade;

void ov80_022344D4(struct BattleArcade *arg0);

SysTask *BattleArcade_SetPartyBeforeBattle(SysTaskFunc callback, void *param, int priority);

void ov80_022352C8(void *arg0);
void ov80_022352D0(void *arg0);
void ov80_022352D8(void *arg0);
void ov80_022352E0(void *arg0);
void ov80_0223530C(void *arg0);
void ov80_02235314(void);
void ov80_02235318(void);
void ov80_0223531C(void);
void ov80_02235320(void);

void ov80_02235364(Party *party, Party *playerParty, s32 a2, s32 a3);

#endif // POKEHEARTGOLD_OVERLAY_80_022340E8_H
