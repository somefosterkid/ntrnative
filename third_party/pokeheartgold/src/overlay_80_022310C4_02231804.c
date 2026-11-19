#include "global.h"

#include "save.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"

struct BattleHall;
struct BattleHallSave;
struct FieldBattleDTO;
struct Party;
struct Pokemon;
typedef struct BattleHallSave BattleHallSave;
void sub_02030C34(u8 *typeRanks);
typedef struct BattleHall BattleHall;
struct BattleHall {
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
};
void ov80_0223157C(BattleHall *battleHall, u8 saveType);
BOOL ov80_0222AF10(BattleHall *battleHall);
BOOL ov80_0222AF54(BattleHall *battleHall);
BOOL ov80_0222AFB8(BattleHall *battleHall);
BOOL ov80_0222B024(BattleHall *battleHall, u8 param1);
BOOL ov80_0222B070(BattleHall *battleHall);

void ov80_02231804(BattleHall *battleHall);
void ov80_02231828(BattleHall *battleHall);
BOOL ov80_02231844(BattleHall *battleHall, u16 command, u16 arg);

void ov80_02231804(BattleHall *battleHall) {
    sub_02030C34(&battleHall->typeRanks[battleHall->challengeType][0]);
    ov80_0223157C(battleHall, 1);
}

void ov80_02231828(BattleHall *battleHall) {
    battleHall->saveStreak = 1;

    if (battleHall->currentRound < ((170 / 10) + 1)) {
        battleHall->currentRound++;
    }

    battleHall->currentBattle = 0;
    ov80_0223157C(battleHall, 0);
}

BOOL ov80_02231844(BattleHall *battleHall, u16 command, u16 arg) {
    int success;

    switch (command) {
    case 0:
        success = ov80_0222AF10(battleHall);
        break;
    case 1:
        success = ov80_0222AF54(battleHall);
        break;
    case 2:
        success = ov80_0222AFB8(battleHall);
        break;
    case 3:
        success = ov80_0222B024(battleHall, arg);
        break;
    case 7:
        success = ov80_0222B070(battleHall);
        break;
    }

    return success;
}
