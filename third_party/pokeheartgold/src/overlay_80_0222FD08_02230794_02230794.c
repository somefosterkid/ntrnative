#include "global.h"
#include "overlay_80_battle_factory.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_12_field_battle_dto.h"

struct BattleFactorySave;

struct FrontierTrainerBase;

struct SysTask;

struct SysTaskQueue;

typedef struct FrontierTrainerBase FrontierTrainerBase;

void Heap_Free(void *ptr);

typedef struct UnkStruct_ov80_0222FD08_02230794_02230794_2F53 {
    u32 trainerID;
    u16 trainerType;
    u16 objectID;
    u16 trainerName[7 + 1];
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
} UnkStruct_ov80_0222FD08_02230794_02230794_2F53;

FrontierTrainerBase *ov80_02229F04(UnkStruct_ov80_0222FD08_02230794_02230794_2F53 *trainer, int trainerID, enum HeapID heapID, enum NarcId narcID);

u16 ov80_0222A30C(u8 trainerClass);

typedef struct BattleFactorySave BattleFactorySave;

u16 ov80_02230794(BattleFactory *factory, u8 trainerSlot);

u16 ov80_02230794(BattleFactory *factory, u8 trainerSlot) {
    UnkStruct_ov80_0222FD08_02230794_02230794_2F53 trainer;
    u8 offset = factory->currentBattle + (trainerSlot * 7);

    Heap_Free(ov80_02229F04(&trainer, factory->trainerIDs[offset], HEAP_ID_FIELD2, (enum NarcId)204));

    return ov80_0222A30C(trainer.trainerType);
}
