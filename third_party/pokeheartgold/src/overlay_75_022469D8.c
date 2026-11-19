#include "global.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct DWCstInetControl;

struct Options;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct String;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;

typedef enum {
    DWC_WDS_STATE_IDLE = 0,
    DWC_WDS_STATE_PROCESS = 1,
    DWC_WDS_STATE_COMPLETED = 2,
    DWC_WDS_STATE_FAILED = 3,
    DWC_WDS_STATE_ERROR = 4
} DWCWDSState;

typedef struct tagDWCWDSData {
    u8 ssid[32];
    u8 wep[32];
    int wepMode;
    char apnum[10];
} DWCWDSData;

typedef struct DWCstInetControl {
    volatile int ac_state;
    u16 state;
    u16 online;
    u16 dmaNo;
    u16 powerMode;
    u8 wontExecuteWDS;
    DWCWDSState processWDSstate;
    DWCWDSData nspotInfo;

} DWCInetControl;

extern WMLinkLevel DWC_GetLinkLevel(void);

extern BOOL DWC_UpdateConnection(void);

typedef enum {
    DWC_ALLOCTYPE_AUTH,
    DWC_ALLOCTYPE_AC,
    DWC_ALLOCTYPE_BM,
    DWC_ALLOCTYPE_UTIL,
    DWC_ALLOCTYPE_BASE,
    DWC_ALLOCTYPE_GS,
    DWC_ALLOCTYPE_ND,

    DWC_ALLOCTYPE_ENC,
    DWC_ALLOCTYPE_MAIL,

    DWC_ALLOCTYPE_LAST
} DWCAllocType;

typedef void *(*DWCAllocEx)(DWCAllocType name, u32 size, int align);

typedef void (*DWCFreeEx)(DWCAllocType name, void *ptr, u32 size);

void DWC_SetMemFunc(DWCAllocEx alloc, DWCFreeEx free);

typedef struct WorldExchangeTrainer {
    u8 gameCode;
    u8 language;
    u8 country;
    u8 unk_03;
    u32 trainerId;
    u16 trainerName[7 + 1];
    u32 unk_10;
    u8 macAddress[6];
    u8 unk_1A[2];
    char email[56];
    u32 emailInitialised;
    u16 rngValue;
    u16 unk_5A;
} WorldExchangeTrainer;

typedef struct WorldExchangeTrainerError {
    u32 validationError;
    u32 systemError;
} WorldExchangeTrainerError;

enum HeapID {
    HEAP_ID_SYSTEM = 0,
    HEAP_ID_SAVE,
    HEAP_ID_DEBUG,
    HEAP_ID_APPLICATION,
    HEAP_ID_FIELD1,
    HEAP_ID_BATTLE,
    HEAP_ID_BAG,
    HEAP_ID_POKETCH_MAIN,
    HEAP_ID_POKETCH_APP,
    HEAP_ID_BOX_DATA,
    HEAP_ID_BOX_GRAPHICS,
    HEAP_ID_FIELD2,
    HEAP_ID_PARTY_MENU,
    HEAP_ID_13,
    HEAP_ID_14,
    HEAP_ID_COMMUNICATION,
    HEAP_ID_16,
    HEAP_ID_TOWN_MAP,
    HEAP_ID_NAMING_SCREEN_APP,
    HEAP_ID_POKEMON_SUMMARY_SCREEN,
    HEAP_ID_20,
    HEAP_ID_21,
    HEAP_ID_22,
    HEAP_ID_23,
    HEAP_ID_24,
    HEAP_ID_TRAINER_CASE,
    HEAP_ID_TRADE_ROOM,
    HEAP_ID_27,
    HEAP_ID_28,
    HEAP_ID_MINING,
    HEAP_ID_DISTORTION_WORLD_WARP,
    HEAP_ID_31,
    HEAP_ID_FIELD3,
    HEAP_ID_UNDERGROUND,
    HEAP_ID_34,
    HEAP_ID_35,
    HEAP_ID_BERRY_TAG,
    HEAP_ID_POKEDEX,
    HEAP_ID_OPTIONS_MENU,
    HEAP_ID_DRAWING,
    HEAP_ID_MAIL,
    HEAP_ID_MAIL_VIEWER,
    HEAP_ID_JOURNAL,
    HEAP_ID_43,
    HEAP_ID_POFFIN_CASE,
    HEAP_ID_POFFIN_MANAGER,
    HEAP_ID_POFFIN_CUTSCENE,
    HEAP_ID_CHOOSE_STARTER_APP,
    HEAP_ID_48,
    HEAP_ID_NINTENDO_WFC,
    HEAP_ID_50,
    HEAP_ID_51,
    HEAP_ID_52,
    HEAP_ID_53,
    HEAP_ID_54,
    HEAP_ID_POFFIN_BERRY_SELECTION,
    HEAP_ID_56,
    HEAP_ID_TRADE_SEQUENCE,
    HEAP_ID_TRADE_SEQUENCE_PHASE,
    HEAP_ID_59,
    HEAP_ID_60,
    HEAP_ID_PC_HALL_OF_FAME,
    HEAP_ID_62,
    HEAP_ID_HALL_OF_FAME,
    HEAP_ID_64,
    HEAP_ID_65,
    HEAP_ID_66,
    HEAP_ID_MOVE_REMINDER,
    HEAP_ID_68,
    HEAP_ID_69,
    HEAP_ID_70,
    HEAP_ID_EGG_HATCH,
    HEAP_ID_BOAT_CUTSCENE,
    HEAP_ID_EVOLUTION,
    HEAP_ID_BATTLE_TOWER_RECORDS_APP,
    HEAP_ID_END_CREDITS,
    HEAP_ID_76,
    HEAP_ID_GAME_START,
    HEAP_ID_MIGRATE_FROM_GBA,
    HEAP_ID_79,
    HEAP_ID_RANKINGS_MACHINE,
    HEAP_ID_MAIN_MENU,
    HEAP_ID_ROWAN_INTRO,
    HEAP_ID_ROWAN_INTRO_TV,
    HEAP_ID_DIPLOMA,
    HEAP_ID_RANGER_LINK,
    HEAP_ID_MYSTERY_GIFT_APP,
    HEAP_ID_WONDER_CARDS_APP,
    HEAP_ID_88,
    HEAP_ID_89,
    HEAP_ID_90,
    HEAP_ID_NETWORK_ICON,
    HEAP_ID_92,
    HEAP_ID_BATTLE_FACTORY_APP,
    HEAP_ID_94,
    HEAP_ID_95,
    HEAP_ID_96,
    HEAP_ID_97,
    HEAP_ID_BATTLE_HALL_APP,
    HEAP_ID_99,
    HEAP_ID_BATTLE_CASTLE_APP,
    HEAP_ID_101,
    HEAP_ID_102,
    HEAP_ID_BATTLE_ARCADE_APP,
    HEAP_ID_104,
    HEAP_ID_105,
    HEAP_ID_106,
    HEAP_ID_107,
    HEAP_ID_108,
    HEAP_ID_109,
    HEAP_ID_110,
    HEAP_ID_SPEAR_PILLAR_CUTSCENE,
    HEAP_ID_112,
    HEAP_ID_113,
    HEAP_ID_FRONTIER_RECORD_APP,
    HEAP_ID_SCRATCH_OFF_CARD_APP,
    HEAP_ID_116,
    HEAP_ID_117,
    HEAP_ID_118,
    HEAP_ID_119,
    HEAP_ID_LIBRARY_TV,
    HEAP_ID_PARTY_MENU1,
    HEAP_ID_PARTY_MENU2,
    HEAP_ID_MAX,
};

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

typedef struct SaveData SaveData;

typedef struct Options {
    u16 textSpeed : 4;
    u16 soundMode : 2;
    u16 battleStyle : 1;
    u16 battleScene : 1;
    u16 buttonMode : 2;
    u16 frame : 5;
    u16 : 1;
} Options;

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

ApplicationManager *OverlayManager_New(const ApplicationManagerTemplate *template, void *args, const enum HeapID heapID);

void OverlayManager_Delete(ApplicationManager *appMan);

void *OverlayManager_GetData(ApplicationManager *appMan, int *);

BOOL OverlayManager_Run(ApplicationManager *appMan);

typedef u16 charcode_t;

typedef struct String {
    u16 maxsize;
    u16 size;
    u32 magic;
    u16 data[0];
} String;

void ov70_022378DC(void);

int ov65_0222E2A8(ApplicationManager *appMan, int *param1);

int ov65_0222E3FC(ApplicationManager *appMan, int *param1);

int ov65_0222E548(ApplicationManager *appMan, int *param1);

int ov98_022471C8(ApplicationManager *appMan, int *param1);

int ov39_02228308(ApplicationManager *appMan, int *param1);

int ov98_02247440(ApplicationManager *appMan, int *param1);

void sub_0203A930(int networkStrength);

BOOL sub_02034DB8(void);

FS_EXTERN_OVERLAY(overlay65);

typedef void *(*UnkFuncPtr_ov98_02249B4C)(UnkStruct_ov98_02246E88 *);

typedef void (*UnkFuncPtr_ov98_02249B4C_1)(UnkStruct_ov98_02246E88 *);

void ov75_02246B48(UnkStruct_ov98_02246E88 *param0);

void ov75_02246B98(UnkStruct_ov98_02246E88 *param0);

void *ov75_02246BF0(DWCAllocType param0, u32 param1, int param2);

void ov75_02246C18(DWCAllocType param0, void *param1, u32 param2);

void *ov98_02246FBC(UnkStruct_ov98_02246E88 *param0);

void ov98_02246FC0(UnkStruct_ov98_02246E88 *param0);

void *ov98_02247168(UnkStruct_ov98_02246E88 *param0);

void ov98_02247198(UnkStruct_ov98_02246E88 *param0);

void *ov98_02246FC4(UnkStruct_ov98_02246E88 *param0);

void ov98_02246FFC(UnkStruct_ov98_02246E88 *param0);

void *ov98_02247070(UnkStruct_ov98_02246E88 *param0);

void ov98_022470B8(UnkStruct_ov98_02246E88 *param0);

void *ov98_022470F8(UnkStruct_ov98_02246E88 *param0);

void ov98_02247134(UnkStruct_ov98_02246E88 *param0);

extern NNSFndHeapHandle _02249BE0;

typedef struct UnkStruct_ov75_02249904 {
    UnkFuncPtr_ov98_02249B4C unk_00;
    UnkFuncPtr_ov98_02249B4C_1 unk_04;
    const ApplicationManagerTemplate *unk_08;
    int unk_0C;
} UnkStruct_ov75_02249904;

extern const UnkStruct_ov75_02249904 ov75_02249904[6];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern const ApplicationManagerTemplate _02102620;

void *ov75_02246D00(UnkStruct_ov98_02246E88 *param0);
void ov75_02246D04(UnkStruct_ov98_02246E88 *param0);
void *ov75_02246D08(UnkStruct_ov98_02246E88 *param0);
void ov75_02246D40(UnkStruct_ov98_02246E88 *param0);
void *ov75_02246DB4(UnkStruct_ov98_02246E88 *param0);
void ov75_02246DFC(UnkStruct_ov98_02246E88 *param0);
void *ov75_02246E3C(UnkStruct_ov98_02246E88 *param0);
void ov75_02246E78(UnkStruct_ov98_02246E88 *param0);
void *ov75_02246EAC(UnkStruct_ov98_02246E88 *param0);
void ov75_02246EDC(UnkStruct_ov98_02246E88 *param0);
BOOL ov75_02246F0C(ApplicationManager *appMan, int *state);
BOOL ov75_02247118(ApplicationManager *appMan, int *state);
BOOL ov75_02247180(ApplicationManager *appMan, int *state);
BOOL ov44_0222A4B4(ApplicationManager *appMan, int *state);
BOOL ov44_0222A60C(ApplicationManager *appMan, int *state);
BOOL ov44_0222A758(ApplicationManager *appMan, int *state);

static const ApplicationManagerTemplate ov75_022498E4 = { ov44_0222A4B4, ov44_0222A60C, ov44_0222A758, 44 };
static const ApplicationManagerTemplate ov75_022498F4 = { ov75_02246F0C, ov75_02247118, ov75_02247180, (FSOverlayID)-1 };

const UnkStruct_ov75_02249904 ov75_02249904[6] = {
    { ov75_02246D00, ov75_02246D04, &ov75_022498F4, 0 },
    { ov75_02246D08, ov75_02246D40, &_02102620, 0 },
    { ov75_02246DB4, ov75_02246DFC, &_02102620, 0 },
    { ov75_02246E3C, ov75_02246E78, &_02102620, 0 },
    { ov75_02246E3C, ov75_02246E78, &_02102620, 0 },
    { ov75_02246EAC, ov75_02246EDC, &ov75_022498E4, 1 },
};
#endif

int ov75_022469D8(ApplicationManager *appMan, int *param1);

int ov75_022469D8(ApplicationManager *appMan, int *param1) {
    UnkStruct_ov98_02246E88 *v0 = OverlayManager_GetData(appMan, param1);

    if (v0->unk_80 == 1) {
        DWC_UpdateConnection();
        ov70_022378DC();
        sub_0203A930(WM_LINK_LEVEL_3 - DWC_GetLinkLevel());
    }

    switch (*param1) {
    case 0:
        ov75_02246B48(v0);
        *param1 = 1;
        break;
    case 1:
        if (sub_02034DB8()) {
            _02249BE0 = v0->unk_10;
            DWC_SetMemFunc(ov75_02246BF0, ov75_02246C18);
            v0->unk_80 = 1;
            *param1 = 2;
        }
        break;
    case 2:
        v0->appMan = OverlayManager_New(ov75_02249904[v0->unk_8C].unk_08, ov75_02249904[v0->unk_8C].unk_00(v0), (enum HeapID)115);
        v0->unk_84 = v0->unk_8C;
        v0->unk_8C = 6;
        *param1 = 3;
        break;
    case 3:
        if (OverlayManager_Run(v0->appMan) == 1) {
            ov75_02249904[v0->unk_84].unk_04(v0);
            OverlayManager_Delete(v0->appMan);

            if (v0->unk_8C == 6) {
                *param1 = 4;
            } else {
                if (ov75_02249904[v0->unk_8C].unk_0C == 1) {
                    ov75_02246B98(v0);
                    *param1 = 2;
                } else if (v0->unk_80 == 1) {
                    *param1 = 2;
                } else {
                    *param1 = 0;
                }
            }
        }
        break;
    case 4:
        return 1;
    }

    return 0;
}
