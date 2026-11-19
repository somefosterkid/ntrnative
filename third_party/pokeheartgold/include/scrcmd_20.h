#ifndef POKEHEARTGOLD_SCRCMD_20_H
#define POKEHEARTGOLD_SCRCMD_20_H

#include "save.h"

struct BattleHallStreakFlags;

typedef struct UnkStruct_0204F1E4 {
    u8 state;
    u8 unk01;
    u8 filler[2];
    u16 playerTeam;
    u16 partnerTeam;
    u16 *result;
} UnkStruct_0204F1E4;

void sub_0204F85C(int a0, int unused, void *data, void *work);
void sub_0204F878(SaveData *, struct BattleHallStreakFlags *, u8);

#endif // POKEHEARTGOLD_SCRCMD_20_H
