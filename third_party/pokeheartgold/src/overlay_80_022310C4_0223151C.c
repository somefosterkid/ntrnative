#include "global.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"
#include "constants/heap.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "save_frontier.h"

struct BattleHall;
struct BattleHallSave;
struct FieldBattleDTO;
struct FrontierOpponent;
struct FrontierPokemon;
struct FrontierTrainer;
struct Party;
struct Pokemon;
struct SaveData;
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
enum BattleFrontierStatsIndex sub_0205C0F4(u8 challengeType);
enum BattleFrontierStatsIndex sub_0205C11C(u8 challengeType);
enum BattleFrontierStatsIndex sub_0205C144(u8 challengeType);
int sub_0205C268(u32 statIndex);
BOOL sub_020313C4(SaveData *saveData, enum BattleFrontierStatsIndex recordIndex, enum BattleFrontierStatsIndex speciesIndex, int hostFriendID, int challengeType, enum HeapID heapID, int *resultCode, int *saveResult);

void ov80_0223151C(BattleHall *battleHall, u16 *updated);

void ov80_0223151C(BattleHall *battleHall, u16 *updated) {
    int unused, unused2;

    if (battleHall->challengeType != 3) {
        *updated = sub_020313C4(battleHall->saveData, sub_0205C0F4(battleHall->challengeType), sub_0205C11C(battleHall->challengeType), sub_0205C268(sub_0205C0F4(battleHall->challengeType)), sub_0205C144(battleHall->challengeType), HEAP_ID_FIELD2, &unused, &unused2);
    } else {
        *updated = 0;
    }
}
