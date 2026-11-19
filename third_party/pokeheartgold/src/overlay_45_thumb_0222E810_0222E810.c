#include "global.h"
#include "save.h"
#include "unk_0202C034.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct DWCstAccLoginId;

struct DWCstAccUserData;

struct PPW_LobbyCallbacks;

struct PPW_LobbyQuestionnaire;

struct PPW_LobbyQuestionnaireRecord;

struct PPW_LobbyRecruitInfo;

struct PPW_LobbySchedule;

struct PPW_LobbyScheduleRecord;

struct PPW_LobbySystemProfile;

struct PPW_LobbyTimeInfo;

struct PPW_LobbyVipRecord;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct WiFiHistory;

struct WiFiList;

typedef enum DWC_LOBBY_CHANNEL_PREFIX {
    DWC_LOBBY_CHANNEL_PREFIX_RELEASE,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG1,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG2,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG3,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG4,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG5,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG6,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG7,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG8,
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG9,
    DWC_LOBBY_CHANNEL_PREFIX_TEST
} DWC_LOBBY_CHANNEL_PREFIX;

typedef enum PPW_LOBBY_STATS_RESULT {
    PPW_LOBBY_STATS_RESULT_SUCCESS,
    PPW_LOBBY_STATS_RESULT_INVALID_PARAM,
    PPW_LOBBY_STATS_RESULT_SERVER_ERROR
} PPW_LOBBY_STATS_RESULT;

typedef struct PPW_LobbyScheduleRecord {
    s32 time;
    PPW_LOBBY_TIME_EVENT event;
} PPW_LobbyScheduleRecord;

typedef struct PPW_LobbySchedule {
    u32 lockTime;
    u32 random;
    u32 roomFlag;
    u8 roomType;
    u8 season;
    u16 scheduleRecordNum;
    PPW_LobbyScheduleRecord scheduleRecords[1];
} PPW_LobbySchedule;

typedef void (*PPW_LobbyJoinChannelCallback)(BOOL success, PPW_LOBBY_CHANNEL_KIND channelKind);

typedef void (*PPW_LobbyPlayerConnectedCallback)(s32 userId, const PPW_LobbySystemProfile *systemProfile, const u8 *userProfile, u32 userProfileSize);

typedef void (*PPW_LobbyPlayerDisconnectedCallback)(s32 userId);

typedef void (*PPW_LobbyStringMessageReceivedCallback)(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const char *message);

typedef void (*PPW_LobbyBinaryMessageReceivedCallback)(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const u8 *data, u32 dataSize);

typedef void (*PPW_LobbyGetChannelDataCallback)(BOOL success, BOOL broadcast, PPW_LOBBY_CHANNEL_KIND channelKind, s32 userId, const char *key, const u8 *data, u32 dataSize, void *param);

typedef void (*PPW_LobbySystemProfileUpdatedCallback)(s32 userId, const PPW_LobbySystemProfile *systemProfile);

typedef void (*PPW_LobbyUserProfileUpdatedCallback)(s32 userId, const u8 *userProfile, u32 userProfileSize);

typedef void (*PPW_LobbyRecruitCallback)(s32 userId, const PPW_LobbyRecruitInfo *recruitInfo);

typedef void (*PPW_LobbyRecruitStoppedCallback)(s32 userId, const PPW_LobbyRecruitInfo *recruitInfo);

typedef void (*PPW_LobbyScheduleProgressCallback)(PPW_LOBBY_TIME_EVENT event);

typedef void (*PPW_LobbyCheckProfileCallback)(PPW_LOBBY_STATS_RESULT result, const u8 *userProfile, u32 userProfileSize);

typedef BOOL (*PPW_LobbyNotifyScheduleCallback)(PPW_LOBBY_STATS_RESULT result, const PPW_LobbySchedule *schedule);

typedef void (*PPW_LobbyGetVipCallback)(PPW_LOBBY_STATS_RESULT result, const PPW_LobbyVipRecord *vipRecords, u32 num);

typedef void (*PPW_LobbyGetQuestionnaireCallback)(PPW_LOBBY_STATS_RESULT result, const PPW_LobbyQuestionnaire *questionnaire);

typedef void (*PPW_LobbySubmitQuestionnaireCallback)(PPW_LOBBY_STATS_RESULT result);

typedef void (*PPW_LobbyExcessFloodCallback)(u32 floodWeight);

typedef struct PPW_LobbyCallbacks {
    PPW_LobbyJoinChannelCallback lobbyJoinChannelCallback;
    PPW_LobbyPlayerConnectedCallback lobbyPlayerConnectedCallback;
    PPW_LobbyPlayerDisconnectedCallback lobbyPlayerDisconnectedCallback;
    PPW_LobbyStringMessageReceivedCallback lobbyStringMessageReceivedCallback;
    PPW_LobbyBinaryMessageReceivedCallback lobbyBinaryMessageReceivedCallback;
    PPW_LobbyGetChannelDataCallback lobbyGetChannelDataCallback;
    PPW_LobbySystemProfileUpdatedCallback lobbySystemProfileUpdatedCallback;
    PPW_LobbyUserProfileUpdatedCallback lobbyUserProfileUpdatedCallback;
    PPW_LobbyRecruitCallback lobbyRecruitCallback;
    PPW_LobbyRecruitStoppedCallback lobbyRecruitStoppedCallback;
    PPW_LobbyScheduleProgressCallback lobbyScheduleProgressCallback;
    PPW_LobbyCheckProfileCallback lobbyCheckProfileCallback;
    PPW_LobbyNotifyScheduleCallback lobbyNotifyScheduleCallback;
    PPW_LobbyGetVipCallback lobbyGetVipCallback;
    PPW_LobbyGetQuestionnaireCallback lobbyGetQuestionnaireCallback;
    PPW_LobbySubmitQuestionnaireCallback lobbySubmitQuestionnaireCallback;
    PPW_LobbyExcessFloodCallback lobbyExcessFloodCallback;
} PPW_LobbyCallbacks;

extern PPW_LOBBY_RESULT ov45_022310C0(const char *gameName, const char *secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix, const PPW_LobbyCallbacks *lobbyCallbacks, const DWCUserData *dwcUserdata, const u8 *userProfile, u32 userProfileSize);

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

enum SaveTableEntryID {
    SAVE_TABLE_ENTRY_SYSTEM = 0,
    SAVE_TABLE_ENTRY_PLAYER,
    SAVE_TABLE_ENTRY_PARTY,
    SAVE_TABLE_ENTRY_BAG,
    SAVE_TABLE_ENTRY_VARS_FLAGS,
    SAVE_TABLE_ENTRY_POKETCH,
    SAVE_TABLE_ENTRY_FIELD_PLAYER_STATE,
    SAVE_TABLE_ENTRY_POKEDEX,
    SAVE_TABLE_ENTRY_DAYCARE,
    SAVE_TABLE_ENTRY_PAL_PAD,
    SAVE_TABLE_ENTRY_MISC,
    SAVE_TABLE_ENTRY_FIELD_OVERWORLD_STATE,
    SAVE_TABLE_ENTRY_UNDERGROUND,
    SAVE_TABLE_ENTRY_REGULATION_BATTLES,
    SAVE_TABLE_ENTRY_IMAGE_CLIPS,
    SAVE_TABLE_ENTRY_MAILBOX,
    SAVE_TABLE_ENTRY_POFFINS,
    SAVE_TABLE_ENTRY_RECORD_MIXED_RNG,
    SAVE_TABLE_ENTRY_JOURNAL,
    SAVE_TABLE_ENTRY_TRAINER_CASE,
    SAVE_TABLE_ENTRY_GAME_RECORDS,
    SAVE_TABLE_ENTRY_SEAL_CASE,
    SAVE_TABLE_ENTRY_CHATOT,
    SAVE_TABLE_ENTRY_FRONTIER,
    SAVE_TABLE_ENTRY_RIBBONS,
    SAVE_TABLE_ENTRY_ENCOUNTERS,
    SAVE_TABLE_ENTRY_GLOBAL_TRADE,
    SAVE_TABLE_ENTRY_TV_BROADCAST,
    SAVE_TABLE_ENTRY_RANKINGS,
    SAVE_TABLE_ENTRY_WIFI_LIST,
    SAVE_TABLE_ENTRY_WIFI_HISTORY,
    SAVE_TABLE_ENTRY_MYSTERY_GIFT,
    SAVE_TABLE_ENTRY_PAL_PARK_TRANSFER,
    SAVE_TABLE_ENTRY_LINK_CONTEST_RECORDS,
    SAVE_TABLE_ENTRY_UNLOCKED_EASY_CHAT_WORDS,
    SAVE_TABLE_ENTRY_EMAIL,
    SAVE_TABLE_ENTRY_WIFI_PLAZA_QUESTIONS,
    SAVE_TABLE_ENTRY_PC_BOXES,

    SAVE_TABLE_ENTRY_MAX
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

typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;

DWCUserData *sub_0202C08C(WiFiList *wiFiList);

extern BOOL ov45_0222F74C(PPW_LOBBY_RESULT param0);

extern void ov45_0222FC44(BOOL param0, PPW_LOBBY_CHANNEL_KIND lobbyChannelKind);

extern void ov45_0222FCE0(s32 userId, const PPW_LobbySystemProfile *param1, const u8 *param2, u32 param3);

extern void ov45_0222FD50(s32 param0);

extern void ov45_0222FDD4(s32 sLotoId, PPW_LOBBY_CHANNEL_KIND unused2, s32 unused3, const char *unused4);

extern void ov45_0222FDD8(s32 param0, PPW_LOBBY_CHANNEL_KIND param1, s32 param2, const u8 *param3, u32 param4);

void ov45_0222FE84(BOOL sLotoId, BOOL unused2, PPW_LOBBY_CHANNEL_KIND unused3, s32 unused4, const char *param4, const u8 *param5, u32 unused5, void *unused6);

extern void ov45_0222FEC4(s32 param0, const PPW_LobbySystemProfile *sysProfile);

extern void ov45_0222FF40(s32 param0, const u8 *param1, u32 unused);

extern void ov45_0222FF7C(s32 param0, const PPW_LobbyRecruitInfo *recruitInfo);

extern void ov45_02230008(s32 param0, const PPW_LobbyRecruitInfo *recruitInfo);

void ov45_02230050(PPW_LOBBY_TIME_EVENT timeEvent);

void ov45_02230064(PPW_LOBBY_STATS_RESULT statsResult, const u8 *param1, u32 param2);

BOOL ov45_02230090(PPW_LOBBY_STATS_RESULT statsResult, const PPW_LobbySchedule *unused);

extern void ov45_022300B0(PPW_LOBBY_STATS_RESULT lobbyStatsResult, const PPW_LobbyVipRecord *lobbyVipRecord, u32 param2);

extern void ov45_022300DC(PPW_LOBBY_STATS_RESULT lobbyStatsResult, const PPW_LobbyQuestionnaire *lobbyQuestionnaire);

void ov45_02230108(PPW_LOBBY_STATS_RESULT lobbyStatsResult);

void ov45_02230130(u32 unused);

BOOL ov45_0222E810(const void *param0, u32 param1);

extern const char ov45_02254F04[];

BOOL ov45_0222E810(const void *param0, u32 param1) {
    PPW_LobbyCallbacks v0;

    ((_022577C0 != 0L) ? (void)0 : GF_AssertFail());

    {
        v0.lobbyJoinChannelCallback = ov45_0222FC44;
        v0.lobbyPlayerConnectedCallback = ov45_0222FCE0;
        v0.lobbyPlayerDisconnectedCallback = ov45_0222FD50;
        v0.lobbyStringMessageReceivedCallback = ov45_0222FDD4;
        v0.lobbyBinaryMessageReceivedCallback = ov45_0222FDD8;
        v0.lobbyGetChannelDataCallback = ov45_0222FE84;
        v0.lobbySystemProfileUpdatedCallback = ov45_0222FEC4;
        v0.lobbyUserProfileUpdatedCallback = ov45_0222FF40;
        v0.lobbyRecruitCallback = ov45_0222FF7C;
        v0.lobbyRecruitStoppedCallback = ov45_02230008;
        v0.lobbyScheduleProgressCallback = ov45_02230050;
        v0.lobbyCheckProfileCallback = ov45_02230064;
        v0.lobbyNotifyScheduleCallback = ov45_02230090;
        v0.lobbyGetVipCallback = ov45_022300B0;
        v0.lobbyGetQuestionnaireCallback = ov45_022300DC;
        v0.lobbySubmitQuestionnaireCallback = ov45_02230108;
        v0.lobbyExcessFloodCallback = ov45_02230130;
    }

    {
        DWCUserData *userData = sub_0202C08C(_022577C0->wiFiList);
        _022577C0->result = ov45_022310C0(ov45_02254F04, "IIup73", (DWC_LOBBY_CHANNEL_PREFIX)(param1), &v0, userData, param0, _022577C0->unk_28);
    }

    return ov45_0222F74C(_022577C0->result);
}
