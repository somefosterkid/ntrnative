#ifndef POKEHEARTGOLD_WIFI_BATTLE_TOWER_MATCH_CANDIDATE_H
#define POKEHEARTGOLD_WIFI_BATTLE_TOWER_MATCH_CANDIDATE_H

#include "global.h"

typedef struct WifiBattleTowerMatchCandidate {
    u16 trainerName[8];
    u8 padding_10[2];
    u8 country;
    u8 region;
    u8 padding_14[4];
    u16 trainerKey[4];
    u8 isAnonymous : 1;
    u8 anonymousNameIdx : 1;
} WifiBattleTowerMatchCandidate; // 0x22

#endif // POKEHEARTGOLD_WIFI_BATTLE_TOWER_MATCH_CANDIDATE_H
