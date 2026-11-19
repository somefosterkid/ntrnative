#ifndef POKEHEARTGOLD_TRAINER_CASE_H
#define POKEHEARTGOLD_TRAINER_CASE_H

#include "global.h"

#define TRAINER_CASE_MAX_BADGES 8

struct PlayTime;

typedef struct TrainerCaseBadge {
    int obtained : 1;
    int polish : 31;
} TrainerCaseBadge;

typedef struct TrainerCase {
    u8 gameVersion;
    u8 language;
    u8 gymLeadersToHideUnused;
    u8 cardLevel;
    u8 badgesInteractable : 1;
    u8 liveTimeDisplay : 1;
    u8 gender : 1;
    u8 pokedexObtained : 1;
    u8 signatureExists : 1;
    u8 trainerAppearance;
    u8 padding[2];
    u16 name[7 + 1];
    const struct PlayTime *playTime;
    u32 money;
    u32 seenPokemon;
    u32 score;
    u16 trainerId;
    u16 playTimeHours;
    u16 hofDebutHour;
    u8 playTimeMinutes;
    u8 adventureStartedYear;
    u8 adventureStartedMonth;
    u8 adventureStartedDay;
    u8 hofDebutYear;
    u8 hofDebutMonth;
    u8 hofDebutDay;
    u8 hofDebutMinute;
    u32 timesLinked;
    u32 linkBattleWins;
    u32 linkBattleLosses;
    u32 linkTrades;
    TrainerCaseBadge badges[TRAINER_CASE_MAX_BADGES];
    u8 signature[24 * 8 * (8 * (8 / 8))];
    u16 dummyChecksum;
    u16 unk_66A;
} TrainerCase;

#endif // POKEHEARTGOLD_TRAINER_CASE_H
