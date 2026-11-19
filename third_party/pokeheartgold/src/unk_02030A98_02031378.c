#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save_frontier.h"

struct BattleHallWinRecords;

typedef struct BattleHallWinRecords BattleHallWinRecords;

u16 sub_020312E0(SaveData *saveData, BattleHallWinRecords *records, int challengeType, int species);

u16 sub_0203132C(BattleHallWinRecords *records, int challengeType, int species, u16 streak);

u16 sub_02031378(SaveData *saveData, BattleHallWinRecords *records, int challengeType, int species, u16 newRecord);

struct FrontierSave;

enum LoadResult {
    LOAD_RESULT_EMPTY = 0,
    LOAD_RESULT_OK,
    LOAD_RESULT_CORRUPT,
    LOAD_RESULT_ERROR,
};
BattleHallWinRecords *sub_020312C4(SaveData *saveData, enum HeapID heapID, int *resultCode);
int sub_020312CC(SaveData *saveData, BattleHallWinRecords *records);
FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);
BOOL sub_020313C4(SaveData *saveData, enum BattleFrontierStatsIndex recordIndex, enum BattleFrontierStatsIndex speciesIndex, int hostFriendID, int challengeType, enum HeapID heapID, int *resultCode, int *saveResult);

struct UnkStruct_02030A80_t;
typedef struct UnkStruct_02030A80_t UnkStruct_02030A80;
typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_0202F298_sub1;
struct UnkStruct_02030A80_t {
    u16 unk_00[8];
    u32 id;
    u8 gender;
    u8 month;
    u8 appearance;
    u8 country;
    u8 region;
    u8 version;
    u8 language;
    u8 isEgg : 1;
    u8 form : 7;
    u16 species;
    u8 unk_1E;
    u8 unk_1F;
    union {
        MailMessage unk_20_val1;
        u16 unk_20_val2[40];
    };
    u8 unk_70[12];
    UnkStruct_0202F298_sub1 unk_7C;
};
UnkStruct_02030A80 *sub_020314A4(enum HeapID heapID);

u16 sub_02031378(SaveData *saveData, BattleHallWinRecords *records, int challengeType, int species, u16 newRecord) {
    if (Save_CheckExtraChunksExist(saveData) == 0) {
        return 0;
    }

    u16 currentRecord = sub_020312E0(saveData, records, challengeType, species);

    if (currentRecord < newRecord) {
        return sub_0203132C(records, challengeType, species, newRecord);
    }
    if (currentRecord > 9999) {
        return sub_0203132C(records, challengeType, species, 9999);
    }

    return currentRecord;
}

BOOL sub_020313C4(SaveData *saveData, enum BattleFrontierStatsIndex recordIndex, enum BattleFrontierStatsIndex speciesIndex, int hostFriendID, int challengeType, enum HeapID heapID, int *resultCode, int *saveResult) {
    u16 newRecord, currentRecord;
    int species;
    BOOL updated = 0;

    ((recordIndex >= 34 && recordIndex <= 60) ? (void)0 : GF_AssertFail());
    ((speciesIndex == STAT_HALL_LATEST_SPECIES_SINGLE || speciesIndex == STAT_HALL_LATEST_SPECIES_DOUBLE || speciesIndex == STAT_HALL_LATEST_SPECIES_MULTI) ? (void)0 : GF_AssertFail());

    *resultCode = LOAD_RESULT_OK;
    *saveResult = 2;

    if (hostFriendID != 0xff) {
        return updated;
    }

    if (!Save_CheckExtraChunksExist(saveData)) {
        return updated;
    }

    FrontierSave *frontier = Save_Frontier_GetStatic(saveData);
    newRecord = FrontierSave_GetStat(frontier, recordIndex, hostFriendID);
    species = FrontierSave_GetStat(frontier, speciesIndex, hostFriendID);
    BattleHallWinRecords *records = sub_020312C4(saveData, heapID, resultCode);

    if (*resultCode != LOAD_RESULT_OK) {
        currentRecord = 0;
    } else {
        currentRecord = sub_020312E0(saveData, records, challengeType, species);
    }

    sub_02031378(saveData, records, challengeType, species, newRecord);

    if (newRecord != currentRecord) {
        *saveResult = sub_020312CC(saveData, records);
        updated = 1;
    }

    if (records != 0L) {
        Heap_Free(records);
    }

    return updated;
}

UnkStruct_02030A80 *sub_020314A4(enum HeapID heapID) {
    UnkStruct_02030A80 *v0 = Heap_Alloc(heapID, sizeof(UnkStruct_02030A80));
    MI_CpuClear8(v0, sizeof(UnkStruct_02030A80));

    return v0;
}
