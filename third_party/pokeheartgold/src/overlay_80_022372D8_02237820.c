#include "global.h"

#include "overlay_80_022372D8.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"

struct BattleHallSave;
struct FieldBattleDTO;
struct Party;
struct Pokemon;
u8 CalcShininessByOtIdAndPersonality(u32 monOTID, u32 monPersonality);
typedef struct BattleHallSave BattleHallSave;
typedef struct BattleHall {
    int unused;
    u8 challengeType;
    u8 currentBattle;
    u8 saveStreak;
    u8 opponentsLevel;
    u16 currentStreak;
    u16 currentRound;
    fx32 playerLevelSqrt;
    u32 unk_10;
    int wonBattle;
    u16 trainerIDs[10 * 2];
    FrontierOpponent opponents[2];
    u8 partySlots[2];
    Party *party;
    u16 monIndices[10 * 2];
    FrontierPokemon opponentMons[10 * 2];
    u16 unk_6F0;
    u16 selectedTypeIdx2;
    u8 selectedType;
    u8 selectedTypeIdx;
    BattleHallSave *hallSave;
    SaveData *saveData;
    FieldBattleDTO *dto;
    u8 typeRanks[4][9];
    u16 heldItems[2];
    u16 commBuffer[44];
    u8 commHugeBuffer[512];
    u8 unk_984[2][512];
    u16 unk_D84[2];
    u8 unk_D88;
    Pokemon *partnersMon;
    u8 msgsReceived;
    u32 unk_D94;
} BattleHall;

BOOL ov80_02237820(BattleHall *battleHall, u8 index);

BOOL ov80_02237820(BattleHall *battleHall, u8 index) {
    if (battleHall->opponentMons[index].personality > ((24 + 1) * 10001)) {
        battleHall->opponentMons[index].personality -= ((24 + 1) * 10001);
    } else {
        battleHall->opponentMons[index].personality += ((24 + 1) * 10001);
    }

    return CalcShininessByOtIdAndPersonality(battleHall->opponentMons[index].otID, battleHall->opponentMons[index].personality);
}

u32 ov80_02237850(u8 challengeType) {
    switch (challengeType) {
    case 0:
        return (1 << 7) | ((0 << 0) | (1 << 0));
    case 1:
        return (1 << 7) | ((1 << 1) | (1 << 0));
    case 2:
        return ((1 << 2) | (1 << 7)) | ((1 << 1) | (1 << 0)) | (1 << 3);
    case 3:
        return ((1 << 2) | (1 << 7)) | ((1 << 1) | (1 << 0)) | (1 << 3);
    }

    return (1 << 7) | ((0 << 0) | (1 << 0));
}

u8 ov80_0223787C(u8 challengeType) {
    if (challengeType == 1) {
        return 2;
    }
    return 1;
}

u8 ov80_02237888(u8 challengeType) {
    if (challengeType == 1) {
        return 2;
    }
    return 1;
}
