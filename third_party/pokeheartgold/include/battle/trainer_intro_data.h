#ifndef POKEHEARTGOLD_BATTLE_TRAINER_INTRO_DATA_H
#define POKEHEARTGOLD_BATTLE_TRAINER_INTRO_DATA_H

#include "battle_setup.h"
#include "heap.h"
#include "party.h"
#include "pm_string.h"

// The link battle trainer intro's work block (allocated by ov12_0223A260 / ov12_0223A3F0, handed to ov05_0221BA00,
// freed by ov12_0223A5E4); size: 0x30 on the DS.
typedef struct TrainerIntroData {
    BattleSetup *setup;
    Party *party[BATTLER_MAX];
    String *trainerNames[BATTLER_MAX];
    enum HeapID heapID;
    u8 mode;
    u8 playerSide;
    u8 battleResult;
    u8 isDone;
    u8 recordingType;
} TrainerIntroData;

#endif // POKEHEARTGOLD_BATTLE_TRAINER_INTRO_DATA_H
