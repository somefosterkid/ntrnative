#include "global.h"

#include "overlay_80_0222AEF8.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"
#include "player_data.h"

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
typedef u16 charcode_t;
BOOL sub_02037030(int cmd, const void *data, int size);
PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);

BOOL ov80_0222AF10(BattleHall *battleHall);

void ov80_0222AEF8(int dummy1, int sLotoId, void *dummy2, void *battleHall) {
    BattleHall *battleHall_dupe = battleHall;
    const u16 *unused2 = dummy2;

    battleHall_dupe->msgsReceived++;

    if (sub_0203769C() == dummy1) {
        return;
    }
}

BOOL ov80_0222AF10(BattleHall *battleHall) {
    int dummy1;
    PlayerProfile *dummy2;
    u16 *data;
    int result, dataSize = ((int)(sizeof(battleHall->commBuffer) / sizeof(*(battleHall->commBuffer))));
    data = battleHall->commBuffer;
    dummy1 = 0;
    dummy2 = Save_PlayerData_GetProfile(battleHall->saveData);

    dummy1 += 1;
    dummy1 += (7 + 1);

    if (sub_02037030(34, data, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222AF3C(int dummy1, int sLotoId, void *dummy2, void *battleHall) {
    BattleHall *battleHall_dupe = battleHall;
    const u16 *unused2 = dummy2;

    battleHall_dupe->msgsReceived++;

    if (sub_0203769C() == dummy1) {
        return;
    }
}
