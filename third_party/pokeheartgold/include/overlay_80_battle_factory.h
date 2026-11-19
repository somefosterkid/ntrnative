#ifndef POKEHEARTGOLD_OVERLAY_80_BATTLE_FACTORY_H
#define POKEHEARTGOLD_OVERLAY_80_BATTLE_FACTORY_H

// BattleFactory: merged by tools/hg/promote.py from 21 m2c local copies (1 variants, base
// src/overlay_80_0222AEF8_0222B140_0222B140.c) and 41 accesses; fields at every accessed offset, pads elsewhere.

#include "overlay_80_frontier_opponent.h"

struct BattleFactorySave;
struct FieldBattleDTO;
struct Party;
struct SaveData;
struct SysTask;

typedef struct BattleFactory {
    int unused;
    u8 challengeType;
    u8 isOpenLevel;
    u8 currentBattle;
    u8 unused2;
    u16 tradeCount;
    u8 saveStreak;
    u8 seenFactoryHeadIntro;
    u16 currentStreak;
    u16 currentRound;
    u32 unused3;
    int wonBattle;
    u16 trainerIDs[14];
    FrontierOpponent opponents[2];
    u16 initialRentalSetIDs[6];
    u8 initialRentalIVs[6];
    u8 pad_266[0x2];
    u32 initialRentalPersonalities[6];
    FrontierPokemon initialRentalMons[6];
    u16 unusd4;
    u16 opponentMonSetIDs[4];
    u8 opponentMonIVs[4];
    u8 pad_3DE[0x2];
    u32 opponentMonPersonalities[4];
    FrontierPokemon opponentMons[4];
    u16 unused5;
    u8 pad_4D2[0x2];
    struct Party *playersParty;
    struct Party *opponentsParty;
    u16 selectedAppSlots[6];
    u16 playerMonSetIDs[6];
    struct BattleFactorySave *factorySave;
    struct SaveData *saveData;
    struct FieldBattleDTO *dto;
    struct SysTask *corridorAnimation;
    u16 commBuffer[60];
    u8 unk_57C;
    u8 unk_57D;
    u16 unk_57E;
    u16 unk_580;
    u16 unk_582;
    u16 partnerRentalSetIDs[6];
    u8 partnerRentalIVs[6];
    u8 pad_596[0x2];
    u32 partnerRentalPersonalities[6];
    FrontierPokemon partnerRentalMons[6];
    u16 unk_700;
    u8 msgsReceived;
    u8 pad_703[0x1];
    u32 unk_704;
} BattleFactory;

// sizeof(BattleFactory) == 0x708

#endif // POKEHEARTGOLD_OVERLAY_80_BATTLE_FACTORY_H
