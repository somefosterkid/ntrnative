#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"

struct BattleHallSave;

struct FieldBattleDTO;

struct FrontierTrainerBase;

struct Party;

struct Pokemon;

struct SaveData;

typedef struct FrontierTrainerBase FrontierTrainerBase;

void Heap_Free(void *ptr);

typedef struct UnkStruct_ov80_022310C4_022317D0_022317D0_2CE4 {
    u32 trainerID;
    u16 trainerType;
    u16 objectID;
    u16 trainerName[7 + 1];
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
} UnkStruct_ov80_022310C4_022317D0_022317D0_2CE4;

FrontierTrainerBase *ov80_02229F04(UnkStruct_ov80_022310C4_022317D0_022317D0_2CE4 *trainer, int trainerID, enum HeapID heapID, enum NarcId narcID);

u16 ov80_0222A30C(u8 trainerClass);

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

u16 ov80_022317D0(BattleHall *battleHall, u8 trainerSlot);

u16 ov80_022317D0(BattleHall *battleHall, u8 trainerSlot) {
    UnkStruct_ov80_022310C4_022317D0_022317D0_2CE4 trainer;
    u8 offset = (battleHall->currentBattle * 2) + trainerSlot;

    Heap_Free(ov80_02229F04(&trainer, battleHall->trainerIDs[offset], HEAP_ID_FIELD2, (enum NarcId)204));

    return ov80_0222A30C(trainer.trainerType);
}
