#include "global.h"

#include "wild_encounter.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "player_data.h"
#include "sav_system_info.h"

struct BerryPatch;

struct MailMessage;

struct ExtraSaveKey;

struct GameTime;

struct MiscSaveBlock;

struct PersistedMapFeatures;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct String;

struct SysInfo;

struct PlayerProfile;

struct UnkStruct_02030A80_t;

struct WiFiHistory;

typedef struct UnkStruct_02030A80_t UnkStruct_02030A80;

typedef u16 charcode_t;

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

enum ExtraSaveTableEntryID {
    EXTRA_SAVE_TABLE_ENTRY_HALL_OF_FAME = 0,
    EXTRA_SAVE_TABLE_ENTRY_FRONTIER,
    EXTRA_SAVE_TABLE_ENTRY_MY_RECORDINGS,
    EXTRA_SAVE_TABLE_ENTRY_DL_RECORDINGS_0,
    EXTRA_SAVE_TABLE_ENTRY_DL_RECORDINGS_1,
    EXTRA_SAVE_TABLE_ENTRY_DL_RECORDINGS_2,

    EXTRA_SAVE_TABLE_ENTRY_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

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

typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;

const charcode_t *PlayerProfile_GetNamePtr(const PlayerProfile *info);

enum AppearanceDataParam {
    APPEARANCE_DATA_INDEX = 0,
    APPEARANCE_DATA_TRAINER_CLASS_2,
    APPEARANCE_DATA_TRAINER_CLASS_1,
};

int GetUnionRoomAvatarAttrBySprite(int gender, int appearance, enum AppearanceDataParam param);

charcode_t *CopyU16StringArray(charcode_t *dst, const charcode_t *src);

PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);

typedef struct BerryPatch {
    u8 berryID;
    u8 growthStage;
    u16 stageMinutesRemaining;
    u16 moistureMinutesRemaining;
    u8 replantCount;
    u16 yield;
    u8 moistureRating;
    u8 yieldRating;
    u8 mulchType;
    u8 isGrowing;
} BerryPatch;

typedef struct PersistedMapFeatures {
    int id;
    u8 buffer[32];
} PersistedMapFeatures;

typedef struct ExtraSaveKey {
    u32 keys[EXTRA_SAVE_TABLE_ENTRY_MAX - 1];
    u32 oldKeys[EXTRA_SAVE_TABLE_ENTRY_MAX - 1];
    u8 keyFlags[EXTRA_SAVE_TABLE_ENTRY_MAX - 1];
    u8 padding[3];
} ExtraSaveKey;

typedef struct MiscSaveBlock {
    BerryPatch berryPatches[128];
    PersistedMapFeatures persistedMapFeatures;
    u16 rivalName[7 + 1];
    u16 tabletName[10 + 1];
    u16 favoriteMon;
    u8 favoriteMonForm : 7;
    u8 favoriteMonIsEgg : 1;
    u8 extraSaveInitalized : 1;
    u8 vsRecorderColor : 4;
    u8 padding : 3;
    u32 unused;
    MailMessage introMsg;
    ExtraSaveKey extraKey;
} MiscSaveBlock;

const MiscSaveBlock *Save_Misc_Const_Get(const SaveData *saveData);

void SaveMisc_GetFavoriteMon(const MiscSaveBlock *miscSave, int *species, int *form, int *isEgg);

void SaveMisc_GetBattleGreeting(const MiscSaveBlock *miscSave, MailMessage *message);

typedef struct GameTime {
    BOOL canary;
    RTCDate date;
    RTCTime time;
    u32 day;
    s64 startTimestamp;
    s64 firstCompletionTimestamp;
    u32 penaltyInMinutes;
} GameTime;

SysInfo *Save_SysInfo_Get(SaveData *saveData);

WiFiHistory *Save_WiFiHistory_Get(SaveData *saveData);

u8 WifiHistory_GetPlayerCountry(const WiFiHistory *wiFiHistory);

u8 WiFiHistory_GetPlayerRegion(const WiFiHistory *wiFiHistory);

UnkStruct_02030A80 *sub_02030A80(enum HeapID heapID);

String *sub_020315B8(const UnkStruct_02030A80 *param0, enum HeapID heapID);

void sub_020314C4(UnkStruct_02030A80 *param0, SaveData *saveData);

void sub_020314C4(UnkStruct_02030A80 *param0, SaveData *saveData) {
    PlayerProfile *v0 = Save_PlayerData_GetProfile(saveData);
    WiFiHistory *wiFiHistory = Save_WiFiHistory_Get(saveData);
    SysInfo *systemData = Save_SysInfo_Get(saveData);
    const MiscSaveBlock *miscSaveBlock = Save_Misc_Const_Get(saveData);
    int species, form, isEgg;
    int v7;
    OSOwnerInfo ownerInfo;

    OS_GetOwnerInfo(&ownerInfo);

    SaveMisc_GetFavoriteMon(miscSaveBlock, &species, &form, &isEgg);
    MI_CpuClear8(param0, sizeof(UnkStruct_02030A80));
    CopyU16StringArray(param0->unk_00, PlayerProfile_GetNamePtr(v0));

    param0->id = PlayerProfile_GetTrainerID(v0);
    param0->gender = PlayerProfile_GetTrainerGender(v0);
    param0->species = species;
    param0->form = form;
    param0->isEgg = isEgg;
    param0->country = WifiHistory_GetPlayerCountry(wiFiHistory);
    param0->region = WiFiHistory_GetPlayerRegion(wiFiHistory);

    for (v7 = 0; v7 < 40; v7++) {
        param0->unk_20_val2[v7] = 0xffff;
    }

    SaveMisc_GetBattleGreeting(miscSaveBlock, &param0->unk_20_val1);

    param0->month = ownerInfo.birthday.month;
    param0->appearance = GetUnionRoomAvatarAttrBySprite(PlayerProfile_GetTrainerGender(v0), PlayerProfile_GetAvatar(v0), APPEARANCE_DATA_INDEX);
    param0->version = GAME_VERSION;
    param0->language = LANGUAGE_ENGLISH;
    param0->unk_7C.unk_00 = SaveArray_CalcCRC16(saveData, param0, sizeof(UnkStruct_02030A80) - (sizeof(UnkStruct_0202F298_sub1)));
}
