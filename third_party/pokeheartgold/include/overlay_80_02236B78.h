#ifndef POKEHEARTGOLD_OVERLAY_80_02236B78_H
#define POKEHEARTGOLD_OVERLAY_80_02236B78_H

#include "global.h"

int ov80_02236B78(u8 challengeType, int currentRound, int trainerOffset);

void ov80_02236BE4(u8 challengeType, int currentRound, u16 trainerIDs[], u8 numTrainers);

u8 ov80_02236DD4(u8 challengeType);
u8 ov80_02236DF8(u8 challengeType, BOOL includePartnersMons);

void ov80_02236E24(u32 param_1, u32 param_2, u16 *param_3, void *param_4, u8 *param_5, void *param_6, u16 param_7, s32 param_8);

u32 ov80_022370F4(u8 challengeType);

struct BattleFactory;

void ov80_02237130(struct BattleFactory *factory);

void ov80_022371B0(s32 param_1);

BOOL ov80_02237254(u8 challengeType);
BOOL ov80_02237254(u8 challengeType);

#endif // POKEHEARTGOLD_OVERLAY_80_02236B78_H
