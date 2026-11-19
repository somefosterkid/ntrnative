#include "global.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct BgConfig;

struct ColoredArrow;

struct DWCstApInfo;

struct DWCstInetControl;

struct ListMenu;

struct ListMenuTemplate;

struct Menu;

struct MessageLoader;

struct NARC;

struct Options;

struct SaveData;

struct String;

struct StringList;

struct StringTemplate;

struct SysTask;

struct SysTaskManager;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;

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

typedef enum {
    DWC_CONNECTINET_STATE_NOT_INITIALIZED = 0,
    DWC_CONNECTINET_STATE_IDLE,
    DWC_CONNECTINET_STATE_OPERATING,
    DWC_CONNECTINET_STATE_OPERATED,
    DWC_CONNECTINET_STATE_CONNECTED,
    DWC_CONNECTINET_STATE_DISCONNECTING,
    DWC_CONNECTINET_STATE_DISCONNECTED,
    DWC_CONNECTINET_STATE_ERROR,
    DWC_CONNECTINET_STATE_FATAL_ERROR,
    DWC_CONNECTINET_STATE_LAST
} DWCInetResult;

typedef enum {
    DWC_APINFO_AREA_JPN = 0,
    DWC_APINFO_AREA_USA,
    DWC_APINFO_AREA_EUR,
    DWC_APINFO_AREA_AUS,
    DWC_APINFO_AREA_KOR,
    DWC_APINFO_AREA_UNKNOWN = 0xff
} DWCApInfoArea;

typedef enum {
    DWC_APINFO_TYPE_USER0 = 0,
    DWC_APINFO_TYPE_USER1,
    DWC_APINFO_TYPE_USER2,
    DWC_APINFO_TYPE_USB,
    DWC_APINFO_TYPE_SHOP,
    DWC_APINFO_TYPE_FREESPOT,
    DWC_APINFO_TYPE_WAYPORT,
    DWC_APINFO_TYPE_OTHER,
    DWC_APINFO_TYPE_NINTENDOWFC = DWC_APINFO_TYPE_OTHER,
    DWC_APINFO_TYPE_NINTENDOSPOT,
    DWC_APINFO_TYPE_UNKNOWN = 99
} DWCApInfoType;

typedef struct DWCstApInfo {
    DWCApInfoType aptype;
    DWCApInfoArea area;
    char spotinfo[9 + 1];
    char essid[32 + 1];
    u8 bssid[6];
} DWCApInfo;

BOOL DWC_GetApInfo(DWCApInfo *apinfo);

extern BOOL DWC_CheckInet(void);

extern void DWC_ProcessInet(void);

extern DWCInetResult DWC_GetInetStatus(void);

extern void DWC_CleanupInet(void);

typedef enum {
    DWC_ERROR_NONE = 0,
    DWC_ERROR_DS_MEMORY_ANY,
    DWC_ERROR_AUTH_ANY,
    DWC_ERROR_AUTH_OUT_OF_SERVICE,
    DWC_ERROR_AUTH_STOP_SERVICE,
    DWC_ERROR_AC_ANY,
    DWC_ERROR_NETWORK,
    DWC_ERROR_GHTTP_ANY,
    DWC_ERROR_DISCONNECTED,
    DWC_ERROR_FATAL,
    DWC_ERROR_FRIENDS_SHORTAGE,
    DWC_ERROR_NOT_FRIEND_SERVER,
    DWC_ERROR_MO_SC_CONNECT_BLOCK,
    DWC_ERROR_SERVER_FULL,
    DWC_ERROR_ND_ANY,
    DWC_ERROR_ND_HTTP,
    DWC_ERROR_SVL_ANY,
    DWC_ERROR_SVL_HTTP,
    DWC_ERROR_PROF_ANY,
    DWC_ERROR_PROF_HTTP,
    DWC_ERROR_NUM
} DWCError;

typedef enum {
    DWC_ETYPE_NO_ERROR = 0,
    DWC_ETYPE_LIGHT,
    DWC_ETYPE_SHOW_ERROR,
    DWC_ETYPE_SHUTDOWN_FM,
    DWC_ETYPE_SHUTDOWN_GHTTP,
    DWC_ETYPE_SHUTDOWN_ND,
    DWC_ETYPE_DISCONNECT,
    DWC_ETYPE_FATAL,
    DWC_ETYPE_NUM
} DWCErrorType;

extern DWCError DWC_GetLastError(int *errorCode);

extern DWCError DWC_GetLastErrorEx(int *errorCode, DWCErrorType *errorType);

extern void DWC_ClearError(void);

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

typedef struct SaveData SaveData;

typedef struct Options Options;

void Heap_Free(void *ptr);

typedef struct String String;

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct MessageLoader MessageLoader;

typedef struct StringTemplate StringTemplate;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

typedef struct ColoredArrow ColoredArrow;

typedef struct StringList StringList;

typedef struct ListMenu ListMenu;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct ListMenuTemplate {
    const StringList *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;

struct ListMenu {
    ListMenuTemplate template;
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};

typedef struct Menu Menu;

void ov70_02237F2C(void);

typedef struct {
    ApplicationManager *appMan;
    SaveData *saveData;
    Options *options;
    void *unk_0C;
    NNSFndHeapHandle unk_10;
    DWCInetControl unk_14;
    u16 unk_7C;
    u8 unk_7E;
    u8 padding_7F;
    int unk_80;
    int unk_84;
    u8 padding_88[4];
    int unk_8C;
    int unk_90;
    u8 padding_94[4];
    void *unk_98;
    WorldExchangeTrainer unk_9C;
    WorldExchangeTrainerError unk_100;
    u32 unk_108;
    u32 unk_10C;
    u8 unk_110;
    u8 unk_111[3];
    String *unk_114;
    String *unk_118;
    BOOL unk_11C;
} UnkStruct_ov98_02246E88;

void sub_0203957C(void);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void sub_0203A914(void);

void Sys_ClearSleepDisableFlag(int mask);

typedef struct {
    UnkStruct_ov98_02246E88 *unk_00;
    BgConfig *unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    StringTemplate *unk_20;
    MessageLoader *unk_24;
    MessageLoader *unk_28;
    MessageLoader *unk_2C;
    MessageLoader *unk_30;
    MessageLoader *unk_34;
    String *unk_38;
    String *unk_3C;
    String *unk_40;
    int unk_44;
    Window unk_48;
    Window unk_58;
    Window unk_68;
    Window unk_78;
    Menu *unk_88;
    void *unk_8C;
    int unk_90;
    int unk_94;
    int unk_98;
    int unk_9C;
    StringList *unk_A0;
    ListMenu *unk_A4;
    int unk_A8;
    int unk_AC;
    int unk_B0;
    int unk_B4;
    int unk_B8;
    int unk_BC;
    int unk_C0;
    Window unk_C4;
    Window unk_D4;
    ColoredArrow *unk_E4;
    s32 unk_E8;
    UnkStruct_ov61_0222C3B0 unk_EC;
} UnkStruct_ov98_02247704;

void ov75_02247878(UnkStruct_ov98_02247704 *param0);

int ov75_02248800(UnkStruct_ov98_02247704 *param0);

int ov75_02248800(UnkStruct_ov98_02247704 *param0) {
    DWC_ProcessInet();

    if (DWC_CheckInet()) {
        switch (DWC_GetInetStatus()) {
        case DWC_CONNECTINET_STATE_ERROR: {
            DWCError v0;
            int v1;
            DWCErrorType v2;

            v0 = DWC_GetLastErrorEx(&v1, &v2);
            param0->unk_14 = v0;
            param0->unk_18 = v1;
            param0->unk_1C = v2;
        }
            DWC_ClearError();
            DWC_CleanupInet();

            sub_0203A914();
            sub_0203957C();
            Sys_ClearSleepDisableFlag(4);

            if (param0->unk_00->unk_11C == 1) {
                ov70_02237F2C();
                param0->unk_00->unk_11C = 0;
            }

            ov75_02247878(param0);
            param0->unk_08 = 27;
            break;
        case DWC_CONNECTINET_STATE_NOT_INITIALIZED:
        case DWC_CONNECTINET_STATE_IDLE:
        case DWC_CONNECTINET_STATE_OPERATING:
        case DWC_CONNECTINET_STATE_OPERATED:
        case DWC_CONNECTINET_STATE_DISCONNECTING:
        case DWC_CONNECTINET_STATE_DISCONNECTED:
        default:
        case DWC_CONNECTINET_STATE_FATAL_ERROR: {
            DWCError v3;
            int v4;

            v3 = DWC_GetLastError(&v4);

            ov75_02247878(param0);
            param0->unk_08 = 32;
            param0->unk_10 = -2;
        } break;
        case DWC_CONNECTINET_STATE_CONNECTED: {
            DWCApInfo v5;
            DWC_GetApInfo(&v5);

            if (v5.aptype == DWC_APINFO_TYPE_SHOP) {
                (void)0;
            }
        }

            param0->unk_08 = 14;
            break;
        }
    }

    return 0;
}

BOOL DWC_NASLoginAsync(void);

int ov75_022488AC(UnkStruct_ov98_02247704 *param0);

int ov75_022488AC(UnkStruct_ov98_02247704 *param0) {
    DWC_NASLoginAsync();

    param0->unk_08 = 15;
    return 0;
}

typedef enum {
    DWC_NASLOGIN_STATE_DIRTY = 0,
    DWC_NASLOGIN_STATE_IDLE,
    DWC_NASLOGIN_STATE_HTTP,
    DWC_NASLOGIN_STATE_SUCCESS,
    DWC_NASLOGIN_STATE_ERROR,
    DWC_NASLOGIN_STATE_CANCELED,
    DWC_NASLOGIN_STATE_MAX
} DWCNasLoginState;

DWCNasLoginState DWC_NASLoginProcess(void);

BOOL DWC_ShutdownGHTTP(void);

extern void DWC_ShutdownFriendsMatch(void);

void sub_020399EC(void);

int ov75_022488BC(UnkStruct_ov98_02247704 *param0);

int ov75_022488BC(UnkStruct_ov98_02247704 *param0) {
    switch (DWC_NASLoginProcess()) {
    case DWC_NASLOGIN_STATE_SUCCESS:;
        param0->unk_08 = 16;
        break;
    case DWC_NASLOGIN_STATE_ERROR:
    case DWC_NASLOGIN_STATE_CANCELED:
    case DWC_NASLOGIN_STATE_DIRTY:
        ov75_02247878(param0);
        {
            int v0;
            DWCErrorType v1;
            DWCError v2;

            v2 = DWC_GetLastErrorEx(&v0, &v1);
            param0->unk_14 = v2;
            param0->unk_18 = v0;

            DWC_ClearError();
            DWC_CleanupInet();

            sub_0203A914();
            sub_0203957C();
            Sys_ClearSleepDisableFlag(4);

            if (param0->unk_00->unk_11C == 1) {
                ov70_02237F2C();
                param0->unk_00->unk_11C = 0;
            }

            param0->unk_08 = 27;

            switch (v1) {
            case DWC_ETYPE_LIGHT:
            case DWC_ETYPE_SHOW_ERROR:
                param0->unk_08 = 27;
                break;
            case DWC_ETYPE_SHUTDOWN_GHTTP:
                DWC_ShutdownGHTTP();
                param0->unk_08 = 27;
                break;
            case DWC_ETYPE_DISCONNECT:
                param0->unk_08 = 27;
                break;
            case DWC_ETYPE_SHUTDOWN_FM:
                DWC_ShutdownFriendsMatch();
                param0->unk_08 = 27;
                break;
            case DWC_ETYPE_SHUTDOWN_ND:
            case DWC_ETYPE_FATAL:
                sub_020399EC();
                break;
            }

            if ((v0 < -20000) && (v0 >= -29999)) {
                param0->unk_08 = 27;
            }
        }
        break;
    }

    return 0;
}

struct DWCstAccLoginId;

struct DWCstAccUserData;

struct SystemData;

struct WiFiList;

typedef struct DWCstAccLoginId {
    u32 id_data;
    u32 userid_lo32;
    u32 playerid;
} DWCAccLoginId;

typedef struct DWCstAccUserData {
    int size;
    DWCAccLoginId pseudo;
    DWCAccLoginId authentic;
    int gs_profile_id;
    int flag;
    u32 gamecode;
    int reserved[5];
    u32 crc32;
} DWCAccUserData;

typedef DWCAccUserData DWCUserData;

u64 DWC_CreateFriendKey(const DWCAccUserData *userdata);

typedef struct WiFiList WiFiList;

typedef struct SystemData SystemData;

SystemData *Save_SysInfo_Get(SaveData *saveData);

s32 Save_SysInfo_GetDwcProfileId(const SystemData *systemData);

void Save_SysInfo_SetDwcProfileId(SystemData *systemData, s32 profileId);

void ov70_022378C0(s32 dwcProfileId, u64 friendKey);

DWCUserData *sub_0202C08C(WiFiList *wiFiList);

WiFiList *sub_0202C6F4(SaveData *saveData);

int sub_0203A040(WiFiList *wifiList);

int ov75_02248994(UnkStruct_ov98_02247704 *param0);

int ov75_02248994(UnkStruct_ov98_02247704 *param0) {
    DWCUserData *v0;
    s32 v1;
    WiFiList *v3 = sub_0202C6F4(param0->unk_00->saveData);
    SystemData *v2 = Save_SysInfo_Get(param0->unk_00->saveData);
    v0 = sub_0202C08C(v3);
    v1 = Save_SysInfo_GetDwcProfileId(v2);

    if (v1 == 0) {
        Save_SysInfo_SetDwcProfileId(v2, sub_0203A040(v3));
    }

    v1 = Save_SysInfo_GetDwcProfileId(v2);

    ov70_022378C0(v1, DWC_CreateFriendKey(v0));

    param0->unk_00->unk_11C = 1;
    param0->unk_08 = 17;

    return 0;
}

void ov70_02238304(const WorldExchangeTrainer *trainer, WorldExchangeTrainerError *err);

void ov75_02246CA0(UnkStruct_ov98_02246E88 *param0);

void ov75_02246CB8(UnkStruct_ov98_02246E88 *param0);

int ov75_022489F8(UnkStruct_ov98_02247704 *param0);

int ov75_022489F8(UnkStruct_ov98_02247704 *param0) {
    ov75_02246CA0(param0->unk_00);
    ov75_02246CB8(param0->unk_00);
    ov70_02238304(&param0->unk_00->unk_9C, &param0->unk_00->unk_100);

    param0->unk_08 = 18;
    param0->unk_E8 = 0;

    return 0;
}

BOOL ov70_02237F38(void);

s32 ov70_02237F58(void);

int ov75_02248A20(UnkStruct_ov98_02247704 *param0);

int ov75_02248A20(UnkStruct_ov98_02247704 *param0) {
    if (ov70_02237F38()) {
        s32 v0 = ov70_02237F58();

        param0->unk_E8 = 0;

        switch (v0) {
        case 0:
            switch (param0->unk_00->unk_100.validationError) {
            case 0:
                switch (param0->unk_00->unk_100.systemError) {
                case 1:
                    param0->unk_08 = 19;
                    break;
                case 2:
                    ov75_02247878(param0);
                    param0->unk_10 = -5001;
                    param0->unk_08 = 32;
                    break;

                case 0:
                    ov75_02247878(param0);
                    param0->unk_10 = -5002;
                    param0->unk_08 = 32;
                    break;
                case 3:
                    ov75_02247878(param0);
                    param0->unk_10 = -5003;
                    param0->unk_08 = 32;
                    break;
                default:
                    sub_020399EC();
                    break;
                }

                break;
            case 1:
                ov75_02247878(param0);
                param0->unk_10 = -5004;
                param0->unk_08 = 32;
                break;
            case 2:
                ov75_02247878(param0);
                param0->unk_10 = -5005;
                param0->unk_08 = 32;
                break;
            default:
                ov75_02247878(param0);
                sub_020399EC();
                break;
            }

            break;
        case 1:
            ov75_02247878(param0);
            param0->unk_10 = v0;
            param0->unk_08 = 32;
            break;
        case 2:
        case -1:
            ov75_02247878(param0);
            param0->unk_10 = v0;
            param0->unk_08 = 32;
            break;
        case -12:
        case -15:
            ov75_02247878(param0);
            param0->unk_10 = v0;
            param0->unk_08 = 32;
            break;
        case -2:
        case -14:
            ov75_02247878(param0);
            param0->unk_10 = v0;
            param0->unk_08 = 32;
            break;
        case -13:
        default:
            ov75_02247878(param0);
            sub_020399EC();
            break;
        }
    } else {
        param0->unk_E8++;

        if (param0->unk_E8 == (30 * 60 * 2)) {
            sub_020399EC();
        }
    }

    return 0;
}
