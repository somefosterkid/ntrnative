#ifndef POKEHEARTGOLD_OVERLAY_80_02237A70_H
#define POKEHEARTGOLD_OVERLAY_80_02237A70_H

#include "global.h"

int ov80_02237A70(u8 challengeType, int currentRound, int trainerOffset);

void ov80_02237ADC(u8 challengeType, int currentRound, u16 trainerIDs[], u8 numTrainers);
u8 ov80_02237B24(u8 challengeType, BOOL includePartnersMons);

u32 ov80_02237D5C(u8 challengeType);
BOOL ov80_02237D8C(u8 challengeType);

#endif // POKEHEARTGOLD_OVERLAY_80_02237A70_H
