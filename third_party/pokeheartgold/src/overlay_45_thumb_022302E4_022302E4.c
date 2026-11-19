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

PPW_LOBBY_RESULT ov45_02232BB0(const PPW_LobbyRecruitInfo *recruitInfo);

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

typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;

extern int ov00_021E7144(void);

extern u16 ov45_0223023C(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1);

void ov45_022302E4(UnkStruct_ov66_022336C4 *unused);

void ov45_022302E4(UnkStruct_ov66_022336C4 *unused) {
    if (_022577C0->unk_69E == 1) {
        u32 numConnections = ov00_021E7144();
        ((numConnections <= 4) ? (void)0 : GF_AssertFail());
        UnkStruct_ov66_022336C4_sub4 *v2 = &_022577C0->unk_5B8[_022577C0->gameKind];

        if (v2->lobbyRecruitInfo.currentNum != numConnections) {
            v2->lobbyRecruitInfo.currentNum = numConnections;
            ov45_02232BB0(&v2->lobbyRecruitInfo);
        }
    }

    u16 v1;

    for (int i = 0; i < UnkEnum_ov66_02232F38_03; i++) {
        if (_022577C0->unk_5B8[i].unk_44 == 1) {
            if (_022577C0->unk_5B8[i].unk_46 > 0) {
                _022577C0->unk_5B8[i].unk_46--;
            }

            v1 = ov45_0223023C((const UnkStruct_ov66_022336C4 *)(_022577C0), (UnkEnum_ov66_02232F38)(i));

            if (v1 < _022577C0->unk_5B8[i].unk_46) {
                _022577C0->unk_5B8[i].unk_46 = v1;
            }
        }
    }
}
