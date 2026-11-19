#ifndef POKEHEARTGOLD_OVERLAY_80_BATTLE_CASTLE_H
#define POKEHEARTGOLD_OVERLAY_80_BATTLE_CASTLE_H

#include "overlay_80_frontier_opponent.h"

// BattleCastle: merged by tools/hg/promote.py from 22 m2c local copies (1 variants, base
// src/overlay_80_0222AEF8_0222B5C8.c) and 47 accesses; fields at every accessed offset, pads elsewhere.

struct BattleCastleSave;
struct FieldBattleDTO;
struct Party;
struct SaveData;

typedef struct BattleCastle {
    int unused;
    struct SaveData *saveData;
    struct BattleCastleSave *castleSave;
    struct FieldBattleDTO *dto;
    u8 challengeType;
    u8 currentBattle;
    u8 monFainted;
    u8 seenCastleValetIntro;
    u16 currentStreak;
    u16 currentRound;
    u32 unused2;
    int wonBattle;
    u16 initialCP;
    u16 newCP;
    u8 partySlots[3];
    u8 saveStreak;
    struct Party *playersParty;
    struct Party *opponentsParty;
    u16 trainerIDs[14];
    FrontierOpponent opponents[2];
    u16 monSetIDs[4];
    u8 opponentMonIVs[4];
    u32 opponentMonPersonalities[4];
    FrontierPokemon opponentMons[4];
    u16 unused3;
    u16 savedHeldItems[3];
    u8 appIdentityUnlocked[4];
    u8 levelAdjustments[4];
    u8 appStatsUnlocked[4];
    u8 appMovesUnlocked[4];
    u16 selectedAppSlots[6];
    u16 unused4[4];
    u16 startingPP[4][4];
    u16 unused5[6];
    u16 commBuffer[40];
    u8 commHugeBuffer[512];
    u8 unk_610[2][512];
    u8 unk_A10;
    u8 unk_A11;
    u16 unk_A12;
    u16 unused6;
    u16 unk_A16;
    u8 unk_A18;
    u8 unk_A19;
    u8 msgsReceived;
    u8 unk_A1B;
    u16 partnersCP;
    u8 pad_A1E[0x2];
    u16 *unk_A20;
    u32 unused7;
} BattleCastle;

// sizeof(BattleCastle) == 0xa28

#endif // POKEHEARTGOLD_OVERLAY_80_BATTLE_CASTLE_H
