#include "global.h"

#include "overlay_80_0222AEF8.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"

struct BattleHallSave;

struct FieldBattleDTO;

struct Party;

struct Pokemon;


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

u16 sub_0203769C(void);

BOOL ov80_0222B024(BattleHall *battleHall, u8 param1);

void ov80_0222AF84(int netID, int unused, void *trainerIDs, void *battleHall) {
    int i, dummy;
    BattleHall *battleHall_dupe = battleHall;
    const u16 *trainerIDs_dupe = trainerIDs;

    dummy = 0;
    battleHall_dupe->msgsReceived++;

    if (sub_0203769C() == netID || sub_0203769C() == 0) {
        return;
    }

    for (i = 0; i < (10 * 2); i++) {
        battleHall_dupe->trainerIDs[i] = trainerIDs_dupe[i + dummy];
    }

    dummy += (10 * 2);
}

BOOL sub_02037030(int cmd, const void *data, int size);

BOOL ov80_0222AFB8(BattleHall *battleHall);

BOOL ov80_0222AFB8(BattleHall *battleHall) {
    int i, dummy;
    u16 *data;
    int result, dataSize = ((int)(sizeof(battleHall->commBuffer) / sizeof(*(battleHall->commBuffer))));
    data = battleHall->commBuffer;
    dummy = 0;

    for (i = 0; i < (10 * 2); i++) {
        data[i] = battleHall->monIndices[i];
    }

    dummy += (10 * 2);

    if (sub_02037030(36, data, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222AFEC(int netID, int unused, void *monIndices, void *battleHall) {
    int i, dummy;
    BattleHall *battleHall_dupe = battleHall;
    const u16 *monIndices_dupe = monIndices;

    dummy = 0;
    battleHall_dupe->msgsReceived++;

    if (sub_0203769C() == netID || sub_0203769C() == 0) {
        return;
    }

    for (i = 0; i < (10 * 2); i++) {
        battleHall_dupe->monIndices[i] = monIndices_dupe[i];
    }

    dummy += (10 * 2);
}

BOOL ov80_0222B024(BattleHall *battleHall, u8 param1) {
    u16 *data;
    int result, dataSize = ((int)(sizeof(battleHall->commBuffer) / sizeof(*(battleHall->commBuffer))));
    data = battleHall->commBuffer;

    *data = param1;

    if (sub_02037030(37, data, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B048(int netID, int unused, void *param2, void *battleHall) {
    BattleHall *battleHall_dupe = battleHall;
    const u16 *param2_dupe = param2;

    battleHall_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleHall_dupe->unk_D88 = *param2_dupe;
}
