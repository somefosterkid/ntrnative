#include "global.h"
#include "save.h"
#include "unk_0202C034.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct PPW_LobbyQuestionnaire;

struct PPW_LobbyQuestionnaireRecord;

struct PPW_LobbyRecruitInfo;

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

typedef enum {
    UnkEnum_ov66_02232F38_00,
    UnkEnum_ov66_02232F38_01,
    UnkEnum_ov66_02232F38_02,
    UnkEnum_ov66_02232F38_03,
} UnkEnum_ov66_02232F38;

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

s32 ov45_0222E9E0(void);

void ov45_0222F154(void);

typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;

s32 ov45_0222FB24(const UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2);

extern void ov45_02230164(UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 gameKind, s32 param2, const PPW_LobbyRecruitInfo *lobbyRecruitInfo);

extern void ov45_02230384(const PPW_LobbyRecruitInfo *lobbyRecruitInfo, s64 *param1);

void ov45_0222FF7C(s32 param0, const PPW_LobbyRecruitInfo *recruitInfo);

void ov45_0222FF7C(s32 param0, const PPW_LobbyRecruitInfo *recruitInfo) {
    s32 v3 = ov45_0222FB24(_022577C0, param0, 0);

    if (v3 == 0xffffffff) {
        return;
    }

    u32 v0 = ov45_0222E9E0();

    if (v0 != param0) {
        if (_022577C0->unk_69E) {
            if (_022577C0->gameKind == recruitInfo->gameKind) {
                s64 v1, v2;
                ov45_02230384(&_022577C0->unk_5B8[_022577C0->gameKind].lobbyRecruitInfo, &v1);
                ov45_02230384(recruitInfo, &v2);

                if (v1 >= v2) {
                    _022577C0->unk_69F = 1;

                    ov45_0222F154();
                } else {
                    return;
                }
            }
        }
    }

    ov45_02230164(_022577C0, (UnkEnum_ov66_02232F38)(recruitInfo->gameKind), param0, recruitInfo);
}
