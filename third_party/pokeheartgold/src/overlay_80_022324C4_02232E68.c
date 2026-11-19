#include "global.h"

#include "overlay_80_022324C4.h"
#include "sys_task.h"
#include "overlay_80_battle_castle.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_save.h"
#include "save_frontier.h"

struct BattleCastleSave;

struct FrontierTrainerBase;

struct SaveData;

typedef struct FrontierTrainerBase FrontierTrainerBase;

void Heap_Free(void *ptr);

typedef struct UnkStruct_ov80_022324C4_02232E68_2CEC {
    u32 trainerID;
    u16 trainerType;
    u16 objectID;
    u16 trainerName[7 + 1];
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
} UnkStruct_ov80_022324C4_02232E68_2CEC;

FrontierTrainerBase *ov80_02229F04(UnkStruct_ov80_022324C4_02232E68_2CEC *trainer, int trainerID, enum HeapID heapID, enum NarcId narcID);

u16 ov80_0222A30C(u8 trainerClass);

u16 ov80_02232E68(BattleCastle *castle, u8 trainerSlot);

struct BattleCastle;
struct FrontierSave;
struct UnkStruct_ov80_022324C4_02232E68_2CEC;
void ov80_02232B58(BattleCastle *castle, u8 saveType);
enum BattleFrontierStatsIndex sub_0205C174(u8 challengeType, u8 rankType);
int sub_0205C268(u32 statIndex);
FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
u16 sub_02031108(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);
void ov80_02232E9C(BattleCastle *castle);
void ov80_02232EE0(BattleCastle *castle);

SysTask *ov80_02237E30(SysTaskFunc callback, void *param, u32 priority);

u16 ov80_02232E68(BattleCastle *castle, u8 trainerSlot) {
    UnkStruct_ov80_022324C4_02232E68_2CEC trainer;
    u8 offset = castle->currentBattle + (trainerSlot * 7);

    Heap_Free(ov80_02229F04(&trainer, castle->trainerIDs[offset], HEAP_ID_FIELD2, (enum NarcId)204));

    return ov80_0222A30C(trainer.trainerType);
}

void ov80_02232E9C(BattleCastle *castle) {
    for (int i = 0; i < 3; i++) {
        sub_02031108(Save_Frontier_GetStatic(castle->saveData), sub_0205C174(castle->challengeType, i), sub_0205C268(sub_0205C174(castle->challengeType, i)), 1);
    }

    ov80_02232B58(castle, 1);
}

void ov80_02232EE0(BattleCastle *castle) {
    castle->saveStreak = 1;

    if (castle->currentRound < ((49 / 7) + 1)) {
        castle->currentRound++;
    }

    castle->currentBattle = 0;
    ov80_02232B58(castle, 0);
}

SysTask *ov80_02232F00(SysTaskFunc callback, void *param, int priority) {
    return ov80_02237E30(callback, param, priority);
}
