// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov191/wifi_bsubway.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wifi_bsubway.h"
#include "app/codein.h"
#include "app/wifi_bsubway_graphic.h"
#include "app/wifi_login.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "dpw/dpw_bt.h"
#include "dwc/dwc.h"
#include "field/bsubway_scr.h"
#include "field/unity_tower.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/dpw_profile.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/nhttp_rap.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "nnsys/g2d.h"
#include "pml/species_names.h"
#include "save/bsubway_save.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/bmp_menu.h"
#include "system/bmp_winframe.h"
#include "system/codein_param.h"
#include "system/country_region.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/pms.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/union_view.h"
#include "system/version.h"
#include "system/wordset.h"
#include "text/system/0413.h"
#include "text/system/ability_handlers_btl_main.h"
#include "field/beacon_view.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
extern const GameProcFunctions data_ov129_021da62c;
extern const GameProcFunctions data_ov115_021d6338;
extern const GameProcFunctions data_ov115_021d654c;

// The Wi-Fi Battle Subway's screen: its proc, the three things it does with the Battle Subway's server (sending the
// player's record, downloading a train's trainers and downloading a train's leaders), the requests behind them and the
// message window. The ROM names only wifi_bsubway_graphic.c, so the name of this file is a guess after it and after
// the callers' event_wifi_bsubway.c. All the names are ours

// Frames a request may take before it is given up, a minute
#define REQUEST_TIMEOUT (60 * 60)

// The ranks a Wi-Fi train has
#define RANK_MAX 10

// The name the player gets when the server finds a bad word in theirs
#define DEFAULT_NAME_MSG AbilityHandlersBtlMain_Text_VersionName

#define TEAM_POKEMON_COUNT 3
#define PLAYER_MESSAGE_COUNT 3
#define ROOM_PLAYER_COUNT 7
#define ROOM_LEADER_COUNT 30

#define PALETTE_COUNT 15
#define BLEND_STEPS 4

// The message window's BG, the yes/no menu's, and the frame's palette
#define MESSAGE_BG 1
#define YES_NO_BG 2
#define FRAME_PALETTE 13

// The proc's steps
enum {
    STATE_BGM_FADE_OUT,
    STATE_BGM_START,
    STATE_LOGIN,
    STATE_LOGIN_WAIT,
    STATE_VIEW_INIT,
    STATE_FADE_IN_WAIT,
    STATE_CONNECT,
    STATE_CONNECT_WAIT,
    STATE_CONNECT_PRINT_WAIT,
    STATE_CHECK_NAME,
    STATE_CHECK_NAME_WAIT,
    STATE_SET_PROFILE,
    STATE_SET_PROFILE_WAIT,
    STATE_MODE_MAIN,
    STATE_DISCONNECT,
    STATE_DISCONNECT_WAIT,
    STATE_LOGOUT,
    STATE_BGM_END,
    STATE_BGM_END_WAIT,
    STATE_END,
    STATE_ERROR,
    STATE_ERROR_WAIT,
};

// What a mode's function returns
enum {
    MODE_RESULT_CONTINUE,
    MODE_RESULT_END,
    MODE_RESULT_CANCEL,
};

// What the check of the team's Pokémon returns
enum {
    CHECK_RESULT_WAIT,
    CHECK_RESULT_OK,
    CHECK_RESULT_NG,
};

// What went wrong with the check of the team's Pokémon
enum {
    CHECK_ERROR_NONE,
    // A Pokémon failed the check a second time
    CHECK_ERROR_POKEMON,
    // The server refused the check
    CHECK_ERROR_STATUS,
    // The request failed, with an HTTP status
    CHECK_ERROR_HTTP,
};

// The server library's types, which are the DS Wi-Fi library's Dpw_Bt_Player, Dpw_Bt_Leader and Dpw_Bt_Room. The
// library's functions take them as untyped pointers. The Pokémon are BSubwayPokemon and the messages PMSData, kept as
// bytes: the types are aligned to 2 bytes
// Black 2's; twinned code confirms the offsets of pokemon, name, versionCode, langCode, countryCode, localCode,
// playerId, leaderMessage, gender, trainerType, messages and result in Black
typedef struct {
    u8 pokemon[TEAM_POKEMON_COUNT][sizeof(BSubwayPokemon)];
    u16 name[8];
    u8 versionCode;
    u8 langCode;
    u8 countryCode;
    u8 localCode;
    u8 playerId[4];
    u8 leaderMessage[sizeof(PMSData)];
    u8 ngName : 1;
    u8 gender : 1;
    u8 : 6;
    u8 trainerType;
    u8 messages[PLAYER_MESSAGE_COUNT][sizeof(PMSData)];
    u16 result;
} Dpw_Bt_Player;

// Black 2's; twinned code confirms the offsets of langCode, countryCode and localCode in Black
typedef struct {
    u16 name[8];
    u8 versionCode;
    u8 langCode;
    u8 countryCode;
    u8 localCode;
    u8 playerId[4];
    u8 leaderMessage[8];
    u8 flags;
    u8 padding;
} Dpw_Bt_Leader;

typedef struct {
    Dpw_Bt_Player players[ROOM_PLAYER_COUNT];
    Dpw_Bt_Leader leaders[ROOM_LEADER_COUNT];
} Dpw_Bt_Room;

// Palette 1's colors 4 to 10 cycle through blends of the file's palettes, a step every other frame, as on the Wi-Fi
// login's screen (wifilogin_disp.c)
// Black 2's; twinned code confirms the offsets of task and active in Black
typedef struct {
    TCB *task;
    BOOL active;
    u16 palettes[PALETTE_COUNT][16];
    u16 blended[PALETTE_COUNT * BLEND_STEPS][16];
    s16 frame;
    u8 unk96A;
    u8 skip;
    // The BG that scrolls with the cycle
    u8 scrollBg;
    u8 unk96D[3];
} WifiBSubwayPaletteCycle;

// The screen's display: the BGs and the message window
typedef struct {
    BOOL initialized;
    HeapID heapId;
    WifiBSubwayGraphic *graphic;
    WifiBSubwayPaletteCycle cycle;
    BmpWin *window;
    BmpMenu *yesNo;
    // The frames' characters, of the message window's BG and of the yes/no menu's
    u32 messageFrameChars;
    u32 yesNoFrameChars;
    WaitIcon *waitIcon;
    MsgData *msgData;
    WordSet *wordSet;
    Font *font;
    StrBuf *message;
    StrBuf *expanded;
    PrintStream *stream;
    TCBExManager *tcbManager;
} WifiBSubwayView;

// A train's download: the number of trains a rank has, then the trainers and leaders of one
typedef struct {
    BOOL hasCount;
    BOOL downloaded;
    u16 rank;
    u16 number;
    u16 count;
    Dpw_Bt_Room data;
} WifiBSubwayRoom;

// The check of the player's name for bad words
typedef struct {
    u16 name[12];
    const u16 *words;
    char result;
    int badWordCount;
    HeapID heapId;
} WifiBSubwayNameCheck;

// The player's profile and record, as the server takes them
typedef struct {
    BOOL profileSet;
    Dpw_Common_Profile profile;
    Dpw_Common_ProfileResult profileResult;
    Dpw_Bt_Player player;
    BSubwayWifiData *wifiData;
    BSubwayScoreData *score;
    // The check of the team's Pokémon, whose signature goes with the record
    NHttpRap *nhttp;
    u16 pokemonCount;
    u8 sign[0x80];
    WifiBSubwayNameCheck nameCheck;
    // A copy of the player's, whose name the check may replace
    PlayerInfo *playerInfo;
} WifiBSubwayUpload;

// What went wrong with a request
typedef struct {
    // The server library's error, when negative
    s32 dpwResult;
    // The server's state, which nothing sets
    u32 serverState;
    // Whether the request ran out of time
    u16 timeout;
    s16 timer;
    // A CHECK_ERROR_*, and the HTTP status of CHECK_ERROR_HTTP
    u32 checkError;
    s32 httpStatus;
} WifiBSubwayError;

// Black 2's; twinned code confirms the offsets of param, view, loginBuffer, dpwActive, saving, seq, inputNumber, room,
// upload, error, codeIn, gameData, wifiList, playerInfo, wifiData and score in Black
typedef struct {
    WifiBSubwayParam *param;
    WifiBSubwayView view;
    WifiLoginParam login;
    WifiLogoutParam logout;
    u8 loginBuffer[0x174];
    // Whether the server library runs
    u16 dpwActive;
    u16 saving;
    // The mode's step
    u32 seq;
    // The number the player typed
    u32 inputNumber;
    WifiBSubwayRoom room;
    WifiBSubwayUpload upload;
    WifiBSubwayError error;
    CodeInParam *codeIn;
    GameData *gameData;
    AdventureSave *adventure;
    WifiList *wifiList;
    PlayerInfo *playerInfo;
    BSubwayWifiData *wifiData;
    BSubwayScoreData *score;
} WifiBSubwayWork;

typedef u32 (*WifiBSubwayModeFunc)(WifiBSubwayWork *wk, HeapID heapId);

BOOL func_ov116_021d6660(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov116_021d6710(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov116_021d6918(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov116_021d696c(WifiBSubwayWork *wk, int state);
static void func_ov116_021d6998(WifiBSubwayRoom *room);
void func_ov116_021d69a8(WifiBSubwayRoom *room, WifiBSubwayError *error, u16 rank);
BOOL func_ov116_021d69c0(WifiBSubwayRoom *room, WifiBSubwayError *error);
void func_ov116_021d69ec(WifiBSubwayRoom *room, WifiBSubwayError *error, u16 number);
BOOL func_ov116_021d6a10(WifiBSubwayRoom *room, WifiBSubwayError *error);
u32 func_ov116_021d6a40(const WifiBSubwayRoom *room);
u32 func_ov116_021d6a44(const WifiBSubwayRoom *room);
u32 func_ov116_021d6a48(const WifiBSubwayRoom *room);
u32 func_ov116_021d6a4c(const WifiBSubwayRoom *room);
void func_ov116_021d6a5c(WifiBSubwayRoom *room);
void func_ov116_021d6af0(WifiBSubwayRoom *room, BSubwayWifiData *wifiData);
void func_ov116_021d6b30(WifiBSubwayRoom *room, BSubwayWifiData *wifiData);
static void func_ov116_021d6b50(WifiBSubwayUpload *upload, HeapID heapId);
static void func_ov116_021d6b70(WifiBSubwayUpload *upload);
void func_ov116_021d6b88(WifiBSubwayUpload *upload, GameData *gameData, HeapID heapId);
u32 func_ov116_021d6c50(WifiBSubwayUpload *upload, HeapID heapId);
void func_ov116_021d6cbc(WifiBSubwayUpload *upload, GameData *gameData, WifiBSubwayError *error);
BOOL func_ov116_021d6d18(WifiBSubwayUpload *upload, WifiBSubwayError *error);
void func_ov116_021d6d40(Dpw_Bt_Player *player, GameData *gameData, PlayerInfo *playerInfo);
void func_ov116_021d6e3c(void *work, int code, int type, int error);
void func_ov116_021d6e50(WifiBSubwayUpload *upload, WifiBSubwayError *error, void *loginBuffer,
                                           HeapID heapId);
u32 func_ov116_021d6ec4(WifiBSubwayUpload *upload, WifiBSubwayError *error, BOOL last);
void func_ov116_021d7010(WifiBSubwayUpload *upload, int index);
void func_ov116_021d7038(WifiBSubwayUpload *upload);
void func_ov116_021d7060(WifiBSubwayUpload *upload, WifiBSubwayError *error);
BOOL func_ov116_021d70b0(WifiBSubwayUpload *upload, WifiBSubwayError *error);
static void func_ov116_021d70d8(WifiBSubwayError *error);
void func_ov116_021d70e4(WifiBSubwayError *error);
BOOL func_ov116_021d70ec(WifiBSubwayError *error, s32 *result);
BOOL func_ov116_021d7158(const WifiBSubwayError *error);
u32 func_ov116_021d7184(const WifiBSubwayError *error);
BOOL func_ov116_021d7200(const WifiBSubwayError *error);
void func_ov116_021d724c(WifiBSubwayError *error, u32 checkError);
void func_ov116_021d7250(WifiBSubwayError *error, s32 status);
void func_ov116_021d7258(WifiBSubwayError *error);
void func_ov116_021d7260(WifiBSubwayError *error);
void func_ov116_021d7278(WifiBSubwayWork *wk, HeapID heapId);
void func_ov116_021d72d0(WifiBSubwayWork *wk, HeapID heapId);
void func_ov116_021d730c(WifiBSubwayWork *wk);
u32 func_ov116_021d7324(WifiBSubwayWork *wk);
void func_ov116_021d737c(WifiBSubwayWork *wk);
static void func_ov116_021d73d0(WifiBSubwayWork *wk);
u32 func_ov116_021d73d4(const WifiBSubwayWork *wk);
void func_ov116_021d73e0(WifiBSubwayWork *wk);
static void func_ov116_021d7420(WifiBSubwayWork *wk);
u32 func_ov116_021d7424(WifiBSubwayWork *wk, HeapID heapId);
u32 func_ov116_021d7450(WifiBSubwayWork *wk, HeapID heapId);
u32 func_ov116_021d7550(WifiBSubwayWork *wk, HeapID heapId);
u32 func_ov116_021d77d0(WifiBSubwayWork *wk, HeapID heapId);
void func_ov116_021d7a3c(WifiBSubwayWork *wk);
BOOL func_ov116_021d7a94(WifiBSubwayWork *wk);
void func_ov116_021d7aa8(WifiBSubwayWork *wk);
BOOL func_ov116_021d7ac8(WifiBSubwayWork *wk);
BOOL func_ov116_021d7b00(WifiBSubwayWork *wk);
BOOL func_ov116_021d7b64(WifiBSubwayWork *wk);
void func_ov116_021d7b70(WifiBSubwayView *view, HeapID heapId);
static void func_ov116_021d7c58(WifiBSubwayView *view);
void func_ov116_021d7cf4(WifiBSubwayView *view);
void func_ov116_021d7d0c(WifiBSubwayView *view, HeapID heapId);
void func_ov116_021d7f54(TCB *task, void *work);
static void func_ov116_021d7fc4(WifiBSubwayView *view);
void func_ov116_021d8010(WifiBSubwayView *view, u32 message);
BOOL func_ov116_021d80cc(WifiBSubwayView *view);
void func_ov116_021d8118(WifiBSubwayView *view, u32 index, u32 number, u32 digits);
void func_ov116_021d8134(WifiBSubwayView *view);
u32 func_ov116_021d8168(WifiBSubwayView *view);
void func_ov116_021d8190(WifiBSubwayView *view);
static void func_ov116_021d81b4(WifiBSubwayView *view);
void func_ov116_021d81d0(WifiBSubwayWork *wk);
BOOL func_ov116_021d81ec(WifiBSubwayWork *wk);

// pokebw2's WIFI_BSUBWAY_PROC_FUNCTIONS
const GameProcFunctions data_ov116_021d83a4 = {
    func_ov116_021d6660,
    func_ov116_021d6710,
    func_ov116_021d6918,
};

// pokebw2's sWifiBSubwayYesNoSetup
static ConfirmDialogSetup data_ov116_021d8560 = { YES_NO_BG, 24, 13, 12, 0 };
// The groups of digits of a train's number, whose length depends on how many trains there are, and of a rank
// pokebw2's sWifiBSubwayRoomBlocks
static int data_ov116_021d8568[CODEIN_BLOCK_COUNT] = { 3, 0, 0 };
// pokebw2's sWifiBSubwayRankBlocks
static int data_ov116_021d8574[CODEIN_BLOCK_COUNT] = { 2, 0, 0 };

// pokebw2's WifiBSubway_ProcInit
BOOL func_ov116_021d6660(GameProc *proc, u32 *state, void *paramPtr, void *work) {
    WifiBSubwayParam *param = paramPtr;
    WifiBSubwayWork *wk;
    GameData *gameData;
    SaveControl *save;

    GFL_OvlLoad(OVERLAY_ID(114));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFI_BSUBWAY, 0x18000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WifiBSubwayWork), HEAPID_WIFI_BSUBWAY);
    MI_CpuFill8(wk, 0, sizeof(WifiBSubwayWork));
    wk->param = param;
    param->result = WIFI_BSUBWAY_RESULT_ERROR;
    gameData = GSYS_GetGameData(wk->param->gsys);
    save = GameData_GetSaveControl(gameData);
    wk->gameData = gameData;
    wk->adventure = getSaveAdventureDataBlk(save);
    wk->wifiList = func_0201283c(gameData);
    wk->playerInfo = GetGameDataPlayerInfo(gameData);
    wk->wifiData = func_02012efc(gameData);
    wk->score = func_02012ef0(gameData);
    func_ov116_021d6998(&wk->room);
    func_ov116_021d6b50(&wk->upload, HEAPID_WIFI_BSUBWAY);
    func_ov116_021d70d8(&wk->error);
    return TRUE;
}

static BOOL func_ov116_021d6710(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBSubwayWork *wk = work;

    if (func_ov116_021d696c(wk, *state)) {
        if (func_ov116_021d7158(&wk->error) || func_0200fb24()) {
            *state = 20;
        }
    }
    switch (*state) {
    case 0:
        GFL_SndBGMFadeOut(6);
        (*state)++;
        break;
    case 1:
        if (!GFL_SndBGMIsFading()) {
            GFL_SndBGMSetPaused(TRUE);
            GFL_SndBGMPush();
            GFL_SndBGMPlay(SEQ_BGM_WIFI_ACCESS, SND_CHANNEL_MASK_ALL);
            GFL_SndBGMFadeIn(6);
            (*state)++;
        }
        break;
    case 2:
        func_ov116_021d737c(wk);
        (*state)++;
        break;
    case 3:
        if (func_ov116_021d73d4(wk) == 0) {
            (*state)++;
        } else {
            wk->param->result = WIFI_BSUBWAY_RESULT_CANCEL;
            *state = 17;
        }
        break;
    case 4:
        func_ov116_021d7b70(&wk->view, HEAPID_WIFI_BSUBWAY);
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 1);
        (*state)++;
        break;
    case 5:
        if (!GFL_FadeIsRunning()) {
            (*state)++;
        }
        break;
    case 6:
        func_ov116_021d7a3c(wk);
        func_ov116_021d8010(&wk->view, Bank0413_Text_026);
        func_ov116_021d8190(&wk->view);
        (*state)++;
        break;
    case 7:
        if (func_ov116_021d7a94(wk)) {
            (*state)++;
        }
        break;
    case 8:
        if (func_ov116_021d80cc(&wk->view)) {
            (*state)++;
        }
        break;
    case 9:
        func_ov116_021d6b88(&wk->upload, wk->gameData, HEAPID_WIFI_BSUBWAY);
        (*state)++;
        break;
    case 10:
        if (func_ov116_021d6c50(&wk->upload, HEAPID_WIFI_BSUBWAY)) {
            (*state)++;
        }
        break;
    case 11:
        func_ov116_021d6cbc(&wk->upload, wk->gameData, &wk->error);
        (*state)++;
        break;
    case 12:
        if (func_ov116_021d6d18(&wk->upload, &wk->error)) {
            (*state)++;
        }
        break;
    case 13: {
        u32 result = func_ov116_021d7424(wk, HEAPID_WIFI_BSUBWAY);

        if (result == 1) {
            wk->param->result = WIFI_BSUBWAY_RESULT_OK;
            (*state)++;
        } else if (result == 2) {
            wk->param->result = WIFI_BSUBWAY_RESULT_CANCEL;
            (*state)++;
        }
        break;
    }
    case 14:
        func_ov116_021d7aa8(wk);
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 1);
        (*state)++;
        break;
    case 15:
        if (func_ov116_021d7ac8(wk) && !GFL_FadeIsRunning()) {
            func_ov116_021d7c58(&wk->view);
            (*state)++;
        }
        break;
    case 16:
        if (!GFL_NetErrShow(FALSE)) {
            func_ov116_021d73e0(wk);
        }
        (*state)++;
        break;
    case 17:
        GFL_SndBGMFadeOut(6);
        (*state)++;
        break;
    case 18:
        if (!GFL_SndBGMIsFading()) {
            (*state)++;
        }
        break;
    case 19:
        return TRUE;
    case 20:
        if (func_ov116_021d7b00(wk)) {
            *state = 21;
        } else {
            *state = 14;
        }
        func_ov116_021d7038(&wk->upload);
        break;
    case 21:
        if (func_ov116_021d7b64(wk)) {
            *state = 14;
        }
        break;
    }
    if (wk->dpwActive != 0) {
        func_ov114_021c8fa8();
    }
    func_ov116_021d7cf4(&wk->view);
    return FALSE;
}


// pokebw2's WifiBSubway_ProcExit
static BOOL func_ov116_021d6918(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBSubwayWork *wk = work;

    func_ov116_021d730c(wk);
    func_ov116_021d73d0(wk);
    func_ov116_021d7420(wk);
    func_ov116_021d6b70(&wk->upload);
    func_ov116_021d7c58(&wk->view);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WIFI_BSUBWAY);
    GFL_OvlUnload(OVERLAY_ID(114));
    GFL_SndBGMPop();
    GFL_SndBGMSetPaused(FALSE);
    GFL_SndBGMFadeIn(60);
    return TRUE;
}

// Whether an error is acted on: up to the mode's steps, while the server library runs and nothing is being saved
// pokebw2's WifiBSubway_CanCheckError
BOOL func_ov116_021d696c(WifiBSubwayWork *wk, int state) {
    if (state <= STATE_MODE_MAIN && !GFL_FadeIsRunning() && wk->dpwActive && !wk->saving) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiBSubwayRoom_Init
static void func_ov116_021d6998(WifiBSubwayRoom *room) {
    MI_CpuFill8(room, 0, sizeof(WifiBSubwayRoom));
}

// Asks the server how many trains a rank has
// pokebw2's WifiBSubwayRoom_RequestCount
void func_ov116_021d69a8(WifiBSubwayRoom *room, WifiBSubwayError *error, u16 rank) {
    room->rank = rank;
    func_ov114_021c941c((u8)rank);
    func_ov116_021d70e4(error);
}

// pokebw2's WifiBSubwayRoom_WaitCount
BOOL func_ov116_021d69c0(WifiBSubwayRoom *room, WifiBSubwayError *error) {
    s32 result;

    if (func_ov116_021d70ec(error, &result) && !func_ov116_021d7158(error)) {
        room->count = result;
        room->hasCount = TRUE;
        return TRUE;
    }
    return FALSE;
}

// Asks the server for a train of the rank
// pokebw2's WifiBSubwayRoom_Download
void func_ov116_021d69ec(WifiBSubwayRoom *room, WifiBSubwayError *error, u16 number) {
    room->number = number;
    func_ov114_021c94a0((u8)room->rank, (u8)room->number, &room->data);
    func_ov116_021d70e4(error);
}

// pokebw2's WifiBSubwayRoom_WaitDownload
BOOL func_ov116_021d6a10(WifiBSubwayRoom *room, WifiBSubwayError *error) {
    s32 result;

    if (room->downloaded) {
        return TRUE;
    }
    if (func_ov116_021d70ec(error, &result) && !func_ov116_021d7158(error)) {
        room->downloaded = TRUE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiBSubwayRoom_GetRank
u32 func_ov116_021d6a40(const WifiBSubwayRoom *room) {
    return room->rank;
}

// pokebw2's WifiBSubwayRoom_GetNumber
u32 func_ov116_021d6a44(const WifiBSubwayRoom *room) {
    return room->number;
}

// pokebw2's WifiBSubwayRoom_GetCount
u32 func_ov116_021d6a48(const WifiBSubwayRoom *room) {
    return room->count;
}

// How many digits a train's number has
// pokebw2's WifiBSubwayRoom_GetCountDigits
u32 func_ov116_021d6a4c(const WifiBSubwayRoom *room) {
    if (room->count < 100) {
        return 2;
    }
    return 3;
}

// Replaces the countries, regions and trainer types that this game doesn't know
// pokebw2's WifiBSubwayRoom_FixData
void func_ov116_021d6a5c(WifiBSubwayRoom *room) {
    Dpw_Bt_Player *players = room->data.players;
    Dpw_Bt_Leader *leaders = room->data.leaders;
    int i;

    for (i = 0; i < ROOM_PLAYER_COUNT; i++) {
        players[i].countryCode =
            func_02025c1c(players[i].countryCode, players[i].localCode, players[i].langCode);
        players[i].localCode = func_02025c30(players[i].countryCode, players[i].localCode, players[i].langCode);
        players[i].trainerType = func_02025c88(func_02025cb0(players[i].trainerType));
    }
    for (i = 0; i < ROOM_LEADER_COUNT; i++) {
        leaders[i].countryCode =
            func_02025c1c(leaders[i].countryCode, leaders[i].localCode, leaders[i].langCode);
        leaders[i].localCode = func_02025c30(leaders[i].countryCode, leaders[i].localCode, leaders[i].langCode);
    }
}

// Saves the train's trainers, and that the train was taken on the server's date
// pokebw2's WifiBSubwayRoom_SavePlayers
void func_ov116_021d6af0(WifiBSubwayRoom *room, BSubwayWifiData *wifiData) {
    RTCTime time;
    RTCDate date;

    DWC_GetDateTime(&date, &time);
    func_0200de6c(wifiData, room->data.players, room->rank, room->number);
    func_0200dd28(wifiData, room->rank, room->number, &date);
}

// pokebw2's WifiBSubwayRoom_SaveLeaders
void func_ov116_021d6b30(WifiBSubwayRoom *room, BSubwayWifiData *wifiData) {
    func_0200df10(wifiData, room->data.leaders, room->rank, room->number);
}

// pokebw2's WifiBSubwayUpload_Init
static void func_ov116_021d6b50(WifiBSubwayUpload *upload, HeapID heapId) {
    MI_CpuFill8(upload, 0, sizeof(WifiBSubwayUpload));
    upload->playerInfo = func_0200846c(heapId);
}

// pokebw2's WifiBSubwayUpload_Free
static void func_ov116_021d6b70(WifiBSubwayUpload *upload) {
    if (upload->playerInfo != NULL) {
        GFL_HeapFree(upload->playerInfo);
        upload->playerInfo = NULL;
    }
}

// Copies the player's profile and asks the server whether the name is a bad word
// pokebw2's WifiBSubwayUpload_CheckName
void func_ov116_021d6b88(WifiBSubwayUpload *upload, GameData *gameData, HeapID heapId) {
    StrBuf *name;
    const u16 *chars;
    u32 i;

    func_02008494(GetGameDataPlayerInfo(gameData), upload->playerInfo);
    name = copyTrainerNameToNewStrbuf(upload->playerInfo->name, heapId);
    MI_CpuFill8(&upload->nameCheck, 0, sizeof(WifiBSubwayNameCheck));
    upload->nameCheck.heapId = heapId;
    chars = GFL_StrBufGetStringPtr(name);
    for (i = 0; i < GFL_StrBufGetCharCount(name) + 1; i++) {
        if (chars[i] == GFL_StrBufGetTerminator()) {
            upload->nameCheck.name[i] = 0;
        } else {
            upload->nameCheck.name[i] = chars[i];
        }
    }
    upload->nameCheck.words = upload->nameCheck.name;
    upload->nameCheck.badWordCount = 0;
    func_ov009_0217bc44(&upload->nameCheck.words, 1, NULL, 0, &upload->nameCheck.result,
                        &upload->nameCheck.badWordCount, sizeof(upload->sign));
    GFL_StrBufFree(name);
}

// Returns 0 until the check ends, 1 once it does, and 2 when the name was replaced
// pokebw2's WifiBSubwayUpload_WaitCheckName
u32 func_ov116_021d6c50(WifiBSubwayUpload *upload, HeapID heapId) {
    BOOL done = DWC_CheckProfanityProcess() == 2;
    BOOL bad;

    if (done) {
        bad = upload->nameCheck.badWordCount > 0;
    }
    if (!done) {
        return 0;
    }
    if (done == TRUE && bad == TRUE) {
        MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_HANDLERS_BTL_MAIN, heapId);
        StrBuf *name = GFL_MsgDataLoadStrbufNew(msgData, DEFAULT_NAME_MSG);

        GFL_MsgDataFree(msgData);
        copyTrainerNameFromStrbuf(upload->playerInfo, name);
        GFL_StrBufFree(name);
        return 2;
    }
    return 1;
}

// Sends the player's profile and fills the record
// pokebw2's WifiBSubwayUpload_SetProfile
void func_ov116_021d6cbc(WifiBSubwayUpload *upload, GameData *gameData, WifiBSubwayError *error) {
    GameData_GetSaveControl(gameData);
    BSubwayWifiData *wifiData = func_02012efc(gameData);
    BSubwayScoreData *score = func_02012ef0(gameData);
    PlayerInfo *playerInfo = upload->playerInfo;

    func_ov114_021b9b00(&upload->profile, playerInfo);
    func_ov114_021c9744(&upload->profile, &upload->profileResult);
    func_ov116_021d70e4(error);
    func_ov116_021d6d40(&upload->player, gameData, playerInfo);
    upload->wifiData = wifiData;
    upload->score = score;
}

// pokebw2's WifiBSubwayUpload_WaitProfile
BOOL func_ov116_021d6d18(WifiBSubwayUpload *upload, WifiBSubwayError *error) {
    s32 result;

    if (func_ov116_021d70ec(error, &result) && !func_ov116_021d7158(error)) {
        upload->profileSet = TRUE;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiBSubwayUpload_FillPlayer
void func_ov116_021d6d40(Dpw_Bt_Player *player, GameData *gameData, PlayerInfo *playerInfo) {
    SaveControl *save = GameData_GetSaveControl(gameData);
    getUnityTower_SurveySaveBlkAddrress(save);
    func_0201283c(gameData);
    BSubwayScoreData *score = func_02012ef0(gameData);
    void *cgear = getCGearDataBlkAddress(save);
    int i;

    MI_CpuFill8(player, 0, sizeof(Dpw_Bt_Player));
    MI_CpuCopy8(GetPlayerName(playerInfo), player->name, sizeof(player->name));
    *(u32 *)player->playerId = getIDAsUInt(playerInfo);
    player->versionCode = game_version;
    player->langCode = region;
    player->countryCode = UnityTowerVisitor_GetCountry(playerInfo);
    player->localCode = UnityTowerVisitor_GetProvince(playerInfo);
    player->gender = getTrainerGender(playerInfo);
    player->trainerType = func_02008554(playerInfo);
    player->trainerType = func_02025c88(player->trainerType);
    for (i = 0; i < PLAYER_MESSAGE_COUNT; i++) {
        func_0200e6fc(cgear, i + 1, (PMSData *)player->messages[i]);
    }
    func_0200e6fc(cgear, 4, (PMSData *)player->leaderMessage);
    player->result = func_0200dcf8(score);
    func_0200dc74(score, 1, player->pokemon);
}

// Ends the check's request when the Wi-Fi library reports an error
// pokebw2's WifiBSubwayUpload_OnError
void func_ov116_021d6e3c(void *work, int code, int type, int error) {
    WifiBSubwayUpload *upload = work;

    if (upload->nhttp != NULL) {
        func_ov114_021b9774(upload->nhttp);
    }
}

// Asks the server to check the team's Pokémon
void func_ov116_021d6e50(WifiBSubwayUpload *upload, WifiBSubwayError *error, void *loginBuffer,
                                           HeapID heapId) {
    upload->nhttp = func_ov114_021b9800(heapId, func_0200853c(upload->playerInfo), loginBuffer);
    func_ov009_02156dc4(func_ov116_021d6e3c, upload);
    func_ov114_021b98a0(upload->nhttp, heapId, sizeof(upload->player.pokemon), 4);
    func_ov114_021b98d8(upload->nhttp, upload->player.pokemon, sizeof(upload->player.pokemon));
    upload->pokemonCount = TEAM_POKEMON_COUNT;
    func_ov114_021b98f8(upload->nhttp);
    func_ov114_021b9748(upload->nhttp);
    func_ov116_021d7258(error);
}

// Returns a CHECK_RESULT_*. A Pokémon that fails the check gets its species' name for a nickname, to be checked
// again; on the last check, the failure is an error instead
// pokebw2's WifiBSubwayUpload_WaitCheckPokemon
u32 func_ov116_021d6ec4(WifiBSubwayUpload *upload, WifiBSubwayError *error, BOOL last) {
    u32 result = CHECK_RESULT_WAIT;
    int status = func_ov114_021b99dc(upload->nhttp);
    int poll = func_ov114_021b9790(upload->nhttp);
    void *answer;
    int i;

    if (status != 200 && poll == 0) {
        func_ov116_021d7250(error, status);
        func_ov114_021b9774(upload->nhttp);
        func_ov114_021b99b8(upload->nhttp);
        func_ov114_021b9838(upload->nhttp);
        upload->nhttp = NULL;
        func_ov009_02156dc4(NULL, NULL);
        return CHECK_RESULT_NG;
    }
    if (poll != 0) {
        func_ov116_021d7260(error);
    } else {
        answer = func_ov114_021b97f4(upload->nhttp);
        if (func_ov114_021b99f0(answer) == 0) {
            BOOL ok = TRUE;

            MI_CpuCopy8(func_ov114_021b9a14(answer, upload->pokemonCount), upload->sign, sizeof(upload->sign));
            for (i = 0; i < upload->pokemonCount; i++) {
                if (func_ov114_021b99f4(answer, i)) {
                    ok = FALSE;
                    if (last) {
                        func_ov116_021d724c(error, CHECK_ERROR_POKEMON);
                    } else {
                        func_ov116_021d7010(upload, i);
                    }
                }
            }
            if (ok) {
                result = CHECK_RESULT_OK;
            } else {
                result = CHECK_RESULT_NG;
            }
        } else {
            if (last) {
                func_ov116_021d724c(error, CHECK_ERROR_STATUS);
            }
            result = CHECK_RESULT_NG;
            for (i = 0; i < upload->pokemonCount; i++) {
                if (func_ov114_021b99f4(answer, i)) {
                    func_ov116_021d7010(upload, i);
                }
            }
        }
        func_ov114_021b99b8(upload->nhttp);
        func_ov114_021b9838(upload->nhttp);
        upload->nhttp = NULL;
        func_ov009_02156dc4(NULL, NULL);
    }
    return result;
}

// pokebw2's WifiBSubwayUpload_ResetNickname
void func_ov116_021d7010(WifiBSubwayUpload *upload, int index) {
    BSubwayPokemon *pokemon = (BSubwayPokemon *)upload->player.pokemon;

    GFL_MsgDataLoadRawStr(g_PMLSpeciesNamesResident, pokemon[index].species, pokemon[index].nickname,
                          NELEMS(pokemon[index].nickname));
}

// pokebw2's WifiBSubwayUpload_CancelCheck
void func_ov116_021d7038(WifiBSubwayUpload *upload) {
    if (upload->nhttp != NULL) {
        func_ov009_02156dc4(NULL, NULL);
        func_ov114_021b99b8(upload->nhttp);
        func_ov114_021b9838(upload->nhttp);
        upload->nhttp = NULL;
    }
}

// Sends the record, for the train the player's saved trainers are of
// pokebw2's WifiBSubwayUpload_Send
void func_ov116_021d7060(WifiBSubwayUpload *upload, WifiBSubwayError *error) {
    u32 rank = func_0200de94(upload->wifiData);
    u32 number = func_0200dea0(upload->wifiData);
    u32 rounds = func_0200dcfc(upload->score);

    func_ov114_021c9530((u8)rank, (u8)number, (u8)rounds, &upload->player, upload->sign, sizeof(upload->sign));
    func_ov116_021d70e4(error);
}

// pokebw2's WifiBSubwayUpload_WaitSend
BOOL func_ov116_021d70b0(WifiBSubwayUpload *upload, WifiBSubwayError *error) {
    s32 result;

    if (func_ov116_021d70ec(error, &result) && !func_ov116_021d7158(error)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiBSubwayError_Init
static void func_ov116_021d70d8(WifiBSubwayError *error) {
    MI_CpuFill8(error, 0, sizeof(WifiBSubwayError));
}

// Starts the wait for a request of the server library
// pokebw2's WifiBSubwayError_StartDpw
void func_ov116_021d70e4(WifiBSubwayError *error) {
    error->timeout = FALSE;
    error->timer = 0;
}

// Returns whether the request ended, giving its result if it went well; one that takes too long is canceled
// pokebw2's WifiBSubwayError_WaitDpw
BOOL func_ov116_021d70ec(WifiBSubwayError *error, s32 *result) {
    BOOL done = FALSE;

    if (!func_ov114_021c93d0()) {
        error->timer++;
        if (error->timer == REQUEST_TIMEOUT) {
            func_ov114_021c9634();
            error->timeout = TRUE;
        }
    } else {
        s32 dpwResult = func_ov114_021c940c();

        done = TRUE;
        switch (dpwResult) {
        case -7:
        case -6:
        case -4:
        case -3:
        case -2:
        case -1:
            error->dpwResult = dpwResult;
            break;
        case -5:
            GFL_NetErrShow(TRUE);
            break;
        default:
            if (dpwResult >= 0) {
                *result = dpwResult;
            } else {
                GFL_NetErrShow(TRUE);
            }
            break;
        }
    }
    return done;
}

// pokebw2's WifiBSubwayError_Check
BOOL func_ov116_021d7158(const WifiBSubwayError *error) {
    if (error->dpwResult < 0) {
        return TRUE;
    }
    if (error->serverState != 0) {
        return TRUE;
    }
    if (error->checkError != CHECK_ERROR_NONE) {
        return TRUE;
    }
    if (error->timeout) {
        return TRUE;
    }
    return FALSE;
}

// The message of the error
// pokebw2's WifiBSubwayError_GetMessage
u32 func_ov116_021d7184(const WifiBSubwayError *error) {
    u32 message = 0;

    if (error->dpwResult < 0) {
        switch (error->dpwResult) {
        case -2:
            message = Bank0413_Text_016;
            break;
        case -6:
            message = Bank0413_Text_017;
            break;
        case -1:
            message = Bank0413_Text_015;
            break;
        case -7:
            message = Bank0413_Text_019;
            break;
        case -4:
            message = Bank0413_Text_018;
            break;
        case -3:
            message = Bank0413_Text_020;
            break;
        default:
            message = Bank0413_Text_022;
            break;
        }
    } else if (error->serverState != 0) {
        switch (error->serverState) {
        case 1:
            message = Bank0413_Text_013;
            break;
        case 2:
            message = Bank0413_Text_014;
            break;
        }
    } else if (error->checkError != CHECK_ERROR_NONE) {
        switch (error->checkError) {
        case CHECK_ERROR_POKEMON:
            message = Bank0413_Text_023;
            break;
        case CHECK_ERROR_STATUS:
            message = Bank0413_Text_023;
            break;
        }
    } else if (error->timeout) {
        message = Bank0413_Text_016;
    }
    return message;
}

// Records the Wi-Fi error of a failed check, for the error screen; returns whether there was one
// pokebw2's WifiBSubwayError_ShowHttpError
BOOL func_ov116_021d7200(const WifiBSubwayError *error) {
    if (error->checkError == CHECK_ERROR_HTTP) {
        u32 message;

        switch (error->httpStatus) {
        case 200:
        case 400:
            message = 0x41;
            break;
        case 401:
            message = 0x42;
            break;
        case 408:
            message = 0x44;
            break;
        default:
            message = 0x43;
            break;
        }
        func_0200fb4c(message);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiBSubwayError_SetCheckError
void func_ov116_021d724c(WifiBSubwayError *error, u32 checkError) {
    error->checkError = checkError;
}

// pokebw2's WifiBSubwayError_SetHttpError
void func_ov116_021d7250(WifiBSubwayError *error, s32 status) {
    error->checkError = CHECK_ERROR_HTTP;
    error->httpStatus = status;
}

// Starts the wait for the check's request
// pokebw2's WifiBSubwayError_StartHttp
void func_ov116_021d7258(WifiBSubwayError *error) {
    error->timeout = FALSE;
    error->timer = 0;
}

// pokebw2's WifiBSubwayError_CountHttp
void func_ov116_021d7260(WifiBSubwayError *error) {
    error->timer++;
    if (error->timer == REQUEST_TIMEOUT) {
        error->timeout = TRUE;
    }
}

// Opens the code input screen for a train's number
// pokebw2's WifiBSubway_CallRoomInput
void func_ov116_021d7278(WifiBSubwayWork *wk, HeapID heapId) {
    data_ov116_021d8568[0] = func_ov116_021d6a4c(&wk->room);
    wk->codeIn = func_0202e4ec(heapId, 1, func_ov116_021d6a48(&wk->room), 3, data_ov116_021d8568);
    GSYS_QueueProc(wk->param->gsys, OVERLAY_CODEIN, &data_ov129_021da62c, wk->codeIn);
}

// Opens the code input screen for a rank
// pokebw2's WifiBSubway_CallRankInput
void func_ov116_021d72d0(WifiBSubwayWork *wk, HeapID heapId) {
    wk->codeIn = func_0202e4ec(heapId, 2, RANK_MAX, 2, data_ov116_021d8574);
    GSYS_QueueProc(wk->param->gsys, OVERLAY_CODEIN, &data_ov129_021da62c, wk->codeIn);
}

// pokebw2's WifiBSubway_FreeCodeIn
void func_ov116_021d730c(WifiBSubwayWork *wk) {
    if (wk->codeIn != NULL) {
        func_0202e54c(wk->codeIn);
        wk->codeIn = NULL;
    }
}

// The number typed, from ASCII or full-width digits
// pokebw2's WifiBSubway_GetCodeInNumber
u32 func_ov116_021d7324(WifiBSubwayWork *wk) {
    u16 terminator = GFL_StrBufGetTerminator();
    const u16 *chars = GFL_StrBufGetStringPtr(wk->codeIn->str);
    u32 number = 0;
    u16 i;

    for (i = 0; chars[i] != terminator; i++) {
        u16 digit = chars[i] - L'0';

        if (digit >= 10) {
            digit = chars[i] - 0xff10;
        }
        number *= 10;
        number += digit;
    }
    return number;
}

// pokebw2's WifiBSubway_CallLogin
void func_ov116_021d737c(WifiBSubwayWork *wk) {
    wk->login.gameData = wk->gameData;
    wk->login.unk4 = 0;
    wk->login.unk18 = TRUE;
    wk->login.unk8 = 1;
    wk->login.unkC = 0x2c;
    wk->login.buffer = wk->loginBuffer;
    GSYS_QueueProc(wk->param->gsys, OVERLAY_WIFILOGIN, &data_ov115_021d6338, &wk->login);
}

// pokebw2's WifiBSubway_FreeLogin
static void func_ov116_021d73d0(WifiBSubwayWork *wk) {
}

// pokebw2's WifiBSubway_GetLoginResult
u32 func_ov116_021d73d4(const WifiBSubwayWork *wk) {
    return wk->login.result;
}

// pokebw2's WifiBSubway_CallLogout
void func_ov116_021d73e0(WifiBSubwayWork *wk) {
    wk->logout.gameData = wk->gameData;
    wk->logout.unk4 = 0;
    wk->logout.unk8 = 1;
    wk->logout.unkC = 1;
    GSYS_QueueProc(wk->param->gsys, OVERLAY_WIFILOGIN, &data_ov115_021d654c, &wk->logout);
}

// pokebw2's WifiBSubway_FreeLogout
static void func_ov116_021d7420(WifiBSubwayWork *wk) {
}

// Runs the mode's step, and returns a MODE_RESULT_*
// pokebw2's WifiBSubway_RunMode
u32 func_ov116_021d7424(WifiBSubwayWork *wk, HeapID heapId) {
    WifiBSubwayModeFunc funcs[] = {
        func_ov116_021d7450,
        func_ov116_021d7550,
        func_ov116_021d77d0,
    };

    return funcs[wk->param->mode](wk, heapId);
}

// Sends the record: the team's Pokémon are checked, a second time if one failed, and the record sent and marked sent
// pokebw2's WifiBSubway_UploadMain
u32 func_ov116_021d7450(WifiBSubwayWork *wk, HeapID heapId) {
    switch (wk->seq) {
    case 0:
        func_ov116_021d8010(&wk->view, Bank0413_Text_007);
        wk->seq++;
        break;
    case 1:
        func_ov116_021d6e50(&wk->upload, &wk->error, wk->loginBuffer, heapId);
        wk->seq++;
        break;
    case 2: {
        u32 result = func_ov116_021d6ec4(&wk->upload, &wk->error, FALSE);

        if (result == CHECK_RESULT_OK) {
            wk->seq = 5;
        } else if (result == CHECK_RESULT_NG) {
            wk->seq = 3;
        }
        break;
    }
    case 3:
        func_ov116_021d6e50(&wk->upload, &wk->error, wk->loginBuffer, heapId);
        wk->seq++;
        break;
    case 4:
        if (func_ov116_021d6ec4(&wk->upload, &wk->error, TRUE) == CHECK_RESULT_OK) {
            wk->seq = 5;
        }
        break;
    case 5:
        func_ov116_021d7060(&wk->upload, &wk->error);
        wk->seq++;
        break;
    case 6:
        if (func_ov116_021d70b0(&wk->upload, &wk->error) && func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 7:
        func_0200dba0(wk->score, 1, BSUBWAY_FLAG_RESET);
        func_ov116_021d81d0(wk);
        wk->seq++;
        break;
    case 8:
        if (func_ov116_021d81ec(wk)) {
            func_ov116_021d81b4(&wk->view);
            func_ov116_021d8010(&wk->view, Bank0413_Text_008);
            wk->seq++;
        }
        break;
    case 9:
        if (func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 10:
        return MODE_RESULT_END;
    }
    return MODE_RESULT_CONTINUE;
}

// Downloads a train of the player's rank: the player types its number, and a train already taken today is refused
// pokebw2's WifiBSubway_DownloadTrainMain
u32 func_ov116_021d7550(WifiBSubwayWork *wk, HeapID heapId) {
    RTCTime time;
    RTCDate date;

    switch (wk->seq) {
    case 0:
        func_ov116_021d69a8(&wk->room, &wk->error, func_0200dc08(wk->score));
        wk->seq++;
        break;
    case 1:
        if (func_ov116_021d69c0(&wk->room, &wk->error)) {
            wk->seq++;
        }
        break;
    case 2:
        func_ov116_021d81b4(&wk->view);
        func_ov116_021d8118(&wk->view, 0, func_ov116_021d6a48(&wk->room),
                                  func_ov116_021d6a4c(&wk->room));
        func_ov116_021d8010(&wk->view, Bank0413_Text_000);
        wk->seq++;
        break;
    case 3:
        if (func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 4:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 1);
        wk->seq++;
        break;
    case 5:
        if (!GFL_FadeIsRunning()) {
            func_ov116_021d7c58(&wk->view);
            wk->seq++;
        }
        break;
    case 6:
        func_ov116_021d7278(wk, heapId);
        wk->seq++;
        break;
    case 7:
        wk->inputNumber = func_ov116_021d7324(wk);
        func_ov116_021d730c(wk);
        func_ov116_021d7b70(&wk->view, heapId);
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 1);
        wk->seq++;
        break;
    case 8:
        if (!GFL_FadeIsRunning()) {
            u32 rank = func_ov116_021d6a40(&wk->room);
            u32 number = wk->inputNumber;
            u32 count = func_ov116_021d6a48(&wk->room);

            DWC_GetDateTime(&date, &time);
            if (number < 1 || number > count) {
                wk->seq = 16;
            } else if (func_0200ddd0(wk->wifiData, rank, number, &date)) {
                wk->seq = 14;
            } else {
                func_ov116_021d69ec(&wk->room, &wk->error, wk->inputNumber);
                func_ov116_021d8118(&wk->view, 0, func_ov116_021d6a44(&wk->room),
                                          func_ov116_021d6a4c(&wk->room));
                func_ov116_021d8118(&wk->view, 1, func_ov116_021d6a40(&wk->room), 2);
                func_ov116_021d8010(&wk->view, Bank0413_Text_001);
                func_ov116_021d8190(&wk->view);
                wk->seq++;
            }
        }
        break;
    case 9:
        if (func_ov116_021d6a10(&wk->room, &wk->error) && func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 10:
        func_ov116_021d6a5c(&wk->room);
        func_ov116_021d6af0(&wk->room, wk->wifiData);
        func_ov116_021d6b30(&wk->room, wk->wifiData);
        func_ov116_021d81d0(wk);
        wk->seq++;
        break;
    case 11:
        if (func_ov116_021d81ec(wk)) {
            func_ov116_021d81b4(&wk->view);
            func_ov116_021d8010(&wk->view, Bank0413_Text_003);
            wk->seq++;
        }
        break;
    case 12:
        if (func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 13:
        return MODE_RESULT_END;
    case 14:
        func_ov116_021d8010(&wk->view, Bank0413_Text_002);
        wk->seq++;
        break;
    case 15:
        if (func_ov116_021d80cc(&wk->view)) {
            wk->seq = 2;
        }
        break;
    case 16:
        func_ov116_021d8010(&wk->view, Bank0413_Text_004);
        wk->seq++;
        break;
    case 17:
        if (func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 18:
        func_ov116_021d8134(&wk->view);
        wk->seq++;
        break;
    case 19: {
        u32 result = func_ov116_021d8168(&wk->view);

        if (result == 0) {
            return MODE_RESULT_CANCEL;
        } else if (result == BMPMENU_CANCEL) {
            wk->seq = 2;
        }
        break;
    }
    }
    return MODE_RESULT_CONTINUE;
}

// Downloads the leaders of a train: the player types its rank and its number
// pokebw2's WifiBSubway_DownloadLeadersMain
u32 func_ov116_021d77d0(WifiBSubwayWork *wk, HeapID heapId) {
    switch (wk->seq) {
    case 0:
        func_ov116_021d81b4(&wk->view);
        func_ov116_021d8010(&wk->view, Bank0413_Text_005);
        wk->seq++;
        break;
    case 1:
        if (func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 2:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 1);
        wk->seq++;
        break;
    case 3:
        if (!GFL_FadeIsRunning()) {
            func_ov116_021d7c58(&wk->view);
            wk->seq++;
        }
        break;
    case 4:
        func_ov116_021d72d0(wk, heapId);
        wk->seq++;
        break;
    case 5:
        wk->inputNumber = func_ov116_021d7324(wk);
        func_ov116_021d730c(wk);
        func_ov116_021d7b70(&wk->view, heapId);
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 1);
        wk->seq++;
        break;
    case 6:
        if (!GFL_FadeIsRunning()) {
            if (wk->inputNumber >= 1 && wk->inputNumber <= RANK_MAX) {
                func_ov116_021d69a8(&wk->room, &wk->error, wk->inputNumber);
                func_ov116_021d8010(&wk->view, Bank0413_Text_026);
                func_ov116_021d8190(&wk->view);
                wk->seq++;
            } else {
                wk->seq = 19;
            }
        }
        break;
    case 7:
        if (func_ov116_021d69c0(&wk->room, &wk->error)) {
            func_ov116_021d81b4(&wk->view);
            func_ov116_021d8118(&wk->view, 0, func_ov116_021d6a48(&wk->room),
                                      func_ov116_021d6a4c(&wk->room));
            func_ov116_021d8010(&wk->view, Bank0413_Text_006);
            wk->seq++;
        }
        break;
    case 8:
        if (func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 9:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 1);
        wk->seq++;
        break;
    case 10:
        if (!GFL_FadeIsRunning()) {
            func_ov116_021d7c58(&wk->view);
            wk->seq++;
        }
        break;
    case 11:
        func_ov116_021d7278(wk, heapId);
        wk->seq++;
        break;
    case 12:
        wk->inputNumber = func_ov116_021d7324(wk);
        func_ov116_021d730c(wk);
        func_ov116_021d7b70(&wk->view, heapId);
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 1);
        wk->seq++;
        break;
    case 13:
        if (!GFL_FadeIsRunning()) {
            u32 count = func_ov116_021d6a48(&wk->room);

            if (wk->inputNumber < 1 || wk->inputNumber > count) {
                wk->seq = 19;
            } else {
                func_ov116_021d69ec(&wk->room, &wk->error, wk->inputNumber);
                func_ov116_021d8118(&wk->view, 0, func_ov116_021d6a44(&wk->room),
                                          func_ov116_021d6a4c(&wk->room));
                func_ov116_021d8118(&wk->view, 1, func_ov116_021d6a40(&wk->room), 2);
                func_ov116_021d8010(&wk->view, Bank0413_Text_001);
                func_ov116_021d8190(&wk->view);
                wk->seq++;
            }
        }
        break;
    case 14:
        if (func_ov116_021d6a10(&wk->room, &wk->error) && func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 15:
        func_ov116_021d6a5c(&wk->room);
        func_ov116_021d6b30(&wk->room, wk->wifiData);
        func_ov116_021d81d0(wk);
        wk->seq++;
        break;
    case 16:
        if (func_ov116_021d81ec(wk)) {
            func_ov116_021d81b4(&wk->view);
            func_ov116_021d8010(&wk->view, Bank0413_Text_003);
            wk->seq++;
        }
        break;
    case 17:
        if (func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 18:
        return MODE_RESULT_END;
    case 19:
        func_ov116_021d8010(&wk->view, Bank0413_Text_009);
        wk->seq++;
        break;
    case 20:
        if (func_ov116_021d80cc(&wk->view)) {
            wk->seq++;
        }
        break;
    case 21:
        func_ov116_021d8134(&wk->view);
        wk->seq++;
        break;
    case 22: {
        u32 result = func_ov116_021d8168(&wk->view);

        if (result == 0) {
            return MODE_RESULT_CANCEL;
        } else if (result == BMPMENU_CANCEL) {
            func_ov116_021d6998(&wk->room);
            wk->seq = 0;
        }
        break;
    }
    }
    return MODE_RESULT_CONTINUE;
}

// Starts the server library and asks for the server's state
// pokebw2's WifiBSubway_Connect
void func_ov116_021d7a3c(WifiBSubwayWork *wk) {
    DWCUserData *userData = func_02009860(wk->wifiList);
    s32 profileId;

    func_02008540(wk->playerInfo, func_02009cc0(wk->wifiList));
    profileId = func_0200853c(wk->playerInfo);
    func_ov114_021c8f2c(profileId, func_0205994c(userData), 0);
    func_ov114_021c96cc();
    func_ov116_021d70e4(&wk->error);
    wk->dpwActive = TRUE;
}

// pokebw2's WifiBSubway_WaitConnect
BOOL func_ov116_021d7a94(WifiBSubwayWork *wk) {
    s32 result;

    return func_ov116_021d70ec(&wk->error, &result);
}

// Cancels the request that runs, if any
// pokebw2's WifiBSubway_Disconnect
void func_ov116_021d7aa8(WifiBSubwayWork *wk) {
    if (!func_ov114_021c93d0()) {
        func_ov114_021c9634();
        func_ov116_021d70e4(&wk->error);
    }
}

// pokebw2's WifiBSubway_WaitDisconnect
BOOL func_ov116_021d7ac8(WifiBSubwayWork *wk) {
    s32 result;

    if (!wk->dpwActive) {
        return TRUE;
    }
    if (func_ov116_021d70ec(&wk->error, &result)) {
        func_ov114_021c93bc();
        wk->dpwActive = FALSE;
        return TRUE;
    }
    return FALSE;
}

// Prints the message of the screen's own error and returns TRUE; an error of the Wi-Fi library or the network is left
// to the error screen
// pokebw2's WifiBSubway_PrintError
BOOL func_ov116_021d7b00(WifiBSubwayWork *wk) {
    if (func_ov009_02157170(TRUE, FALSE)) {
        return FALSE;
    }
    if (func_0200fb24()) {
        func_0203ca98();
        return FALSE;
    }
    if (func_ov116_021d7158(&wk->error)) {
        u32 message;

        if (func_ov116_021d7200(&wk->error)) {
            return FALSE;
        }
        message = func_ov116_021d7184(&wk->error);
        func_ov116_021d81b4(&wk->view);
        func_ov116_021d8010(&wk->view, message);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiBSubway_WaitError
BOOL func_ov116_021d7b64(WifiBSubwayWork *wk) {
    return func_ov116_021d80cc(&wk->view);
}

// pokebw2's WifiBSubwayView_Init
void func_ov116_021d7b70(WifiBSubwayView *view, HeapID heapId) {
    view->graphic = func_ov116_021d821c(1, heapId);
    view->heapId = heapId;
    func_ov116_021d7d0c(view, heapId);
    view->window = BmpWin_CreateDynamic(MESSAGE_BG, 1, 19, 30, 4, 12, TRUE);
    BmpWin_DrawFrame(view->window, 1, view->messageFrameChars, FRAME_PALETTE);
    GFL_BitmapFill(BmpWin_GetBitmap(view->window), 15);
    BmpWin_FlushMap(view->window);
    BmpWin_FlushChar(view->window);
    GFL_BGSysQueueScrLoad(MESSAGE_BG);
    view->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0413, heapId);
    view->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    view->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    view->message = GFL_StrBufCreate(0x80, heapId);
    view->expanded = GFL_StrBufCreate(0x80, heapId);
    view->tcbManager = GFL_TCBExMgrCreate(heapId, heapId, 1, 0);
    view->initialized = TRUE;
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    func_0203d34c(TRUE, heapId);
}

// pokebw2's WifiBSubwayView_Free
static void func_ov116_021d7c58(WifiBSubwayView *view) {
    if (view->initialized) {
        if (view->stream != NULL) {
            func_0201ce48(view->stream);
        }
        GFL_TCBExMgrFree(view->tcbManager);
        GFL_MsgDataFree(view->msgData);
        GFL_WordSetSystemFree(view->wordSet);
        GFL_FontFree(view->font);
        GFL_StrBufFree(view->message);
        GFL_StrBufFree(view->expanded);
        func_ov116_021d7fc4(view);
        func_ov116_021d81b4(view);
        if (view->yesNo != NULL) {
            func_0201fef4(view->yesNo);
            view->yesNo = NULL;
        }
        BmpWin_Free(view->window);
        func_ov116_021d82ac(view->graphic);
        func_0201dd68();
        MI_CpuFill8(view, 0, sizeof(WifiBSubwayView));
    }
}

// pokebw2's WifiBSubwayView_Update
void func_ov116_021d7cf4(WifiBSubwayView *view) {
    if (view->initialized) {
        GFL_TCBExMgrUpdate(view->tcbManager);
    }
}

// Loads the palettes the cycle blends between, makes the blends and starts the cycle
static inline void WifiBSubwayView_InitCycle(WifiBSubwayView *view, ArcTool *arc, u32 heapId) {
    int i;
    int count = 0;
    NNSG2dPaletteData *palette;
    void *file;

    MI_CpuFill8(&view->cycle, 0, sizeof(view->cycle));
    file = GFL_G2DIOReadNCLRArc(arc, 2, &palette, heapId);
    MI_CpuCopy8(palette->pRawData, view->cycle.palettes, sizeof(view->cycle.palettes));
    MI_CpuCopy8(palette->pRawData, view->cycle.blended, sizeof(view->cycle.palettes));
    GFL_HeapFree(file);

    // Colors 4 to 10 of each palette, blended into the next in 4 steps
    for (i = 0; i < PALETTE_COUNT; i++) {
        int next = i + 1;
        fx32 fraction;
        BOOL done;

        if (next >= PALETTE_COUNT) {
            next -= PALETTE_COUNT;
        }
        fraction = 0;
        done = FALSE;
        while (TRUE) {
            int j;

            for (j = 4; j < 11; j++) {
                func_02021f64(&view->cycle.palettes[i][j], &view->cycle.blended[count][j], 1, fraction >> 8,
                            view->cycle.palettes[next][j]);
            }
            count++;
            if (done == TRUE) {
                break;
            }
            fraction += FX32_ONE * 6 / 16;
            if (fraction >= FX32_ONE) {
                fraction = FX32_ONE;
                done = TRUE;
            }
        }
    }
    DC_FlushRange(view->cycle.blended, sizeof(view->cycle.blended));
    view->cycle.active = TRUE;
    view->cycle.task = GFL_VBlankTCBAdd(func_ov116_021d7f54, &view->cycle, 20);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG3, 8, 16);
    view->cycle.scrollBg = 0;
}

// Loads the Wi-Fi login screen's BGs and the frames of the message window and the yes/no menu
// pokebw2's WifiBSubwayView_LoadGraphics
void func_ov116_021d7d0c(WifiBSubwayView *view, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_PDWACC, heapId);
    u32 chars;

    GFL_G2DIOLoadArcNCLRDefault(arc, 3, PALTYPE_SUB_BG, 0, 0, heapId);
    chars = GFL_BGSysLoadArcNCGRDynamic(arc, 4, 4, 0, FALSE, heapId);
    GFL_G2DIOLoadNSCRSync(arc, 6, 4, 0, (u16)chars, 0, FALSE, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 3, PALTYPE_MAIN_BG, 0, 0, heapId);
    chars = GFL_BGSysLoadArcNCGRDynamic(arc, 4, 0, 0, FALSE, heapId);
    GFL_G2DIOLoadNSCRSync(arc, 5, 0, 0, (u16)chars, 0, FALSE, heapId);
    chars = GFL_BGSysLoadArcNCGRDynamic(arc, 4, 3, 0, FALSE, heapId);
    GFL_G2DIOLoadNSCRSync(arc, 7, 3, 0, (u16)chars, 0, FALSE, heapId);
    WifiBSubwayView_InitCycle(view, arc, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysAllocChar(MESSAGE_BG, BGSYS_TILE_SIZE_16, FALSE);
    GFL_BGSysAllocChar(YES_NO_BG, BGSYS_TILE_SIZE_16, FALSE);
    view->messageFrameChars = LoadCursorImageEndOfHeap(MESSAGE_BG, FRAME_PALETTE, 0, heapId);
    view->yesNoFrameChars = LoadCursorImageEndOfHeap(YES_NO_BG, FRAME_PALETTE, 0, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 12 * 0x20, 0x20, heapId);
}

// pokebw2's WifiBSubwayView_CyclePalette
void func_ov116_021d7f54(TCB *task, void *work) {
    WifiBSubwayPaletteCycle *cycle = work;

    if (cycle->active) {
        cycle->skip ^= 1;
        if (!(cycle->skip & 1)) {
            GX_LoadBGPltt(&cycle->blended[cycle->frame][4], 0x20 + 4 * 2, 7 * 2);
            GXS_LoadBGPltt(&cycle->blended[cycle->frame][4], 0x20 + 4 * 2, 7 * 2);
            cycle->frame++;
            if (cycle->frame >= PALETTE_COUNT * BLEND_STEPS) {
                cycle->frame = 0;
            }
            GFL_BGSysMoveBG(cycle->scrollBg, BG_MOVE_LEFT, 7);
        }
    }
}

// pokebw2's WifiBSubwayView_FreeGraphics
static void func_ov116_021d7fc4(WifiBSubwayView *view) {
    GFL_BGSysFreeCharMemory(MESSAGE_BG, CHAR_POS(view->messageFrameChars), CHAR_SIZE(view->messageFrameChars));
    GFL_BGSysFreeCharMemory(YES_NO_BG, CHAR_POS(view->yesNoFrameChars), CHAR_SIZE(view->yesNoFrameChars));
    if (view->cycle.task != NULL) {
        GFL_TCBRemove(view->cycle.task);
        view->cycle.task = NULL;
        view->cycle.active = FALSE;
    }
    G2_BlendNone();
}

// Prints a message into the window, with the numbers that were set
// pokebw2's WifiBSubwayView_Print
void func_ov116_021d8010(WifiBSubwayView *view, u32 message) {
    if (view->stream != NULL) {
        func_0201ce48(view->stream);
        view->stream = NULL;
    }
    GFL_MsgDataLoadStrbuf(view->msgData, message, view->message);
    GFL_WordSetFormatStrbuf(view->wordSet, view->expanded, view->message);
    GFL_BitmapFill(BmpWin_GetBitmap(view->window), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    view->stream = func_0201cce4(view->window, 0, 0, view->expanded, view->font, func_02012ffc(), view->tcbManager, 2,
                                 view->heapId, 15);
    BmpWin_DrawFrame(view->window, 1, view->messageFrameChars, FRAME_PALETTE);
    BmpWin_FlushChar(view->window);
}

// Returns whether the message is printed, going on at the A Button or a touch where it waits
// pokebw2's WifiBSubwayView_WaitPrint
BOOL func_ov116_021d80cc(WifiBSubwayView *view) {
    u32 state;

    if (view->stream == NULL) {
        return TRUE;
    }
    state = func_0201ce30(view->stream);
    if (state == PRINT_STREAM_DONE) {
        func_0201ce48(view->stream);
        view->stream = NULL;
        return TRUE;
    }
    if (state == PRINT_STREAM_PAUSED) {
        if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A || func_02035c60()) {
            func_0201ce38(view->stream);
        }
    }
    return FALSE;
}

// pokebw2's WifiBSubwayView_SetNumber
void func_ov116_021d8118(WifiBSubwayView *view, u32 index, u32 number, u32 digits) {
    WordSetNumber(view->wordSet, index, number, digits, 2, TRUE);
}

// pokebw2's WifiBSubwayView_StartYesNo
void func_ov116_021d8134(WifiBSubwayView *view) {
    data_ov116_021d8560.unk4 = view->yesNoFrameChars;
    view->yesNo = ShopUI_CreateConfirmDialog(&data_ov116_021d8560, view->yesNoFrameChars, FRAME_PALETTE, 0,
                                             view->heapId);
}

// Returns 0 for yes, BMPMENU_CANCEL for no, also when there is no menu, and BMPMENU_NULL until the player answers
// pokebw2's WifiBSubwayView_UpdateYesNo
u32 func_ov116_021d8168(WifiBSubwayView *view) {
    u32 result = BMPMENU_CANCEL;

    if (view->yesNo != NULL) {
        result = func_0201febc(view->yesNo);
        if (result != BMPMENU_NULL) {
            view->yesNo = NULL;
        }
    }
    return result;
}

// pokebw2's WifiBSubwayView_StartWaitIcon
void func_ov116_021d8190(WifiBSubwayView *view) {
    view->waitIcon = func_0202ee30(GFL_VBlankGetTCBMgr(), view->window, 15, 16, view->heapId);
}

// pokebw2's WifiBSubwayView_StopWaitIcon
static void func_ov116_021d81b4(WifiBSubwayView *view) {
    if (view->waitIcon != NULL) {
        func_0202f038(view->waitIcon);
        view->waitIcon = NULL;
    }
}

// pokebw2's WifiBSubway_StartSave
void func_ov116_021d81d0(WifiBSubwayWork *wk) {
    func_02012dac(wk->gameData);
    wk->saving = TRUE;
}

// pokebw2's WifiBSubway_WaitSave
BOOL func_ov116_021d81ec(WifiBSubwayWork *wk) {
    if (!wk->saving) {
        return TRUE;
    }
    if (func_02012dd0(wk->gameData) == 2) {
        wk->saving = FALSE;
        return TRUE;
    }
    return FALSE;
}
