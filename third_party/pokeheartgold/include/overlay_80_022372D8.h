#ifndef POKEHEARTGOLD_OVERLAY_80_022372D8_H
#define POKEHEARTGOLD_OVERLAY_80_022372D8_H

#include "global.h"

void ov80_022372D8(u8 selectedTypeIdx, u8 numTrainers, u8 typeRank, u8 currentBattle, u16 trainerIDs[]);
void ov80_02237334(u8 challengeType, u8 numTrainers, int currentRound, u8 rank, u8 currentBattle, u16 trainerIDs[]);

u32 ov80_02237850(u8 challengeType);
u8 ov80_0223787C(u8 challengeType);
u8 ov80_02237888(u8 challengeType);

void ov80_02237894(u8 *arg0, s32 arg1, s32 arg2, u16 *arg3, s32 arg4, s32 arg5, s32 arg6);

u8 ov80_02237920(u8 spot);

BOOL ov80_0223792C(u8 challengeType);

fx32 ov80_02237A40(u32 level);

#endif // POKEHEARTGOLD_OVERLAY_80_022372D8_H
