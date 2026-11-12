// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov260/wifibattlematch_net.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/wifibattlematch_net.h"
#include "types.h"
#include "app/livebattlematch_flow.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "dwc/dwc.h"
#include "dwc/gdb.h"
#include "dwc/nd.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/nhttp_rap.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/game_data.h"
#include "text/system/ability_handlers_btl_main.h"
#include "dwc/auth.h"
#include "dwc/competition.h"
#include "dwc/nhttp.h"
#include "dwc/sc.h"

// wifibattlematch_net.c, named by its embedded string, in overlay 260: the net part of the Wi-Fi battle competitions.
// It matches two players through the Wi-Fi Connection, keeps their stats and teams in the Sake database through the
// Gdb client that follows it in the overlay (dwc/gdb.h), sends the teams from one to the other and downloads the
// competitions' files. The work is called p_wk, as its assertions' texts show

// The most fields that a request on a table can have
#define WBM_GDB_FIELD_TABLE_MAX 0x26

// The table that the players' stats are in
#define WBM_GDB_TABLE_NAME "PlayerStats_v1"

// The kinds of the error that the work keeps
enum {
    WBM_ERROR_NONE,
    WBM_ERROR_DWC,
    WBM_ERROR_GDB,
    WBM_ERROR_UNUSED_3,
    WBM_ERROR_HTTP,
    WBM_ERROR_TIMEOUT,
};

// How an error ends for the player: nothing, the Wi-Fi connection lost, the competition failed or the game's network
// error. They are what func_ov009_02157170 and the error converters answer
typedef enum {
    WBM_RESULT_OK,
    WBM_RESULT_DISCONNECTED,
    WBM_RESULT_FAILED,
    WBM_RESULT_NET_ERROR,
    WBM_RESULT_TIMEOUT,
} WifiBattleMatchNetResult;

// The error that was found, by its kind
typedef struct {
    s32 dwcCode;
    s32 gdbCode;
    s32 gdbSubCode1;
    s32 gdbSubCode2;
    u32 unk10;
    s32 httpCode;
    s32 timeoutCode;
    u32 kind;
} WifiBattleMatchNetError;

// A key of the matching query: its value, the ID the DWC library gave it and its three letters
typedef struct {
    s32 value;
    u8 keyId;
    u8 unk5;
    char name[6];
} WifiBattleMatchNetKey;

// What a player offers in a match, which is sent to the other player whole
typedef struct {
    const AuthCertificate *certificate;
    ScReport *connection;
    u8 unk8[0x28];
    u8 unk30[0x28];
    u8 unk58[0x10];
} WifiBattleMatchNetInfo;

// What overlay 290 shares with this file: the player's ID in the Sake database, the player's side and the other
// player's
typedef struct {
    s32 sake_recordID;
    WifiBattleMatchNetInfo mine;
    WifiBattleMatchNetInfo theirs;
} WifiBattleMatchNetData;

// The kind of a match (0 to 4), how it ended (0 to 6) and the two sides' counts
typedef struct {
    u32 format;
    u32 outcome;
    u32 mine;
    u32 theirs;
} WifiBattleMatchNetParam;

// Sets a battle's fields in the exchange with the other player
typedef int (*WifiBattleMatchNetResultFunc)(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                            int a3);

// Two values that the server answers with, and that are sent back
typedef struct {
    u32 value1;
    u32 value2;
} WifiBattleMatchNetValues;

// What an NHTTP answer starts with: its result, and the two values at its end
typedef struct {
    u32 result;
    u8 unk4[0x7c];
    WifiBattleMatchNetValues values;
} WifiBattleMatchNetAnswer;

// What the download's callback sets, which is in the .bss and not in the work
typedef struct {
    u8 done;
    u8 done2;
    u32 code;
} WifiBattleMatchNetDownload;

// The name that the bad word check reads and its result
typedef struct {
    u16 name[12];
    const u16 *words;
    u8 result;
    int badWordCount;
    u16 unk24;
} WifiBattleMatchNetNameCheck;

// The value of the matching key "ver", which only players of the same version match with
#ifdef BLACK2
#define WBM_MATCH_VERSION 0x20017
#elif GAME_VERSION == VERSION_WHITE
#define WBM_MATCH_VERSION 0x20014
#else
#define WBM_MATCH_VERSION 0x20015
#endif

// The name that a player gets when the check refuses theirs, by the version
#define WBM_DEFAULT_NAME AbilityHandlersBtlMain_Text_VersionName

struct WifiBattleMatchNet {
    void *unk0;
    WifiList *wifiList;
    GameData *gameData;
    s32 profileId;
    void *loginBuffer;
    u32 syncId;
    WifiBattleMatchNetError error;
    u8 loginDate[6];
    u32 matchState;
    char matchQuery[0x80];
    WifiBattleMatchNetKey matchKeys[8];
    HeapID msgHeapId;
    u32 phase;
    u32 state;
    u32 resumeState;
    u32 counter;
    u32 flagInfoReceived;
    u32 flagPlayerInfoReceived;
    u32 flag140;
    u32 flagPartyReceived;
    u32 flag148;
    u32 flagValueReceived;
    u32 receivedValue;
    u32 phaseB;
    WifiBattleMatchNetResultFunc resultFunc;
    s32 errorCode;
    u32 flag164;
    u32 requestSent;
    u32 flag16C;
    u32 flag170;
    u32 gdbOpen;
    u32 unk178;
    void *unk17c;
    GdbRecordsFunc recordsFunc;
    WifiBattleMatchNetParam param;
    GdbField *record;
    const char **tableNames;
    s32 table_name_num;
    u32 unk1a0;
    u32 timer;
    u32 matchTimer;
    u32 unk1ac;
    u32 mode;
    s32 recordIdArg;
    u8 unk1b8[0x10];
    WifiBattleMatchNetRecordData recordData;
    u8 recvBuffer[0x1000];
    DWCEvalPlayerCallback evalFunc;
    NHttpRap *p_nhttp;
    WifiBattleMatchNetValues httpValues;
    WifiBattleMatchNetValues postData;
    u8 downloadBuffer[0x160];
    DWCNdFileInfo downloadFile;
    u32 downloadPercent;
    u32 downloadReceived;
    u32 downloadSize;
    char downloadAttr[0xc];
    u32 downloading;
    u32 unk145c;
    u32 unk1460;
    WifiBattleMatchNetData *p_data;
    WifiBattleMatchNetNameCheck nameCheck;
    PlayerInfo *namePlayerInfo;
    u32 unk1494;
    u32 unk1498;
};

static void func_ov175_021da470(u8 *dest);
void func_ov175_021da4b8(WifiBattleMatchNet *p_wk, u32 index, s32 value);
int func_ov175_021da4e8(int index, void *work);
int func_ov175_021da4f0(int index, void *work);
static int func_ov175_021da4f8(int index, void *work);
static void func_ov175_021dadb4(int error, void *work);
static void func_ov175_021dadcc(int error, void *work);
static void func_ov175_021dae00(int error, void *work);
static void func_ov175_021dae18(WifiBattleMatchNet *p_wk);
void func_ov114_021b9774(NHttpRap *rap);
void func_ov114_021b9838(NHttpRap *rap);
int func_ov175_021df23c(int a0, int a1, int a2, ScReport **connection);
int func_ov175_021df298(ScReport *connection);
int func_ov175_021df2c8(ScReport *connection);
int func_ov175_021df2f8(ScReport *connection);
int func_ov175_021df328(ScReport *connection);
int func_ov175_021df358(ScReport *connection, int index, u8 *unk30, int a3, int mode, const AuthCertificate *certificate, u8 *unk58);
int func_ov175_021df3b0(ScReport *connection, int a1, int a2);
void func_ov175_021dbfa4(WifiBattleMatchNet *p_wk);
static int func_ov175_021dae54(WifiBattleMatchNetInfo *info, WifiBattleMatchNetInfo *theirs, WifiBattleMatchNetParam *param, int a3, WifiBattleMatchNetResultFunc func, int a5, int mode, int a7);
static int func_ov175_021dafb8(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                              int a3);
static int func_ov175_021db160(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param,
                                                 int isMine, int a3);
static int func_ov175_021db218(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                        int a3);
static int func_ov175_021db228(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                        int a3);
static int func_ov175_021db238(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                            int a3);
static int func_ov175_021db248(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                            int a3);
void func_ov175_021db258(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov175_021db294(int netId, int size, const void *data, void *work,
                                                    NetHandle *handle);
void *func_ov175_021db2d4(int netId, void *work, int size);
void func_ov175_021db300(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov175_021db328(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov175_021db350(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov175_021db7c0(int recordNum, GdbField **records, void *work, int fieldNum);
void func_ov175_021dba00(int recordNum, GdbField **records, void *work, int fieldNum);
void func_ov175_021dbb00(int recordNum, GdbField **records, void *work, int fieldNum);
void func_ov175_021dbb54(int recordNum, GdbField **records, void *work, int fieldNum);
void func_ov175_021dbbac(int recordNum, GdbField **records, void *work, int fieldNum);
static BOOL func_ov175_021dc5ac(const u32 *data, u32 size);
void func_ov175_021dc5c8(void *work, int code, int type, int error);
u32 func_ov175_021dc5f8(void *work, int a1, int event, int a3);
u32 func_ov175_021dc610(u32 result);
static void func_ov175_021dc680(WifiBattleMatchNet *p_wk, u32 nextState);
static void func_ov175_021dc6a4(WifiBattleMatchNet *p_wk, u32 nextState);
void func_ov175_021dc6d0(WifiBattleMatchNet *p_wk, u32 nextState);
static void func_ov175_021dc6fc(u32 reason, u32 error, int serverError);
static u32 func_ov175_021dce60(u32 code);
static u32 func_ov175_021dcee0(u32 code, u32 subCode1, u32 subCode2);
static u32 func_ov175_021dcfdc(u32 code);
static void func_ov175_021dcffc(WifiBattleMatchNetError *error);
static void func_ov175_021dce08(WifiBattleMatchNetError *error, int code, int line);
static void func_ov175_021dce24(WifiBattleMatchNetError *error, int code, int line);
void func_ov175_021dce40(WifiBattleMatchNetError *error, int code, int line);
void func_ov175_021dce48(WifiBattleMatchNetError *error, int code, int line);
void func_ov175_021dce50(WifiBattleMatchNetError *error, int code, int line);
void func_ov175_021dce58(WifiBattleMatchNetError *error, int code, int line);

// The commands that the two players send each other
// pokebw2's sCmd2402Data
static const u32 data_ov175_021e2c8c = 0;

void func_ov175_021db378(int netId, int size, const void *data, void *work, NetHandle *handle);
static const NetCommand sNetCommands[] = {
    { func_ov175_021db258, NULL },
    { func_ov175_021db294, func_ov175_021db2d4 },
    { func_ov175_021db300, NULL },
    { func_ov175_021db328, func_ov175_021db2d4 },
    { func_ov175_021db350, func_ov175_021db2d4 },
    { func_ov175_021db378, NULL },
};

static char sVer[] = "ver";
static char sDis[] = "dis";
static const char *sWifiCupPartyTable = "WIFICUP_POKEMON_PARTY";
static const char *sLastLoginTable = "LAST_LOGIN_DATETIME";
static const char *sRecordIdTable = "recordid";
// The names of the matching keys, by the number the work gives them
static const char *sMatchKeyNames[8] = { "mod", "rul", "deb", "rat", "cup", sDis, "cnt", sVer };

// pokebw2's sDownload
static WifiBattleMatchNetDownload data_ov175_021e4b80;

// pokebw2's WifiBattleMatchNet_Delete

void func_ov175_021db72c(int recordNum, GdbField **records, void *work, int fieldNum);
BOOL func_ov114_021b95d4(u32 a0, NHttpRap *a1);

WifiBattleMatchNet *func_ov175_021d9dc8(void *p_data, GameData *gameData, void *loginBuffer, HeapID heapId) {
    WifiBattleMatchNet *p_wk;
    WifiList *wifiList;
    PlayerInfo *info;

    p_wk = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchNet), FALSE, "wifibattlematch_net.c", 688);
    MI_CpuFill8(p_wk, 0, sizeof(WifiBattleMatchNet));
    wifiList = func_0201283c(gameData);
    p_wk->wifiList = wifiList;
    p_wk->unk0 = func_02009860(wifiList);
    info = GetGameDataPlayerInfo(gameData);
    p_wk->profileId = func_0200853c(info);
    p_wk->gameData = gameData;
    p_wk->loginBuffer = loginBuffer;
    p_wk->p_data = p_data;
    p_wk->unk1a0 = 0x573;
    p_wk->record = GFL_HeapAllocate(heapId, sizeof(GdbField) * WBM_GDB_FIELD_TABLE_MAX, FALSE, "wifibattlematch_net.c", 704);
    MI_CpuFill8(p_wk->record, 0, sizeof(GdbField) * WBM_GDB_FIELD_TABLE_MAX);
    if (!func_0203cd98()) {
        GFL_ASSERT(0);
    }
    func_0203a574(0x2400, sNetCommands, 6, p_wk);
    func_0203d644(0);
    func_ov009_02156dc4(func_ov175_021dc5c8, p_wk);
    func_ov009_02156de0(func_ov175_021dc5f8, p_wk);
    return p_wk;
}

void func_ov175_021d9ea0(WifiBattleMatchNet *p_wk) {
    func_ov009_02156de0(NULL, NULL);
    func_ov009_02156dc4(NULL, NULL);
    func_0203a5e4(0x2400);
    GFL_HeapFree(p_wk->record);
    GFL_HeapFree(p_wk);
}

// pokebw2's WifiBattleMatchNet_Nop
void func_ov175_021d9ed0(WifiBattleMatchNet *p_wk) {
}

// pokebw2's WifiBattleMatchNet_StartSync
void func_ov175_021d9ed4(WifiBattleMatchNet *p_wk, u32 syncId) {
    p_wk->syncId = syncId;
    func_02039c68(func_020399a8(), (u8)p_wk->syncId, 0x24);
}

// pokebw2's WifiBattleMatchNet_CheckSync
BOOL func_ov175_021d9eec(WifiBattleMatchNet *p_wk) {
    return func_02039cb4(func_020399a8(), (u8)p_wk->syncId, 0x24);
}

// pokebw2's WifiBattleMatchNet_CheckError
u32 func_ov175_021d9f04(WifiBattleMatchNet *p_wk, u32 a1, u32 a2) {
    WifiBattleMatchNetError *error = &p_wk->error;
    WifiBattleMatchNetResult result = WBM_RESULT_OK;

    if (func_0203cd98()) {
        if (func_0200fb24()) {
            func_0203d7b0(0);
            func_0203d7bc(0);
            GFL_SndStop();
            if (GameData_CheckEventsPaused(p_wk->gameData)) {
                func_02012e08(p_wk->gameData);
                func_0200f7c0(0x29);
            }
        }
        switch (func_ov009_02157170(a1, a2)) {
        case 0:
            result = WBM_RESULT_OK;
            break;
        case 1:
            result = WBM_RESULT_DISCONNECTED;
            break;
        case 2:
            result = WBM_RESULT_FAILED;
            break;
        case 3:
            result = WBM_RESULT_NET_ERROR;
            break;
        case 4:
            result = WBM_RESULT_TIMEOUT;
            break;
        }
        return result;
    }
    if (result == 0) {
        switch (error->kind) {
        case WBM_ERROR_NONE:
            result = WBM_RESULT_OK;
            break;
        case WBM_ERROR_DWC:
            result = func_ov175_021dce60(error->dwcCode);
            break;
        case WBM_ERROR_GDB:
            result = func_ov175_021dcee0(error->gdbCode, error->gdbSubCode1, error->gdbSubCode2);
            break;
        case WBM_ERROR_UNUSED_3:
        case WBM_ERROR_HTTP:
            result = WBM_RESULT_OK;
            break;
        case WBM_ERROR_TIMEOUT:
            result = func_ov175_021dcfdc(error->timeoutCode);
            break;
        default:
            result = 0;
            break;
        }
        if (result != 0) {
            func_0203d7b0(0);
            func_0203d7bc(0);
            GFL_SndStop();
            if (GameData_CheckEventsPaused(p_wk->gameData)) {
                func_02012e08(p_wk->gameData);
                func_0200f7c0(0x29);
            }
        }
        switch (result) {
        case WBM_RESULT_DISCONNECTED:
            func_0203c968();
            func_0203ca04();
            func_0200f83c();
            func_0200fba0();
            break;
        case WBM_RESULT_FAILED:
            if (!func_0200fb24()) {
                func_0203c9ac(0, 0, 0, 0x3f7);
                func_0200faac();
            }
            func_0200fb38();
            break;
        case WBM_RESULT_NET_ERROR:
            GFL_NetErrAbort();
            break;
        }
        if (result != 0) {
            func_ov175_021dcffc(error);
        }
    }
    return result;
}

// pokebw2's WifiBattleMatchNet_ResetLogin
void func_ov175_021da044(WifiBattleMatchNet *p_wk) {
    p_wk->state = 0;
    p_wk->phase = 0;
}

u32 func_ov175_021da054(WifiBattleMatchNet *p_wk) {
    switch (p_wk->phase) {
    case 0:
        func_ov175_021db3a4(p_wk, -1, WBM_READ_RECORD_ID, p_wk->p_data);
        p_wk->phase = 1;
        break;
    case 1: {
        u32 result = func_ov175_021db478(p_wk);
        if (result == 1) {
            if (p_wk->p_data->sake_recordID != 0) {
                p_wk->phase = 4;
            } else {
                p_wk->phase = 2;
            }
        } else if (result == 2) {
            return 2;
        }
        break;
    }
    case 2:
        func_ov175_021dbfa4(p_wk);
        p_wk->phase = 3;
        break;
    case 3: {
        u32 result = func_ov175_021dbfb4(p_wk);
        if (result == 1) {
            p_wk->phase = 4;
        } else if (result == 2) {
            return 2;
        }
        break;
    }
    case 4:
        return 1;
    }
    return 0;
}

void func_ov175_021da0d4(WifiBattleMatchNet *p_wk, u32 mode, u32 a2, u32 a3, const WifiBattleMatchNetKeys *keys) {
    u32 modValue = mode;

    if (mode == 1) {
        modValue += a2;
    }
    if (!func_0203cd98()) {
        GFL_NetErrAbort();
    }
    DWC_CloseAllConnectionsHard();
    func_ov175_021da4b8(p_wk, 0, modValue);
    func_ov175_021da4b8(p_wk, 1, a3);
    func_ov175_021da4b8(p_wk, 2, 1);
    func_ov175_021da4b8(p_wk, 3, keys->rating);
    func_ov175_021da4b8(p_wk, 5, keys->disconnects);
    func_ov175_021da4b8(p_wk, 4, keys->cup);
    func_ov175_021da4b8(p_wk, 6, keys->matches);
    func_ov175_021da4b8(p_wk, 7, WBM_MATCH_VERSION);
    STD_TSPrintf(p_wk->matchQuery, "mod=%d And rul=%d And deb=%d", modValue, a3, 1);
    p_wk->matchState = 0;
    p_wk->timer = 0;
    p_wk->matchTimer = 0;
    MI_CpuFill8(&p_wk->flagInfoReceived, 0, 6 * sizeof(u32));
    switch (mode) {
    case 0:
        p_wk->evalFunc = func_ov175_021da4f8;
        break;
    case 1:
        GFL_ASSERT(0);
        break;
    case 2:
        p_wk->evalFunc = func_ov175_021da4e8;
        break;
    case 3:
        p_wk->evalFunc = func_ov175_021da4f0;
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
}


// pokebw2's WifiBattleMatchNet_MatchMain
u32 func_ov175_021da1e4(WifiBattleMatchNet *p_wk) {
    switch (p_wk->matchState) {
    case 0:
        if (func_ov009_0215580c(p_wk->matchQuery, 2, p_wk->evalFunc, p_wk)) {
            func_0203c304();
            func_ov009_02156bc4(0);
            p_wk->matchState = 1;
        } else {
            func_0203c554(TRUE);
            return 2;
        }
        break;
    case 1:
        p_wk->matchState = 2;
        break;
    case 2:
        switch (func_0203c3ec()) {
        case 0: {
            GFLNetErrorInfo *info = func_0203ca98();

            if ((u32)(info->type - 0x3f0) <= 1) {
                func_ov175_021da384(p_wk);
                p_wk->matchState = 7;
            }
            break;
        }
        case 1:
            p_wk->matchTimer = 0;
            p_wk->matchState = 3;
            func_0203d7b0(1);
            func_0203d7bc(1);
            break;
        default:
            func_ov175_021da384(p_wk);
            p_wk->matchState = 7;
            break;
        }
        break;
    case 3:
        if (func_02039338()) {
            if (func_02039acc()) {
                p_wk->matchState = 4;
            }
            if (p_wk->matchTimer++ > 3600) {
                func_ov175_021da384(p_wk);
                p_wk->matchState = 7;
            }
        } else {
            p_wk->matchTimer = 0;
            p_wk->matchState = 5;
        }
        break;
    case 4:
        func_02039c68(func_020399a8(), 0xf, 0x24);
        p_wk->matchTimer = 0;
        p_wk->matchState = 6;
        break;
    case 5:
        if (p_wk->matchTimer++ > 3600) {
            func_ov175_021da384(p_wk);
            p_wk->matchState = 7;
        } else if (func_020399ec() && func_02039acc()) {
            func_02039c68(func_020399a8(), 0xf, 0x24);
            p_wk->matchTimer = 0;
            p_wk->matchState = 6;
        }
        break;
    case 6:
        if (p_wk->matchTimer++ > 3600) {
            func_ov175_021da384(p_wk);
            p_wk->matchState = 7;
        } else if (func_02039cb4(func_020399a8(), 0xf, 0x24)) {
            p_wk->matchState = 8;
        }
        break;
    case 7:
        if (func_ov175_021da390(p_wk)) {
            GFLNetErrorInfo *info = func_0203ca98();

            if ((u32)(info->type - 0x3f0) <= 1) {
                func_0203ca04();
                func_0203c968();
                func_0200fba0();
                func_0203c554(TRUE);
            }
            return 2;
        }
        break;
    case 8:
        func_0203d378();
        return 1;
    }
    return 0;
}

// pokebw2's WifiBattleMatchNet_CancelMatch
void func_ov175_021da36c(WifiBattleMatchNet *p_wk) {
    func_0203d7b0(0);
    func_0203d7bc(0);
    func_0203c554(TRUE);
}

// pokebw2's WifiBattleMatchNet_ResetEnd
void func_ov175_021da384(WifiBattleMatchNet *p_wk) {
    p_wk->phaseB = 0;
}

// pokebw2's WifiBattleMatchNet_EndMain
BOOL func_ov175_021da390(WifiBattleMatchNet *p_wk) {
    switch (p_wk->phaseB) {
    case 0:
        func_ov175_021d9ed4(p_wk, 0x18);
        p_wk->timer = 0;
        p_wk->phaseB++;
        break;
    case 1:
        if (func_ov175_021d9eec(p_wk)) {
            p_wk->phaseB++;
        } else if (p_wk->timer++ > 600) {
            p_wk->phaseB++;
        }
        break;
    case 2:
        func_0203d7b0(0);
        func_0203d7bc(0);
        p_wk->counter = 0;
        p_wk->phaseB++;
        break;
    case 3:
        if (p_wk->counter++ > 30) {
            p_wk->phaseB++;
        }
        break;
    case 4:
        func_0203c554(TRUE);
        p_wk->phaseB++;
        break;
    case 5:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiBattleMatchNet_SendCmd2402
BOOL func_ov175_021da434(WifiBattleMatchNet *p_wk) {
    if (func_ov009_02156cd0()) {
        return func_0203d3a8(func_020399a8(), 0x2402, 4, &data_ov175_021e2c8c);
    }
    return TRUE;
}

// pokebw2's WifiBattleMatchNet_TakeCmd2402
BOOL func_ov175_021da45c(WifiBattleMatchNet *p_wk) {
    u32 flag = p_wk->flag140;

    if (flag != 0) {
        p_wk->flag140 = 0;
    }
    return flag;
}

// pokebw2's WifiBattleMatchNet_GetLoginDate
static void func_ov175_021da470(u8 *dest) {
    RTCDate date;
    RTCTime time;

    if (!func_0203cd98()) {
        GFL_NetErrAbort();
    }
    if (!DWC_GetDateTime(&date, &time)) {
        GFL_NetErrAbort();
    }
    MI_CpuFill8(dest, 0, 6);
    dest[0] = date.year;
    dest[1] = date.month;
    dest[2] = date.day;
    dest[3] = time.hour;
    dest[4] = time.minute;
    dest[5] = time.second;
}

// pokebw2's WifiBattleMatchNet_SetMatchKey
void func_ov175_021da4b8(WifiBattleMatchNet *p_wk, u32 index, s32 value) {
    WifiBattleMatchNetKey *key;

    p_wk->matchKeys[index].value = value;
    key = &p_wk->matchKeys[index];
    MI_CpuCopy8(sMatchKeyNames[index], key->name, 3);
    key->name[3] = '\0';
    key->keyId = DWC_AddMatchKeyInt(key->keyId, key->name, &key->value);
}

// pokebw2's WifiBattleMatchNet_EvalAlias
int func_ov175_021da4e8(int index, void *work) {
    return func_ov175_021da4f8(index, work);
}

// pokebw2's WifiBattleMatchNet_EvalAny
int func_ov175_021da4f0(int index, void *work) {
    return 1000;
}

// pokebw2's WifiBattleMatchNet_Eval
static int func_ov175_021da4f8(int index, void *work) {
    WifiBattleMatchNet *p_wk = work;
    int disconnects;
    int rating;
    int matches;
    int cup;
    int ratio;
    int myRatio;
    int ratingGap;
    int ratioGap;
    int score;

    rating = DWC_GetMatchIntValue(index, p_wk->matchKeys[3].name, -1);
    disconnects = DWC_GetMatchIntValue(index, p_wk->matchKeys[5].name, -1);
    matches = DWC_GetMatchIntValue(index, p_wk->matchKeys[6].name, -1);
    cup = DWC_GetMatchIntValue(index, p_wk->matchKeys[4].name, -1);

    if (rating == -1 && disconnects == -1 && cup == -1 && matches == -1) {
        return 0;
    }
    if (cup == p_wk->matchKeys[4].value) {
        ratio = disconnects * 50 / (matches + 5);
        myRatio = p_wk->matchKeys[5].value * 50 / (p_wk->matchKeys[6].value + 5);
        ratingGap = MATH_ABS(rating - p_wk->matchKeys[3].value);
        ratioGap = MATH_ABS(ratio - myRatio);
        score = 1000 - ratingGap - ratioGap * 32;
        if (score < 0) {
            return 0;
        }
        return score;
    }
    return 0;
}

// pokebw2's WifiBattleMatchNet_StartReport
void func_ov175_021da5b0(WifiBattleMatchNet *p_wk, u32 mode, u32 kind, u32 a3, const u32 *a4, u32 a5) {
    p_wk->unk1494 = 0;
    p_wk->state = 0;
    p_wk->unk1ac = 1;
    p_wk->flagValueReceived = 0;
    p_wk->flagInfoReceived = 0;
    p_wk->mode = mode;
    p_wk->flag164 = a5;
    p_wk->requestSent = 0;
    p_wk->flag16C = 0;
    p_wk->flag170 = 0;
    if (mode == 0) {
        switch (kind) {
        case 0:
            p_wk->resultFunc = func_ov175_021db228;
            break;
        case 2:
            p_wk->resultFunc = func_ov175_021db218;
            break;
        default:
            GFL_ASSERT(0);
            break;
        }
    } else if (mode == 1) {
        if (a5 != 0) {
            switch (kind) {
            case 0:
                p_wk->resultFunc = func_ov175_021db248;
                break;
            case 2:
                p_wk->resultFunc = func_ov175_021db238;
                break;
            default:
                GFL_ASSERT(0);
                break;
            }
        } else {
            func_0203d378();
            p_wk->param.format = a3;
            p_wk->param.outcome = a4[0];
            p_wk->param.mine = a4[4];
            switch (kind) {
            case 0:
                p_wk->resultFunc = func_ov175_021db160;
                break;
            case 2:
                p_wk->resultFunc = func_ov175_021dafb8;
                break;
            default:
                GFL_ASSERT(0);
                break;
            }
        }
    }
}

// pokebw2's WifiBattleMatchNet_ReportMain
u32 func_ov175_021da6b8(WifiBattleMatchNet *p_wk, u32 *out) {
    GFLNetErrorInfo *netError;
    int err;
    u8 *dst;
    char *src;

    if (out != NULL) {
        *out = p_wk->flag170;
    }
    netError = func_0203ca98();
    if ((u32)(netError->type - 0x3f0) <= 1) {
        p_wk->flag164 = TRUE;
    }
    if (func_0200fb24() == 1) {
        if (netError->unk4 == 6 || netError->unk4 == 3) {
            func_ov175_021dae18(p_wk);
            return 2;
        }
    } else if (p_wk->flag164 == 0 && func_0200fb24() == 2) {
        func_ov175_021dae18(p_wk);
        return 2;
    }
    if (p_wk->flag164 != 0 && p_wk->mode != 0 && p_wk->requestSent == 0 && p_wk->state > 7 && p_wk->state < 10) {
        p_wk->flag16C = 0;
        p_wk->state = 6;
    }
    switch (p_wk->state) {
    case 0:
        if (p_wk->flag164 != 0) {
            p_wk->state = 4;
        } else {
            func_02039c68(func_020399a8(), 4, 0x24);
            p_wk->state = 1;
        }
        break;
    case 1:
        if (p_wk->flag164 != 0) {
            p_wk->state = 4;
        } else if (func_02039cb4(func_020399a8(), 4, 0x24) || p_wk->flag164 != 0) {
            p_wk->state = 2;
        }
        break;
    case 2:
        if (p_wk->flag164 != 0) {
            p_wk->state = 4;
        } else if (func_ov175_021dc2c0(p_wk, &p_wk->param.mine) || p_wk->flag164 != 0) {
            p_wk->state = 3;
        }
        break;
    case 3:
        if (p_wk->flag164 != 0) {
            p_wk->state = 4;
        } else if (func_ov175_021dc2d8(p_wk, &p_wk->param.theirs) || p_wk->flag164 != 0) {
            p_wk->state = 4;
        }
        break;
    case 4:
        p_wk->counter = 0;
        p_wk->gdbOpen = TRUE;
        err = func_ov175_021dee3c(0xb5f, 0);
        if (err != 0) {
            func_ov175_021dce24(&p_wk->error, err, 1873);
            func_ov175_021dae18(p_wk);
            return 2;
        }
        p_wk->state = 5;
        break;
    case 5:
        err = func_ov175_021df490(&p_wk->p_data->mine.certificate);
        if (err != 0) {
            func_ov175_021dce24(&p_wk->error, err, 1889);
            func_ov175_021dae18(p_wk);
            return 2;
        }
        p_wk->state = 6;
        break;
    case 6:
        if (func_0203d378() || p_wk->flag164 != 0) {
            p_wk->counter++;
            err = func_ov175_021df018(func_ov175_021dadb4, 100, p_wk);
            p_wk->requestSent = TRUE;
            if (err != 0) {
                func_ov175_021dce24(&p_wk->error, err, 1907);
                func_ov175_021dae18(p_wk);
                return 2;
            }
            p_wk->timer = 0;
            p_wk->state = 7;
        } else {
            p_wk->state = 8;
        }
        break;
    case 7:
        if (p_wk->counter == 0) {
            dst = p_wk->p_data->mine.unk8;
            src = func_ov175_021df20c();
            MI_CpuCopy8(src, dst, 0x28);
            p_wk->state = 8;
        } else {
            if (p_wk->timer++ > 3600) {
                func_ov175_021dce08(&p_wk->error, 1, 1935);
                func_ov175_021dae18(p_wk);
                return 2;
            }
            func_ov175_021defdc();
            if (p_wk->errorCode != 0) {
                func_ov175_021dce24(&p_wk->error, p_wk->errorCode, 1943);
                func_ov175_021dae18(p_wk);
                return 2;
            }
        }
        break;
    case 8:
        if (p_wk->flag164 != 0) {
            p_wk->state = 11;
        } else {
            dst = p_wk->p_data->mine.unk30;
            src = func_ov175_021df224();
            MI_CpuCopy8(src, dst, 0x28);
            func_02039c68(func_020399a8(), 1, 0x24);
            p_wk->state = 9;
        }
        break;
    case 9:
        if (func_02039cb4(func_020399a8(), 1, 0x24) || p_wk->flag164 != 0) {
            p_wk->state = 10;
        }
        break;
    case 10:
        if (func_0203d378() || p_wk->flag164 != 0) {
            if (func_0203d3a8(func_020399a8(), 0x2400, 0x68, &p_wk->p_data->mine) || p_wk->flag164 != 0) {
                p_wk->state = 11;
            }
        } else if (p_wk->flagInfoReceived != 0 || p_wk->flag164 != 0) {
            p_wk->flagInfoReceived = 0;
            err = func_ov175_021df1c8(p_wk->p_data->theirs.unk8);
            dst = p_wk->p_data->mine.unk8;
            src = func_ov175_021df20c();
            MI_CpuCopy8(src, dst, 0x28);
            if (err != 0) {
                func_ov175_021dce24(&p_wk->error, err, 1997);
                func_ov175_021dae18(p_wk);
                return 2;
            }
            p_wk->state = 11;
        }
        break;
    case 11:
        p_wk->counter++;
        err = func_ov175_021df09c(p_wk->unk1ac, func_ov175_021dadcc, 100, p_wk);
        if (err != 0) {
            func_ov175_021dce24(&p_wk->error, err, 2013);
            func_ov175_021dae18(p_wk);
            return 2;
        }
        p_wk->timer = 0;
        p_wk->state = 12;
        break;
    case 12:
        if (p_wk->counter == 0) {
            p_wk->state = 13;
        } else {
            if (p_wk->timer++ > 3600) {
                func_ov175_021dce08(&p_wk->error, 1, 2034);
                func_ov175_021dae18(p_wk);
                return 2;
            }
            func_ov175_021defdc();
            if (p_wk->errorCode != 0) {
                func_ov175_021dce24(&p_wk->error, p_wk->errorCode, 2042);
                func_ov175_021dae18(p_wk);
                return 2;
            }
        }
        break;
    case 13:
        if (p_wk->flag164 != 0) {
            p_wk->state = 19;
        } else {
            func_02039c68(func_020399a8(), 2, 0x24);
            p_wk->state = 14;
        }
        break;
    case 14:
        if (func_02039cb4(func_020399a8(), 2, 0x24) || p_wk->flag164 != 0) {
            p_wk->state = 15;
        }
        break;
    case 15:
        if (func_0203d378()) {
            if (func_0203d3a8(func_020399a8(), 0x2400, 0x68, &p_wk->p_data->mine) || p_wk->flag164 != 0) {
                p_wk->state = 16;
            }
        } else if (p_wk->flagInfoReceived != 0 || p_wk->flag164 != 0) {
            if (p_wk->flag164 == 0) {
                p_wk->flag16C = TRUE;
            }
            p_wk->flagInfoReceived = 0;
            p_wk->state = 16;
        }
        break;
    case 16:
        if (!func_0203d378()) {
            if (func_0203d3a8(func_020399a8(), 0x2400, 0x68, &p_wk->p_data->mine) || p_wk->flag164 != 0) {
                p_wk->state = 17;
            }
        } else if (p_wk->flagInfoReceived != 0 || p_wk->flag164 != 0) {
            if (p_wk->flag164 == 0) {
                p_wk->flag16C = TRUE;
            }
            p_wk->flagInfoReceived = 0;
            p_wk->state = 17;
        }
        break;
    case 17:
        func_02039c68(func_020399a8(), 3, 0x24);
        p_wk->state = 18;
        break;
    case 18:
        if (func_02039cb4(func_020399a8(), 3, 0x24) || p_wk->flag164 != 0) {
            p_wk->state = 19;
        }
        break;
    case 19:
        err =
            func_ov175_021dae54(&p_wk->p_data->mine, &p_wk->p_data->theirs, &p_wk->param, p_wk->unk1ac,
                                                p_wk->resultFunc, p_wk->flag164, p_wk->mode, p_wk->flag16C);
        if (err != 0) {
            func_ov175_021dce24(&p_wk->error, err, 2157);
            func_ov175_021dae18(p_wk);
            return 2;
        }
        p_wk->state = 20;
        break;
    case 20:
        p_wk->counter++;
        err = func_ov175_021df130(p_wk->p_data->mine.connection, p_wk->unk1ac, func_ov175_021dae00, 100,
                                  p_wk);
        if (err != 0) {
            func_ov175_021dce24(&p_wk->error, err, 2172);
            func_ov175_021dae18(p_wk);
            return 2;
        }
        p_wk->timer = 0;
        p_wk->state = 21;
        break;
    case 21:
        if (p_wk->counter == 0) {
            p_wk->state = 22;
        } else {
            if (p_wk->timer++ > 3600) {
                func_ov175_021dce08(&p_wk->error, 1, 2192);
                func_ov175_021dae18(p_wk);
                return 2;
            }
            func_ov175_021defdc();
            if (p_wk->errorCode != 0) {
                func_ov175_021dce24(&p_wk->error, p_wk->errorCode, 2200);
                func_ov175_021dae18(p_wk);
                return 2;
            }
        }
        break;
    case 22:
        p_wk->flag170 = TRUE;
        func_ov175_021dae18(p_wk);
        p_wk->state = 23;
        break;
    case 23:
        func_02039c68(func_020399a8(), 5, 0x24);
        p_wk->state = 24;
        break;
    case 24:
        if (func_02039cb4(func_020399a8(), 5, 0x24) || p_wk->flag164 != 0) {
            p_wk->state = 25;
        }
        break;
    case 25:
        return 1;
    }
    return 0;
}

// pokebw2's WifiBattleMatchNet_GetReceivedValue
u32 func_ov175_021dadac(WifiBattleMatchNet *p_wk) {
    return p_wk->receivedValue;
}

// pokebw2's WifiBattleMatchNet_OnRequestDone
static void func_ov175_021dadb4(int error, void *work) {
    WifiBattleMatchNet *p_wk = work;

    if (error != 0) {
        p_wk->errorCode = error;
    }
    p_wk->counter--;
}

// pokebw2's WifiBattleMatchNet_OnRequestDoneWithData
static void func_ov175_021dadcc(int error, void *work) {
    WifiBattleMatchNet *p_wk = work;
    WifiBattleMatchNetInfo *info = &p_wk->p_data->mine;

    if (error == 0) {
        MI_CpuCopy8(func_ov175_021df224(), info->unk30, 0x28);
    } else {
        p_wk->errorCode = error;
    }
    p_wk->counter--;
}

// pokebw2's WifiBattleMatchNet_OnRequestDone2
static void func_ov175_021dae00(int error, void *work) {
    WifiBattleMatchNet *p_wk = work;

    if (error != 0) {
        p_wk->errorCode = error;
    }
    p_wk->counter--;
}

// pokebw2's WifiBattleMatchNet_EndRequests
static void func_ov175_021dae18(WifiBattleMatchNet *p_wk) {
    if (p_wk->gdbOpen == 1) {
        if (p_wk->state > 0x13) {
            func_ov175_021df458(p_wk->p_data->mine.connection);
        }
        if (p_wk->state >= 4) {
            func_ov175_021def58();
        }
        p_wk->gdbOpen = 0;
    }
}

void func_ov175_021db6a0(WifiBattleMatchNet *p_wk);
static int func_ov175_021dae54(WifiBattleMatchNetInfo *info, WifiBattleMatchNetInfo *theirs,
                               WifiBattleMatchNetParam *param, int a3, WifiBattleMatchNetResultFunc func, int a5,
                               int mode, int a7) {
    int err;
    int resultMode;

    err = func_ov175_021df23c(1, 2, 0, &info->connection);
    if (err != 0) {
        return err;
    }
    err = func_ov175_021df298(info->connection);
    if (err != 0) {
        return err;
    }
    err = func_ov175_021df2c8(info->connection);
    if (err != 0) {
        return err;
    }
    err = func_ov175_021df328(info->connection);
    if (err != 0) {
        return err;
    }
    if (a5 != 0 || mode == 0) {
        resultMode = 5;
    } else {
        switch (param->outcome) {
        case 1:
        case 4:
            resultMode = 0;
            break;
        case 0:
        case 3:
            resultMode = 1;
            break;
        case 2:
            resultMode = 2;
            break;
        default:
            GFL_ASSERT(0);
            break;
        }
    }
    err = func_ov175_021df358(info->connection, 0, info->unk30, 0, resultMode, info->certificate, info->unk58);
    if (err != 0) {
        return err;
    }
    err = func(info, param, 1, a5);
    if (err != 0) {
        return err;
    }
    err = func_ov175_021df328(info->connection);
    if (err != 0) {
        return err;
    }
    if (a7 == 1) {
        int theirsMode;

        if (mode == 0) {
            theirsMode = 5;
        } else if (a5 != 0) {
            theirsMode = 3;
        } else {
            switch (param->outcome) {
            case 1:
            case 4:
                theirsMode = 1;
                break;
            case 0:
            case 3:
                theirsMode = 0;
                break;
            case 2:
                theirsMode = 2;
                break;
            default:
                GFL_ASSERT(0);
                break;
            }
        }
        err = func_ov175_021df358(info->connection, 1, theirs->unk30, 0, theirsMode, theirs->certificate, theirs->unk58);
        if (err != 0) {
            return err;
        }
        err = func(info, param, 0, a5);
        if (err != 0) {
            return err;
        }
    }
    err = func_ov175_021df2f8(info->connection);
    if (err != 0) {
        return err;
    }
    if (a5 != 0) {
        return func_ov175_021df3b0(info->connection, a3, 1);
    }
    return func_ov175_021df3b0(info->connection, a3, 0);
}


static int func_ov175_021dafb8(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                              int a3) {
    int win;
    int lose;
    int skip;
    int draw;
    int decided;
    int err;

    decided = 0;
    skip = 0;
    draw = 1;
    if (param->mine == 0 && param->theirs == 0) {
        draw = 0;
    }
    switch (param->outcome) {
    case 0:
    case 3:
        win = 0;
        lose = 1;
        decided = 1;
        break;
    case 1:
    case 4:
        win = 1;
        lose = 0;
        decided = 1;
        break;
    case 2:
        win = 0;
        lose = 0;
        skip = 1;
        break;
    case 5:
    case 6:
    default:
        win = 0;
        lose = 0;
        break;
    }
    if (isMine == 0 && decided) {
        win ^= 1;
        lose ^= 1;
    }
    switch (param->format) {
    case 0:
        if (skip == 0) {
            err = func_ov175_021df420(info->connection, 0xf, 1);
            if (err != 0) {
                return err;
            }
        }
        err = func_ov175_021df3e8(info->connection, 5, win);
        if (err != 0) {
            return err;
        }
        err = func_ov175_021df3e8(info->connection, 6, lose);
        if (err != 0) {
            return err;
        }
        break;
    case 1:
        if (skip == 0) {
            err = func_ov175_021df420(info->connection, 0x10, 1);
            if (err != 0) {
                return err;
            }
        }
        err = func_ov175_021df3e8(info->connection, 8, win);
        if (err != 0) {
            return err;
        }
        err = func_ov175_021df3e8(info->connection, 7, lose);
        if (err != 0) {
            return err;
        }
        break;
    case 2:
        if (skip == 0) {
            err = func_ov175_021df420(info->connection, 0x12, 1);
            if (err != 0) {
                return err;
            }
        }
        err = func_ov175_021df3e8(info->connection, 0xb, win);
        if (err != 0) {
            return err;
        }
        err = func_ov175_021df3e8(info->connection, 0xc, lose);
        if (err != 0) {
            return err;
        }
        break;
    case 3:
        if (skip == 0) {
            err = func_ov175_021df420(info->connection, 0x11, 1);
            if (err != 0) {
                return err;
            }
        }
        err = func_ov175_021df3e8(info->connection, 9, win);
        if (err != 0) {
            return err;
        }
        err = func_ov175_021df3e8(info->connection, 0xa, lose);
        if (err != 0) {
            return err;
        }
        break;
    case 4:
        if (skip == 0) {
            err = func_ov175_021df420(info->connection, 0x13, 1);
            if (err != 0) {
                return err;
            }
        }
        err = func_ov175_021df3e8(info->connection, 0xd, win);
        if (err != 0) {
            return err;
        }
        err = func_ov175_021df3e8(info->connection, 0xe, lose);
        if (err != 0) {
            return err;
        }
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
    err = func_ov175_021df3e8(info->connection, 0x14, draw != 0);
    if (err != 0) {
        return err;
    }
    return func_ov175_021df3e8(info->connection, 0x1b, -1);
}

static int func_ov175_021db160(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param,
                                                 int isMine, int a3) {
    int win;
    int lose;
    int draw;
    int decided;
    int skip;
    int err;

    decided = 0;
    skip = 0;
    draw = 1;
    if (param->mine == 0 && param->theirs == 0) {
        draw = 0;
    }
    switch (param->outcome) {
    case 0:
    case 3:
        win = 0;
        lose = 1;
        decided = 1;
        break;
    case 1:
    case 4:
        win = 1;
        lose = 0;
        decided = 1;
        break;
    case 2:
        win = 0;
        lose = 0;
        skip = 1;
        break;
    case 5:
    case 6:
    default:
        win = 0;
        lose = 0;
        break;
    }
    if (isMine == 0 && decided) {
        win ^= 1;
        lose ^= 1;
    }
    if (skip == 0) {
        err = func_ov175_021df420(info->connection, 0x15, 1);
        if (err != 0) {
            return err;
        }
    }
    err = func_ov175_021df3e8(info->connection, 0x16, win);
    if (err != 0) {
        return err;
    }
    err = func_ov175_021df3e8(info->connection, 0x17, lose);
    if (err != 0) {
        return err;
    }
    err = func_ov175_021df3e8(info->connection, 0x18, draw != 0);
    if (err != 0) {
        return err;
    }
    return func_ov175_021df3e8(info->connection, 0x1c, -1);
}

static int func_ov175_021db218(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                        int a3) {
    return func_ov175_021df3e8(info->connection, 0x1a, 1);
}

static int func_ov175_021db228(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                        int a3) {
    return func_ov175_021df3e8(info->connection, 0x19, 1);
}

static int func_ov175_021db238(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                            int a3) {
    return func_ov175_021df3e8(info->connection, 0x1b, -1);
}

static int func_ov175_021db248(WifiBattleMatchNetInfo *info, WifiBattleMatchNetParam *param, int isMine,
                                            int a3) {
    return func_ov175_021df3e8(info->connection, 0x1c, -1);
}

// pokebw2's WifiBattleMatchNet_OnInfoReceived
void func_ov175_021db258(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WifiBattleMatchNet *p_wk = work;

    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        MI_CpuCopy8(data, &p_wk->p_data->theirs, 0x68);
        p_wk->flagInfoReceived = 1;
    }
}

void func_ov175_021db294(int netId, int size, const void *data, void *work,
                                                    NetHandle *handle) {
    WifiBattleMatchNet *p_wk = work;
    PlayerInfo *info = func_02012950(p_wk->gameData, netId);

    MI_CpuCopy8(data, info, func_02008468());
    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        p_wk->flagPlayerInfoReceived = 1;
    }
}

// pokebw2's WifiBattleMatchNet_GetRecvBuffer
void *func_ov175_021db2d4(int netId, void *work, int size) {
    WifiBattleMatchNet *p_wk = work;

    if (netId == func_0203d1a4(func_020399a8())) {
        GFL_ASSERT(0);
        return NULL;
    }
    return p_wk->recvBuffer;
}

// pokebw2's WifiBattleMatchNet_OnCmd2402
void func_ov175_021db300(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WifiBattleMatchNet *p_wk = work;

    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        p_wk->flag140 = 1;
    }
}

// pokebw2's WifiBattleMatchNet_OnPartyReceived
void func_ov175_021db328(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WifiBattleMatchNet *p_wk = work;

    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        p_wk->flagPartyReceived = 1;
    }
}

// pokebw2's WifiBattleMatchNet_OnCmd2404
void func_ov175_021db350(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WifiBattleMatchNet *p_wk = work;

    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        p_wk->flag148 = 1;
    }
}

void func_ov175_021db378(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WifiBattleMatchNet *p_wk = work;
    u32 value = *(const u32 *)data;

    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        p_wk->receivedValue = value;
        p_wk->flagValueReceived = 1;
    }
}

void func_ov175_021db3a4(WifiBattleMatchNet *p_wk, int recordId, int kind, void *out) {
    p_wk->state = 0;
    p_wk->unk17c = out;
    p_wk->recordIdArg = recordId;
    switch (kind) {
    case WBM_READ_ARENA:
        p_wk->tableNames = func_ov175_021dd02c();
        p_wk->table_name_num = func_ov175_021dd034();
        p_wk->recordsFunc = func_ov175_021db7c0;
        break;
    case WBM_READ_WIFICUP:
        p_wk->tableNames = func_ov175_021dd038();
        p_wk->table_name_num = func_ov175_021dd040();
        p_wk->recordsFunc = func_ov175_021dba00;
        break;
    case WBM_READ_RECORD_ID:
        p_wk->tableNames = &sRecordIdTable;
        p_wk->table_name_num = 1;
        p_wk->recordsFunc = func_ov175_021dbb00;
        break;
    case WBM_READ_WIFICUP_PARTY:
        p_wk->tableNames = &sWifiCupPartyTable;
        p_wk->table_name_num = 1;
        p_wk->recordsFunc = func_ov175_021dbb54;
        break;
    case WBM_READ_LAST_LOGIN:
        p_wk->tableNames = &sLastLoginTable;
        p_wk->table_name_num = 1;
        p_wk->recordsFunc = func_ov175_021dbbac;
        break;
    }
}

u32 func_ov175_021db478(WifiBattleMatchNet *p_wk) {
    int err;

    switch (p_wk->state) {
    case 0:
        err = func_ov175_021e10cc(0xb5f, p_wk->unk0, 0);
        if (err != 0) {
            func_ov175_021dce40(&p_wk->error, err, 0xdef);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        p_wk->state = 1;
        break;
    case 1:
        GFL_ASSERT(p_wk->table_name_num < WBM_GDB_FIELD_TABLE_MAX);
        if (p_wk->recordIdArg == -1) {
            err = func_ov175_021e1618(WBM_GDB_TABLE_NAME, p_wk->tableNames, p_wk->table_name_num, func_ov175_021db72c, p_wk);
        } else {
            err = func_ov175_021e16f0(WBM_GDB_TABLE_NAME, &p_wk->recordIdArg, 1, p_wk->tableNames, p_wk->table_name_num, func_ov175_021db72c, p_wk);
        }
        if (err != 0) {
            func_ov175_021dce40(&p_wk->error, err, 0xe0a);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        p_wk->state = 2;
        p_wk->timer = 0;
        break;
    case 2:
        if (p_wk->timer++ > 3600) {
            func_ov175_021dce08(&p_wk->error, 1, 0xe17);
            p_wk->state = 6;
        } else {
            func_ov175_021e1414();
            err = func_ov175_021e1434();
            if (err == 0 || err == 4) {
                func_ov175_021dce48(&p_wk->error, err, 0xe26);
                p_wk->state = 6;
            } else if (err == 1) {
                p_wk->state = 3;
            }
        }
        break;
    case 3:
        err = func_ov175_021e1454();
        if (err == 0) {
            func_ov175_021dce50(&p_wk->error, err, 0xe39);
            p_wk->state = 6;
        } else if (err != 1) {
            func_ov175_021dce50(&p_wk->error, err, 0xe3f);
            func_ov175_021db6a0(p_wk);
            return 2;
        } else {
            p_wk->state = 4;
        }
        break;
    case 4:
        func_ov175_021db6a0(p_wk);
        p_wk->state = 5;
        break;
    case 5:
        return 1;
    case 6:
        err = func_ov175_021e11f0();
        if (err == 0) {
            p_wk->timer = 0;
            p_wk->state = 7;
        } else {
            func_ov175_021dce40(&p_wk->error, err, 0xe5e);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        break;
    case 7:
        if (p_wk->timer++ > 3600) {
            func_ov175_021dce08(&p_wk->error, 1, 0xe6a);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        func_ov175_021e1414();
        err = func_ov175_021e1434();
        if (err == 0 || err == 4) {
            func_ov175_021db6a0(p_wk);
            return 2;
        } else if (err == 1) {
            p_wk->state = 8;
        }
        break;
    case 8:
        func_ov175_021db6a0(p_wk);
        return 2;
    }
    return 0;
}




void func_ov175_021db6a0(WifiBattleMatchNet *p_wk) {
    func_ov175_021e1340();
}

void func_ov175_021db6a8(WifiBattleMatchNet *p_wk) {
    PlayerInfo *info = GetGameDataPlayerInfo(p_wk->gameData);

    p_wk->record[0].name = "MYSTATUS";
    p_wk->record[0].type = GDB_FIELD_BINARY;
    p_wk->record[0].value.binaryData.data = info;
    p_wk->record[0].value.binaryData.size = 0x20;
    func_ov175_021da470(p_wk->loginDate);
    p_wk->record[1].name = "LAST_LOGIN_DATETIME";
    p_wk->record[1].type = GDB_FIELD_BINARY;
    p_wk->record[1].value.binaryData.data = p_wk->loginDate;
    p_wk->record[1].value.binaryData.size = 6;
    p_wk->record[2].name = "INITIAL_PROFILE_ID";
    p_wk->record[2].type = GDB_FIELD_INT;
    p_wk->record[2].value.intValue = p_wk->profileId;
    p_wk->record[3].name = "NOW_PROFILE_ID";
    p_wk->record[3].type = GDB_FIELD_INT;
    p_wk->record[3].value.intValue = func_ov175_021dd020(p_wk);
    p_wk->table_name_num = 4;
}

void func_ov175_021db72c(int recordNum, GdbField **records, void *work, int fieldNum) {
    WifiBattleMatchNet *p_wk = work;
    int i;
    int j;

    p_wk->recordsFunc(recordNum, records, p_wk->unk17c, p_wk->table_name_num);
    if (p_wk->recordIdArg == -1) {
        for (j = 0; j < recordNum; j++) {
            for (i = 0; i < p_wk->table_name_num; i++) {
                GdbField *field = &records[j][i];
                if (_STD_CompareNString(field->name, "recordid") == 0) {
                    p_wk->p_data->sake_recordID = field->value.intValue;
                }
            }
        }
    }
}


// pokebw2's WifiBattleMatchNet_ReadArena
void func_ov175_021db7c0(int recordNum, GdbField **records, void *work, int fieldNum) {
    WifiBattleMatchNetStats *stats = work;
    int i;
    int j;

    for (i = 0; i < recordNum; i++) {
        for (j = 0; j < fieldNum; j++) {
            GdbField *field = &records[i][j];

            if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_SINGLE_WIN))) {
                stats->numSingleWin = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_SINGLE_LOSE))) {
                stats->numSingleLose = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_DOUBLE_WIN))) {
                stats->numDoubleWin = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_DOUBLE_LOSE))) {
                stats->numDoubleLose = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_ROTATE_WIN))) {
                stats->numRotateWin = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_ROTATE_LOSE))) {
                stats->numRotateLose = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_TRIPLE_WIN))) {
                stats->numTripleWin = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_TRIPLE_LOSE))) {
                stats->numTripleLose = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_SHOOTER_WIN))) {
                stats->numShooterWin = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_SHOOTER_LOSE))) {
                stats->numShooterLose = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_ELO_RATING_1V1_SINGLE))) {
                stats->eloRatingSingle = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_ELO_RATING_1V1_DOUBLE))) {
                stats->eloRatingDouble = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_ELO_RATING_1V1_ROTATE))) {
                stats->eloRatingRotate = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_ELO_RATING_1V1_TRIPLE))) {
                stats->eloRatingTriple = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_ELO_RATING_1V1_SHOOTER))) {
                stats->eloRatingShooter = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_DISCONNECTS_COUNTER))) {
                stats->disconnectsCounter = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_CHEATS_COUNTER))) {
                stats->cheatsCounter = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, "INITIAL_PROFILE_ID")) {
                stats->initialProfileId = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, "NOW_PROFILE_ID")) {
                stats->nowProfileId = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, "RECORD_SAVE_IDX")) {
                stats->recordSaveIdx = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, "recordid")) {
                stats->recordId = field->value.intValue;
            }
        }
    }
}

// pokebw2's WifiBattleMatchNet_ReadWifiCup
void func_ov175_021dba00(int recordNum, GdbField **records, void *work, int fieldNum) {
    WifiBattleMatchNetCupStats *stats = work;
    int i;
    int j;

    for (i = 0; i < recordNum; i++) {
        for (j = 0; j < fieldNum; j++) {
            GdbField *field = &records[i][j];

            if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_WIFICUP_WIN))) {
                stats->numWin = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_NUM_WIFICUP_LOSE))) {
                stats->numLose = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_ELO_RATING_1V1_WIFICUP))) {
                stats->eloRating = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_DISCONNECTS_WIFICUP_COUNTER))) {
                stats->disconnectsCounter = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, func_ov175_021dd044(ARENA_CHEATS_WIFICUP_COUNTER))) {
                stats->cheatsCounter = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, "RECORD_SAVE_IDX")) {
                stats->recordSaveIdx = field->value.intValue;
            } else if (!_STD_CompareNString(field->name, "WIFICUP_POKEMON_PARTY")) {
                GdbBinaryData *bin = &field->value.binaryData;

                MI_CpuCopy8(bin->data, stats->party, bin->size);
            }
        }
    }
}

// pokebw2's WifiBattleMatchNet_ReadRecordId
void func_ov175_021dbb00(int recordNum, GdbField **records, void *work, int fieldNum) {
    int i;
    int j;

    for (i = 0; i < recordNum; i++) {
        for (j = 0; j < fieldNum; j++) {
            GdbField *field = &records[i][j];

            if (_STD_CompareNString(field->name, "recordid") == 0) {
                *(s32 *)work = field->value.intValue;
            }
        }
    }
}

// pokebw2's WifiBattleMatchNet_ReadWifiCupParty
void func_ov175_021dbb54(int recordNum, GdbField **records, void *work, int fieldNum) {
    int i;
    int j;

    for (i = 0; i < recordNum; i++) {
        for (j = 0; j < fieldNum; j++) {
            GdbField *field = &records[i][j];

            if (_STD_CompareNString(field->name, "WIFICUP_POKEMON_PARTY") == 0) {
                MI_CpuCopy8(field->value.binaryData.data, work, field->value.binaryData.size);
            }
        }
    }
}

// pokebw2's WifiBattleMatchNet_ReadLastLogin
void func_ov175_021dbbac(int recordNum, GdbField **records, void *work, int fieldNum) {
    int i;
    int j;

    for (i = 0; i < recordNum; i++) {
        for (j = 0; j < fieldNum; j++) {
            GdbField *field = &records[i][j];

            if (_STD_CompareNString(field->name, "LAST_LOGIN_DATETIME") == 0) {
                MI_CpuCopy8(field->value.binaryData.data, work, field->value.binaryData.size);
            }
        }
    }
}

// pokebw2's WifiBattleMatchNet_StartWrite
static const char *data_ov175_021e2df8[6] = {
    "RECORD_DATA_01",
    "RECORD_DATA_02",
    "RECORD_DATA_03",
    "RECORD_DATA_04",
    "RECORD_DATA_05",
    "RECORD_DATA_06",
};

void func_ov175_021dbc04(WifiBattleMatchNet *p_wk, int kind, void *data) {
    p_wk->state = 0;
    GFL_ASSERT(p_wk->p_data->sake_recordID != 0);
    switch (kind) {
    case WBM_WRITE_WIFICUP:
        p_wk->record[0].name = func_ov175_021dd044(ARENA_ELO_RATING_1V1_WIFICUP);
        p_wk->record[0].type = GDB_FIELD_INT;
        p_wk->record[0].value.intValue = 1500;
        p_wk->record[1].name = func_ov175_021dd044(ARENA_CHEATS_WIFICUP_COUNTER);
        p_wk->record[1].type = GDB_FIELD_INT;
        p_wk->record[1].value.intValue = 0;
        p_wk->record[2].name = func_ov175_021dd044(ARENA_DISCONNECTS_WIFICUP_COUNTER);
        p_wk->record[2].type = GDB_FIELD_INT;
        p_wk->record[2].value.intValue = 0;
        p_wk->record[3].name = func_ov175_021dd044(ARENA_NUM_WIFICUP_LOSE);
        p_wk->record[3].type = GDB_FIELD_INT;
        p_wk->record[3].value.intValue = 0;
        p_wk->record[4].name = func_ov175_021dd044(ARENA_NUM_WIFICUP_WIN);
        p_wk->record[4].type = GDB_FIELD_INT;
        p_wk->record[4].value.intValue = 0;
        p_wk->record[5].name = "WIFICUP_POKEMON_PARTY";
        p_wk->record[5].type = GDB_FIELD_BINARY;
        p_wk->record[5].value.binaryData.data = data;
        p_wk->record[5].value.binaryData.size = 1332;
        p_wk->table_name_num = 6;
        break;
    case WBM_WRITE_LAST_LOGIN:
        func_ov175_021da470(p_wk->loginDate);
        p_wk->table_name_num = 1;
        p_wk->record[0].name = "LAST_LOGIN_DATETIME";
        p_wk->record[0].type = GDB_FIELD_BINARY;
        p_wk->record[0].value.binaryData.data = p_wk->loginDate;
        p_wk->record[0].value.binaryData.size = 6;
        break;
    case WBM_WRITE_PROFILE:
        func_ov175_021db6a8(p_wk);
        break;
    case WBM_WRITE_RECORD_DATA:
        p_wk->recordData = *(WifiBattleMatchNetRecordData *)data;
        p_wk->table_name_num = 2;
        p_wk->record[0].name = data_ov175_021e2df8[p_wk->recordData.slot];
        p_wk->record[0].type = GDB_FIELD_BINARY;
        p_wk->record[0].value.binaryData.data = p_wk->recordData.data;
        p_wk->record[0].value.binaryData.size = 0x50;
        p_wk->record[1].name = "RECORD_SAVE_IDX";
        p_wk->record[1].type = GDB_FIELD_BYTE;
        p_wk->record[1].value.byteValue = (p_wk->recordData.slot + 1) % 6;
        break;
    }
}

u32 func_ov175_021dbdac(WifiBattleMatchNet *p_wk) {
    int err;

    switch (p_wk->state) {
    case 0:
        err = func_ov175_021e10cc(0xb5f, p_wk->unk0, 0);
        if (err != 0) {
            func_ov175_021dce40(&p_wk->error, err, 0x1161);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        p_wk->state = 1;
        break;
    case 1:
        GFL_ASSERT(p_wk->table_name_num < WBM_GDB_FIELD_TABLE_MAX);
        err = func_ov175_021e1540(WBM_GDB_TABLE_NAME, p_wk->p_data->sake_recordID, p_wk->record, p_wk->table_name_num);
        if (err != 0) {
            func_ov175_021dce40(&p_wk->error, err, 0x116e);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        p_wk->timer = 0;
        p_wk->state = 2;
        break;
    case 2:
        if (p_wk->timer++ > 3600) {
            func_ov175_021dce08(&p_wk->error, 1, 0x117b);
            p_wk->state = 6;
        } else {
            func_ov175_021e1414();
            err = func_ov175_021e1434();
            if (err == 0 || err == 4) {
                func_ov175_021dce48(&p_wk->error, err, 0x1186);
                p_wk->state = 6;
            } else if (err == 1) {
                p_wk->state = 3;
            }
        }
        break;
    case 3:
        err = func_ov175_021e1454();
        if (err == 0) {
            func_ov175_021dce50(&p_wk->error, err, 0x1199);
            p_wk->state = 6;
        } else if (err != 1) {
            func_ov175_021db6a0(p_wk);
            func_ov175_021dce50(&p_wk->error, err, 0x11a0);
            return 2;
        } else {
            p_wk->state = 4;
        }
        break;
    case 4:
        func_ov175_021db6a0(p_wk);
        p_wk->state = 5;
        /* fall through */
    case 5:
        return 1;
    case 6:
        err = func_ov175_021e11f0();
        if (err == 0) {
            p_wk->timer = 0;
            p_wk->state = 7;
        } else {
            func_ov175_021dce40(&p_wk->error, err, 0x11be);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        break;
    case 7:
        if (p_wk->timer++ > 3600) {
            func_ov175_021dce08(&p_wk->error, 1, 0x11ca);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        func_ov175_021e1414();
        err = func_ov175_021e1434();
        if (err == 0 || err == 4) {
            func_ov175_021db6a0(p_wk);
            return 2;
        } else if (err == 1) {
            p_wk->state = 8;
        }
        break;
    case 8:
        func_ov175_021db6a0(p_wk);
        return 2;
    }
    return 0;
}


// pokebw2's WifiBattleMatchNet_StartCreateRecord
void func_ov175_021dbfa4(WifiBattleMatchNet *p_wk) {
    p_wk->state = 0;
    func_ov175_021db6a8(p_wk);
}

u32 func_ov175_021dbfb4(WifiBattleMatchNet *p_wk) {
    int err;

    switch (p_wk->state) {
    case 0:
        err = func_ov175_021e10cc(0xb5f, p_wk->unk0, 0);
        if (err != 0) {
            func_ov175_021dce40(&p_wk->error, err, 0x1215);
            return 2;
        }
        p_wk->state = 1;
        break;
    case 1:
        err = func_ov175_021e1464(WBM_GDB_TABLE_NAME, p_wk->record, p_wk->table_name_num, &p_wk->p_data->sake_recordID);
        if (err != 0) {
            func_ov175_021dce40(&p_wk->error, err, 0x1223);
            return 2;
        }
        p_wk->timer = 0;
        p_wk->state = 2;
        break;
    case 2:
        if (p_wk->timer++ > 3600) {
            func_ov175_021dce08(&p_wk->error, 1, 0x122f);
            p_wk->state = 4;
        } else {
            func_ov175_021e1414();
            err = func_ov175_021e1434();
            if (err == 0 || err == 4) {
                func_ov175_021dce48(&p_wk->error, err, 0x123b);
                p_wk->state = 4;
            } else if (err == 1) {
                p_wk->state = 3;
            }
        }
        break;
    case 3:
        err = func_ov175_021e1454();
        if (err == 0) {
            func_ov175_021dce50(&p_wk->error, err, 0x124f);
            p_wk->state = 4;
        } else if (err != 1) {
            func_ov175_021db6a0(p_wk);
            func_ov175_021dce50(&p_wk->error, err, 0x1256);
            return 2;
        } else {
            if (p_wk->p_data->sake_recordID == 0) {
                func_ov175_021dce08(&p_wk->error, 2, 0x125c);
            }
            func_ov175_021db6a0(p_wk);
            return 1;
        }
        break;
    case 4:
        err = func_ov175_021e11f0();
        if (err == 0) {
            p_wk->timer = 0;
            p_wk->state = 5;
        } else {
            func_ov175_021dce40(&p_wk->error, err, 0x126f);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        break;
    case 5:
        if (p_wk->timer++ > 3600) {
            func_ov175_021dce08(&p_wk->error, 1, 0x127b);
            func_ov175_021db6a0(p_wk);
            return 2;
        }
        func_ov175_021e1414();
        err = func_ov175_021e1434();
        if (err == 0 || err == 4) {
            func_ov175_021db6a0(p_wk);
            return 2;
        } else if (err == 1) {
            p_wk->state = 6;
        }
        break;
    case 6:
        func_ov175_021db6a0(p_wk);
        return 2;
    }
    return 0;
}


// pokebw2's WifiBattleMatchNet_GetRecordId
u32 func_ov175_021dc194(WifiBattleMatchNet *p_wk) {
    return p_wk->p_data->sake_recordID;
}

// pokebw2's WifiBattleMatchNet_SendPlayerInfo
BOOL func_ov175_021dc1a0(WifiBattleMatchNet *p_wk, const void *data) {
    BOOL toServer = func_0203d1a4(func_020399a8()) == 0;
    NetHandle *handle = func_020399a8();

    return func_0203d3f8(handle, toServer, 0x2401, PokeParty_GetSaveDataSize() + 0xd4, data, TRUE, TRUE, TRUE);
}

// pokebw2's WifiBattleMatchNet_TakePlayerInfo
BOOL func_ov175_021dc1e4(WifiBattleMatchNet *p_wk, void **out) {
    u32 flag = p_wk->flagPlayerInfoReceived;

    if (flag != 0) {
        p_wk->flagPlayerInfoReceived = 0;
        *out = p_wk->recvBuffer;
    }
    return flag;
}

// pokebw2's WifiBattleMatchNet_SendParty
BOOL func_ov175_021dc200(WifiBattleMatchNet *p_wk, PokeParty *party) {
    BOOL toServer = func_0203d1a4(func_020399a8()) == 0;
    NetHandle *handle = func_020399a8();

    return func_0203d3f8(handle, toServer, 0x2403, PokeParty_GetSaveDataSize(), party, TRUE, TRUE, TRUE);
}

// pokebw2's WifiBattleMatchNet_TakeParty
BOOL func_ov175_021dc240(WifiBattleMatchNet *p_wk, PokeParty *party) {
    u32 flag = p_wk->flagPartyReceived;

    if (flag != 0) {
        p_wk->flagPartyReceived = 0;
        MI_CpuCopy8(p_wk->recvBuffer, party, PokeParty_GetSaveDataSize());
    }
    return flag;
}

// pokebw2's WifiBattleMatchNet_SendValue4
BOOL func_ov175_021dc268(WifiBattleMatchNet *p_wk, const void *data) {
    BOOL toServer = func_0203d1a4(func_020399a8()) == 0;

    return func_0203d3f8(func_020399a8(), toServer, 0x2404, 4, data, TRUE, TRUE, TRUE);
}

// pokebw2's WifiBattleMatchNet_TakeValue4
BOOL func_ov175_021dc2a0(WifiBattleMatchNet *p_wk, void *out) {
    u32 flag = p_wk->flag148;

    if (flag != 0) {
        p_wk->flag148 = 0;
        MI_CpuCopy8(p_wk->recvBuffer, out, 4);
    }
    return flag;
}

// pokebw2's WifiBattleMatchNet_SendReportValue
BOOL func_ov175_021dc2c0(WifiBattleMatchNet *p_wk, const void *data) {
    return func_0203d3a8(func_020399a8(), 0x2405, 4, data);
}

// pokebw2's WifiBattleMatchNet_TakeReportValue
BOOL func_ov175_021dc2d8(WifiBattleMatchNet *p_wk, u32 *out) {
    u32 flag = p_wk->flagValueReceived;

    if (flag != 0) {
        p_wk->flagValueReceived = 0;
        *out = p_wk->receivedValue;
    }
    return flag;
}

// pokebw2's WifiBattleMatchNet_InitDownload
void func_ov175_021dc2f4(WifiBattleMatchNet *p_wk, s32 profileId) {
    p_wk->state = 0;
    p_wk->downloading = 0;
    MI_CpuFill8(p_wk->downloadAttr, 0, 10);
    MI_CpuFill8(&p_wk->downloadFile, 0, sizeof(DWCNdFileInfo));
    STD_TSPrintf(p_wk->downloadAttr, "%d", profileId);
}

// pokebw2's WifiBattleMatchNet_DownloadMain
u32 func_ov175_021dc338(WifiBattleMatchNet *p_wk) {
    switch (p_wk->state) {
    case 0:
        if (!func_ov114_021c5928(func_ov175_021dc6fc, "IRAO", "WX9x7Zh6J3aBC4zQ")) {
            return 3;
        }
        p_wk->downloading = TRUE;
        func_ov175_021dc680(p_wk, 1);
        break;
    case 1:
        if (!func_ov114_021c5c04("REGCARD_E", p_wk->downloadAttr, "")) {
            func_ov175_021dc6a4(p_wk, 8);
        } else {
            p_wk->state = 2;
        }
        break;
    case 2:
        if (!func_ov114_021c5c38(&p_wk->downloadFile, 0, 1)) {
            func_ov175_021dc6a4(p_wk, 8);
        } else {
            func_ov175_021dc680(p_wk, 3);
        }
        break;
    case 3:
        if (!func_ov175_021dc5ac((const u32 *)&p_wk->downloadFile, sizeof(DWCNdFileInfo))) {
            if (!func_ov114_021c5cfc(&p_wk->downloadFile, p_wk->downloadBuffer, sizeof(p_wk->downloadBuffer))) {
                func_ov175_021dc6a4(p_wk, 8);
            } else {
                data_ov175_021e4b80.done = 0;
                data_ov175_021e4b80.code = 0;
                p_wk->state = 4;
            }
        } else if (!func_ov114_021c5b80()) {
            p_wk->downloading = FALSE;
            return 2;
        } else {
            func_ov175_021dc680(p_wk, 7);
        }
        break;
    case 4:
        func_ov114_021c5ab8();
        if (p_wk->timer++ > 7200) {
            func_0203c9ac(0, 0, 0, 0x3f7);
            func_ov175_021dc6d0(p_wk, 8);
        }
        if (data_ov175_021e4b80.done == 0) {
            if (func_ov114_021c5e14(&p_wk->downloadReceived, &p_wk->downloadSize) == 1) {
                u32 percent = p_wk->downloadReceived * 100 / p_wk->downloadSize;

                if (p_wk->downloadPercent != percent) {
                    p_wk->downloadPercent = percent;
                }
            }
        } else if (data_ov175_021e4b80.code != 0) {
            if (data_ov175_021e4b80.code == ND_ERROR_FATAL) {
                GFL_NetErrAbort();
            } else {
                func_ov175_021dc6a4(p_wk, 8);
            }
        } else {
            if (!func_ov114_021c5b80()) {
                p_wk->downloading = FALSE;
                return 3;
            }
            func_ov175_021dc680(p_wk, 5);
        }
        break;
    case 5:
        p_wk->state = 6;
        break;
    case 6:
        if (func_0200b1a8(p_wk->downloadBuffer)) {
            p_wk->downloading = FALSE;
            return 1;
        }
        p_wk->downloading = FALSE;
        return 2;
    case 7:
        p_wk->downloading = FALSE;
        return 2;
    case 8:
        p_wk->downloading = FALSE;
        return 3;
    case 100:
        func_ov114_021c5ab8();
        if (p_wk->timer++ > 7200) {
            func_0203c9ac(0, 0, 0, 0x3f7);
            func_ov175_021dc6d0(p_wk, 8);
        }
        if (data_ov175_021e4b80.code != 0) {
            if (data_ov175_021e4b80.code == ND_ERROR_FATAL) {
                GFL_NetErrAbort();
            } else {
                func_ov175_021dc6a4(p_wk, 8);
            }
        }
        if (data_ov175_021e4b80.done != 0) {
            data_ov175_021e4b80.done = 0;
            if (data_ov175_021e4b80.code == 0) {
                p_wk->state = p_wk->resumeState;
            }
        }
        break;
    case 200:
        func_ov114_021c5ab8();
        if (data_ov175_021e4b80.done2 != 0) {
            data_ov175_021e4b80.done2 = 0;
            p_wk->state = p_wk->resumeState;
        }
        break;
    case 300:
        func_ov114_021c5ab8();
        if (data_ov175_021e4b80.done != 0) {
            data_ov175_021e4b80.done = 0;
            func_ov175_021dc6a4(p_wk, p_wk->resumeState);
        }
        break;
    }
    return 0;
}

// pokebw2's WifiBattleMatchNet_IsAllZero
static BOOL func_ov175_021dc5ac(const u32 *data, u32 size) {
    u32 i;

    for (i = 0; i < size / 4; i++) {
        if (data[i] != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's WifiBattleMatchNet_OnDwcError
void func_ov175_021dc5c8(void *work, int code, int type, int error) {
    WifiBattleMatchNet *p_wk = work;

    if (type == 3 || type == 6) {
        if (p_wk->p_nhttp != NULL) {
            func_ov114_021b9774(p_wk->p_nhttp);
            func_ov114_021b9838(p_wk->p_nhttp);
            p_wk->p_nhttp = NULL;
        }
        func_ov175_021dae18(p_wk);
    }
}

// pokebw2's WifiBattleMatchNet_OnDwcEvent
u32 func_ov175_021dc5f8(void *work, int a1, int event, int a3) {
    WifiBattleMatchNet *p_wk = work;

    if (event == 3 || event == 6) {
        return p_wk->downloading;
    }
    return 0;
}

// pokebw2's WifiBattleMatchNet_GetHttpResultMessage
u32 func_ov175_021dc610(u32 result) {
    switch (result) {
    case 0:
        GFL_ASSERT(0);
        return 0;
    case 1:
        return 0x34;
    case 2:
        return 0x35;
    case 3:
        return 0x36;
    case 4:
        return 0x37;
    case 5:
        return 0x38;
    case 6:
        return 0x39;
    case 7:
        return 0x3a;
    case 8:
        return 0x3b;
    case 9:
    default:
        return 0x3c;
    }
}

// pokebw2's WifiBattleMatchNet_GetDownload
void func_ov175_021dc66c(WifiBattleMatchNet *p_wk, void *dest) {
    MI_CpuCopy8(p_wk->downloadBuffer, dest, sizeof(p_wk->downloadBuffer));
}

// pokebw2's WifiBattleMatchNet_StartDownloadWait
static void func_ov175_021dc680(WifiBattleMatchNet *p_wk, u32 nextState) {
    data_ov175_021e4b80.done = 0;
    data_ov175_021e4b80.code = 0;
    p_wk->resumeState = nextState;
    p_wk->state = 100;
    p_wk->timer = 0;
}

// pokebw2's WifiBattleMatchNet_StartDownloadEnd
static void func_ov175_021dc6a4(WifiBattleMatchNet *p_wk, u32 nextState) {
    data_ov175_021e4b80.done = 0;
    data_ov175_021e4b80.code = 0;
    p_wk->resumeState = nextState;
    p_wk->state = 200;
    if (!func_ov114_021c5b80()) {
        p_wk->resumeState = nextState;
    }
}

// pokebw2's WifiBattleMatchNet_StartDownloadCancel
void func_ov175_021dc6d0(WifiBattleMatchNet *p_wk, u32 nextState) {
    data_ov175_021e4b80.done = 0;
    data_ov175_021e4b80.code = 0;
    p_wk->resumeState = nextState;
    p_wk->state = 300;
    if (!func_ov114_021c5db4()) {
        p_wk->resumeState = nextState;
    }
}

// pokebw2's WifiBattleMatchNet_OnDownload
static void func_ov175_021dc6fc(u32 reason, u32 error, int serverError) {
    switch (error) {
    case DWC_ND_ERROR_NONE:
    case DWC_ND_ERROR_ALLOC:
    case DWC_ND_ERROR_BUSY:
    case DWC_ND_ERROR_HTTP:
    case DWC_ND_ERROR_BUFFULL:
    case ND_ERROR_BAD_PARAMETER:
    case DWC_ND_ERROR_DLSERVER:
        break;
    case DWC_ND_ERROR_CANCELED:
    case ND_ERROR_FATAL:
        GFL_NetErrAbort();
        break;
    }
    data_ov175_021e4b80.done = 1;
    data_ov175_021e4b80.code = error;
    data_ov175_021e4b80.done2 = 1;
}

void func_ov175_021dc738(WifiBattleMatchNet *p_wk, HeapID heapId) {
    GFL_ASSERT(p_wk->p_nhttp == NULL);
    p_wk->state = 0;
    p_wk->p_nhttp = func_ov114_021b9800(heapId, func_0200853c(GetGameDataPlayerInfo(p_wk->gameData)), p_wk->loginBuffer);
}

u32 func_ov175_021dc77c(WifiBattleMatchNet *p_wk) {
    NHttpRap *p_nhttp = p_wk->p_nhttp;
    int status;
    int result;
    WifiBattleMatchNetAnswer *answer;

    if (p_nhttp == NULL) {
        return 2;
    }
    switch (p_wk->state) {
    case 0:
        if (func_ov114_021b95d4(5, p_nhttp)) {
            p_wk->state = 1;
        }
        break;
    case 1:
        if (func_ov114_021b9748(p_nhttp) == 0) {
            p_wk->state = 2;
        } else {
            if (p_wk->p_nhttp != NULL) {
                func_ov114_021b9838(p_wk->p_nhttp);
                p_wk->p_nhttp = NULL;
            }
            return 2;
        }
        break;
    case 2:
        status = func_ov114_021b99dc(p_nhttp);
        result = func_ov114_021b9790(p_wk->p_nhttp);
        if (result == 0) {
            answer = func_ov114_021b97f4(p_wk->p_nhttp);
            switch (status) {
            case 503:
                p_wk->state = 4;
                func_0200fb4c(0x3e);
                return 0;
            case 502:
                p_wk->state = 4;
                func_0200fb4c(0x3f);
                return 0;
            }
            p_wk->httpValues = answer->values;
            if (answer->result == 0) {
                p_wk->state = 3;
            } else {
                func_0200fb4c(func_ov175_021dc610(answer->result));
                p_wk->state = 4;
            }
        } else if (result != 15) {
            if (p_wk->p_nhttp != NULL) {
                func_ov114_021b9838(p_wk->p_nhttp);
                p_wk->p_nhttp = NULL;
            }
            return 2;
        }
        break;
    case 4:
        MI_CpuFill8(&p_wk->httpValues, 0, sizeof(p_wk->httpValues));
        if (p_wk->p_nhttp != NULL) {
            func_ov114_021b9774(p_wk->p_nhttp);
            func_ov114_021b9838(p_wk->p_nhttp);
            p_wk->p_nhttp = NULL;
        }
        return 3;
    case 3:
        if (p_nhttp != NULL) {
            func_ov114_021b9838(p_nhttp);
            p_wk->p_nhttp = NULL;
        }
        return 1;
    }
    return 0;
}


// pokebw2's WifiBattleMatchNet_GetAccountValues
void func_ov175_021dc8a0(WifiBattleMatchNet *p_wk, void *out) {
    WifiBattleMatchNetValues *values = out;

    *values = p_wk->httpValues;
}

void func_ov175_021dc8b4(WifiBattleMatchNet *p_wk, const void *in, HeapID heapId) {
    const WifiBattleMatchNetValues *values = in;

    GFL_ASSERT(p_wk->p_nhttp == NULL);
    p_wk->p_nhttp = func_ov114_021b9800(heapId, func_0200853c(GetGameDataPlayerInfo(p_wk->gameData)), p_wk->loginBuffer);
    p_wk->postData = *values;
    p_wk->state = 0;
}

u32 func_ov175_021dc908(WifiBattleMatchNet *p_wk) {
    NHttpRap *p_nhttp = p_wk->p_nhttp;
    int status;
    int result;
    WifiBattleMatchNetAnswer *answer;

    if (p_nhttp == NULL) {
        return 2;
    }
    switch (p_wk->state) {
    case 0:
        if (func_ov114_021b95d4(6, p_nhttp)) {
            func_ov114_021be79c(func_ov114_021b973c(p_wk->p_nhttp), &p_wk->postData, sizeof(p_wk->postData));
            p_wk->state = 1;
        }
        break;
    case 1:
        if (func_ov114_021b9748(p_nhttp) == 0) {
            p_wk->state = 2;
        } else {
            if (p_wk->p_nhttp != NULL) {
                func_ov114_021b9838(p_wk->p_nhttp);
                p_wk->p_nhttp = NULL;
            }
            return 2;
        }
        break;
    case 2:
        status = func_ov114_021b99dc(p_nhttp);
        result = func_ov114_021b9790(p_wk->p_nhttp);
        if (result == 0) {
            answer = func_ov114_021b97f4(p_wk->p_nhttp);
            switch (status) {
            case 503:
                p_wk->state = 3;
                func_0200fb4c(0x3e);
                return 0;
            case 502:
                p_wk->state = 3;
                func_0200fb4c(0x3f);
                return 0;
            }
            if (answer->result == 0) {
                p_wk->state = 4;
            } else {
                func_0200fb4c(func_ov175_021dc610(answer->result));
                p_wk->state = 3;
            }
        } else if (result != 15) {
            func_ov175_021dce58(&p_wk->error, result, 0x16a0);
            if (p_wk->p_nhttp != NULL) {
                func_ov114_021b9838(p_wk->p_nhttp);
                p_wk->p_nhttp = NULL;
            }
            return 2;
        }
        break;
    case 3:
        if (p_nhttp != NULL) {
            func_ov114_021b9774(p_nhttp);
            func_ov114_021b9838(p_wk->p_nhttp);
            p_wk->p_nhttp = NULL;
        }
        return 3;
    case 4:
        if (p_nhttp != NULL) {
            func_ov114_021b9838(p_nhttp);
            p_wk->p_nhttp = NULL;
        }
        return 1;
    }
    return 0;
}


// pokebw2's WifiBattleMatchNet_StartNameCheck
void func_ov175_021dca28(WifiBattleMatchNet *p_wk, PlayerInfo *info, u16 unk24) {
    StrBuf *name;
    const u16 *chars;
    u32 i;

    p_wk->namePlayerInfo = info;
    name = copyTrainerNameToNewStrbuf(info->name, p_wk->msgHeapId);
    MI_CpuFill8(&p_wk->nameCheck, 0, sizeof(p_wk->nameCheck));
    p_wk->nameCheck.unk24 = unk24;
    chars = GFL_StrBufGetStringPtr(name);
    for (i = 0; i < GFL_StrBufGetCharCount(name) + 1; i++) {
        if (chars[i] == GFL_StrBufGetTerminator()) {
            p_wk->nameCheck.name[i] = 0;
        } else {
            p_wk->nameCheck.name[i] = chars[i];
        }
    }
    p_wk->nameCheck.words = p_wk->nameCheck.name;
    p_wk->nameCheck.badWordCount = 0;
    func_ov009_0217bc44(&p_wk->nameCheck.words, 1, NULL, 0, &p_wk->nameCheck.result, &p_wk->nameCheck.badWordCount,
                        0x80);
    GFL_StrBufFree(name);
}

// pokebw2's WifiBattleMatchNet_NameCheckMain
BOOL func_ov175_021dcaec(WifiBattleMatchNet *p_wk, u32 *out) {
    BOOL done = DWC_CheckProfanityProcess() == 2;

    if (done) {
        *out = p_wk->nameCheck.badWordCount > 0;
    }
    if (done == 1 && *out == 1) {
        MsgData *msgData =
            GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_HANDLERS_BTL_MAIN, p_wk->msgHeapId);
        StrBuf *name = GFL_MsgDataLoadStrbufNew(msgData, WBM_DEFAULT_NAME);

        GFL_MsgDataFree(msgData);
        copyTrainerNameFromStrbuf(p_wk->namePlayerInfo, name);
        GFL_StrBufFree(name);
    }
    return done;
}

void func_ov175_021dcb58(WifiBattleMatchNet *p_wk, void *data, int kind) {
    PokeParty *party = data;

    p_wk->state = 0;
    p_wk->p_nhttp = func_ov114_021b9800(p_wk->msgHeapId, func_0200853c(GetGameDataPlayerInfo(p_wk->gameData)), p_wk->loginBuffer);
    switch (kind) {
    case 0:
        func_ov114_021b98a0(p_wk->p_nhttp, p_wk->msgHeapId, PokeParty_GetPkmRawSize(), 0);
        func_ov114_021b98d8(p_wk->p_nhttp, party, PokeParty_GetPkmRawSize());
        p_wk->unk178 = 1;
        break;
    case 1: {
        u32 size = PokeParty_GetPkmRawSize();
        int i;

        func_ov114_021b98a0(p_wk->p_nhttp, p_wk->msgHeapId, PokeParty_GetPkmCount(party) * size, 0);
        for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
            PartyPkm *pkm = PokeParty_GetPkm(party, i);

            func_ov114_021b98d8(p_wk->p_nhttp, pkm, PokeParty_GetPkmRawSize());
        }
        p_wk->unk178 = PokeParty_GetPkmCount(party);
        break;
    }
    }
}

u32 func_ov175_021dcc28(WifiBattleMatchNet *p_wk, WifiBattleMatchNetCheck *out) {
    WifiBattleMatchNetAnswer *answer;
    int status;
    int result;
    u32 i;

    switch (p_wk->state) {
    case 0:
        if (func_ov114_021b98f8(p_wk->p_nhttp)) {
            if (func_ov114_021b9748(p_wk->p_nhttp) == 0) {
                p_wk->state++;
            } else {
                if (p_wk->p_nhttp != NULL) {
                    func_ov114_021b9838(p_wk->p_nhttp);
                    p_wk->p_nhttp = NULL;
                    p_wk->state = 0;
                }
                return 2;
            }
        } else {
            if (p_wk->p_nhttp != NULL) {
                func_ov114_021b9838(p_wk->p_nhttp);
                p_wk->p_nhttp = NULL;
                p_wk->state = 0;
            }
            return 2;
        }
        break;
    case 1:
        status = func_ov114_021b99dc(p_wk->p_nhttp);
        result = func_ov114_021b9790(p_wk->p_nhttp);
        if (result == 0) {
            switch (status) {
            case 400:
                func_0200fb4c(0x41);
                if (p_wk->p_nhttp != NULL) {
                    func_ov114_021b99b8(p_wk->p_nhttp);
                    func_ov114_021b9838(p_wk->p_nhttp);
                    func_ov114_021b9774(p_wk->p_nhttp);
                    p_wk->p_nhttp = NULL;
                    p_wk->state = 0;
                }
                return 3;
            case 401:
                func_0200fb4c(0x42);
                if (p_wk->p_nhttp != NULL) {
                    func_ov114_021b99b8(p_wk->p_nhttp);
                    func_ov114_021b9838(p_wk->p_nhttp);
                    func_ov114_021b9774(p_wk->p_nhttp);
                    p_wk->p_nhttp = NULL;
                    p_wk->state = 0;
                }
                return 3;
            case 408:
                func_0200fb4c(0x44);
                if (p_wk->p_nhttp != NULL) {
                    func_ov114_021b99b8(p_wk->p_nhttp);
                    func_ov114_021b9838(p_wk->p_nhttp);
                    func_ov114_021b9774(p_wk->p_nhttp);
                    p_wk->p_nhttp = NULL;
                    p_wk->state = 0;
                }
                return 3;
            default:
                func_0200fb4c(0x43);
                if (p_wk->p_nhttp != NULL) {
                    func_ov114_021b99b8(p_wk->p_nhttp);
                    func_ov114_021b9838(p_wk->p_nhttp);
                    func_ov114_021b9774(p_wk->p_nhttp);
                    p_wk->p_nhttp = NULL;
                    p_wk->state = 0;
                }
                return 3;
            case 200:
                p_wk->state++;
                break;
            }
        } else if (result != 15) {
            func_ov175_021dce58(&p_wk->error, result, 0x17a4);
            func_ov114_021b99b8(p_wk->p_nhttp);
            if (p_wk->p_nhttp != NULL) {
                func_ov114_021b9838(p_wk->p_nhttp);
                p_wk->p_nhttp = NULL;
                p_wk->state = 0;
            }
            return 2;
        }
        break;
    case 2:
        answer = func_ov114_021b97f4(p_wk->p_nhttp);
        out->status = func_ov114_021b99f0(answer);
        for (i = 0; i < p_wk->unk178; i++) {
            out->result[i] = func_ov114_021b99f4(answer, i);
        }
        if (out->status == 0) {
            MI_CpuCopy8(func_ov114_021b9a14(answer, p_wk->unk178), out->signature, 0x80);
        }
        func_ov114_021b99b8(p_wk->p_nhttp);
        if (p_wk->p_nhttp != NULL) {
            func_ov114_021b9838(p_wk->p_nhttp);
            p_wk->p_nhttp = NULL;
        }
        return 1;
    }
    return 0;
}


// pokebw2's WifiBattleMatchNet_SetTimeoutError
static void func_ov175_021dce08(WifiBattleMatchNetError *error, int code, int line) {
    error->timeoutCode = code;
    error->kind = WBM_ERROR_TIMEOUT;
    func_0203c9ac(0, 0, 0, 0x3f5);
}

// pokebw2's WifiBattleMatchNet_SetDwcError
static void func_ov175_021dce24(WifiBattleMatchNetError *error, int code, int line) {
    error->dwcCode = code;
    error->kind = WBM_ERROR_DWC;
    func_0203c9ac(0, 0, 0, 0x3f7);
}

// pokebw2's WifiBattleMatchNet_SetGdbError
void func_ov175_021dce40(WifiBattleMatchNetError *error, int code, int line) {
    error->gdbCode = code;
    error->kind = WBM_ERROR_GDB;
}

// pokebw2's WifiBattleMatchNet_SetGdbSubError1
void func_ov175_021dce48(WifiBattleMatchNetError *error, int code, int line) {
    error->gdbSubCode1 = code;
    error->kind = WBM_ERROR_GDB;
}

// pokebw2's WifiBattleMatchNet_SetGdbSubError2
void func_ov175_021dce50(WifiBattleMatchNetError *error, int code, int line) {
    error->gdbSubCode2 = code;
    error->kind = WBM_ERROR_GDB;
}

// pokebw2's WifiBattleMatchNet_SetHttpError
void func_ov175_021dce58(WifiBattleMatchNetError *error, int code, int line) {
    error->httpCode = code;
    error->kind = WBM_ERROR_HTTP;
}

// pokebw2's WifiBattleMatchNet_GetDwcResult
static u32 func_ov175_021dce60(u32 code) {
    switch (code) {
    case 0:
        return WBM_RESULT_OK;
    case 1:
        return WBM_RESULT_FAILED;
    case 2:
        return WBM_RESULT_FAILED;
    case 3:
        return WBM_RESULT_FAILED;
    case 4:
        return WBM_RESULT_FAILED;
    case 5:
        return WBM_RESULT_FAILED;
    case 6:
        return WBM_RESULT_FAILED;
    case 7:
        return WBM_RESULT_FAILED;
    case 8:
        return WBM_RESULT_FAILED;
    case 9:
        return WBM_RESULT_FAILED;
    case 10:
        return WBM_RESULT_FAILED;
    case 11:
        return WBM_RESULT_FAILED;
    case 12:
        return WBM_RESULT_FAILED;
    case 13:
        return WBM_RESULT_FAILED;
    case 14:
        return WBM_RESULT_FAILED;
    default:
        GFL_ASSERT(0);
        return WBM_RESULT_OK;
    }
}

// pokebw2's WifiBattleMatchNet_GetGdbResult
static u32 func_ov175_021dcee0(u32 code, u32 subCode1, u32 subCode2) {
    switch (code) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return WBM_RESULT_FAILED;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        return WBM_RESULT_DISCONNECTED;
    case 17:
    case 18:
        return WBM_RESULT_FAILED;
    case 19:
        return WBM_RESULT_DISCONNECTED;
    case 21:
    case 22:
        return WBM_RESULT_FAILED;
    }
    switch (subCode1) {
    case 0:
        return WBM_RESULT_DISCONNECTED;
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        return WBM_RESULT_FAILED;
    }
    switch (subCode2) {
    case 2:
        return WBM_RESULT_FAILED;
    case 3:
        return WBM_RESULT_FAILED;
    case 4:
        return WBM_RESULT_DISCONNECTED;
    case 5:
        return WBM_RESULT_FAILED;
    case 6:
        return WBM_RESULT_FAILED;
    case 7:
        return WBM_RESULT_FAILED;
    case 8:
    case 9:
    case 10:
        return WBM_RESULT_FAILED;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        return WBM_RESULT_DISCONNECTED;
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return WBM_RESULT_DISCONNECTED;
    case 22:
        return WBM_RESULT_FAILED;
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        return WBM_RESULT_DISCONNECTED;
    case 28:
    case 29:
        return WBM_RESULT_FAILED;
    case 30:
    case 31:
        return WBM_RESULT_FAILED;
    }
    return WBM_RESULT_OK;
}

// pokebw2's WifiBattleMatchNet_GetTimeoutResult
static u32 func_ov175_021dcfdc(u32 code) {
    switch (code) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    }
    return 0;
}

// pokebw2's WifiBattleMatchNet_ClearError
static void func_ov175_021dcffc(WifiBattleMatchNetError *error) {
    MI_CpuFill8(error, 0, sizeof(WifiBattleMatchNetError));
}

// pokebw2's WifiBattleMatchNet_ShowIcon
void func_ov175_021dd008(WifiBattleMatchNet *p_wk, int a1) {
    if (a1 != 0) {
        func_0203d34c(TRUE, p_wk->msgHeapId);
    }
}

// pokebw2's WifiBattleMatchNet_GetGSID
s32 func_ov175_021dd020(WifiBattleMatchNet *p_wk) {
    return func_02009cc0(p_wk->wifiList);
}
