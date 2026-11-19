#include "global.h"
#include "overlay_80_battle_arcade.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_12_field_battle_dto.h"

struct BattleArcadeSave;

struct FrontierTrainerBase;

typedef struct FrontierTrainerBase FrontierTrainerBase;

void Heap_Free(void *ptr);

typedef struct UnkStruct_ov80_022340E8_02234774_02234774_2D1E {
    u32 trainerID;
    u16 trainerType;
    u16 objectID;
    u16 trainerName[7 + 1];
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
} UnkStruct_ov80_022340E8_02234774_02234774_2D1E;

FrontierTrainerBase *ov80_02229F04(UnkStruct_ov80_022340E8_02234774_02234774_2D1E *trainer, int trainerID, enum HeapID heapID, enum NarcId narcID);

u16 ov80_0222A30C(u8 trainerClass);

typedef struct BattleArcadeSave BattleArcadeSave;

u32 ov80_022347A8(BattleArcade *arcade, u32 trainerSlot, s32);

u16 ov80_02234774(BattleArcade *arcade, u8 trainerSlot, s32 arg2);

u16 ov80_02234774(BattleArcade *arcade, u8 trainerSlot, s32 arg2) {
    UnkStruct_ov80_022340E8_02234774_02234774_2D1E trainer;
    u8 offset = ov80_022347A8(arcade, trainerSlot, arg2);

    Heap_Free(ov80_02229F04(&trainer, arcade->trainerIDs[offset], HEAP_ID_FIELD2, (enum NarcId)204));

    return ov80_0222A30C(trainer.trainerType);
}
