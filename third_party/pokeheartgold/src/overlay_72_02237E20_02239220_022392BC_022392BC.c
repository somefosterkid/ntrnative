#include <dwc.h>

#include "global.h"
#include "sprite_resources_header.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_format.h"
#include "frontier_pokemon.h"
#include "wifi_battle_tower_match_candidate.h"
#include "save_frontier.h"
#include "sav_system_info.h"
#include "overlay_70_gtsapplication_state.h"
#include "msgdata.h"

struct BgConfig;

struct BoxPokemon;

struct DWCstAccLoginId;

struct DWCstAccUserData;

struct DWCstInetControl;

struct FrontierPokemon;

struct G2dRenderer;

struct Menu;

struct MsgData;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct SysInfo;

struct WifiBattleTowerAppState;

struct WifiBattleTowerDownloadBuffer;

struct WifiBattleTowerDownloadData;

struct WifiBattleTowerMatchCandidate;

struct WifiBattleTowerRecord;

struct WifiPlayerProfile;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;



typedef u16 charcode_t;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct WifiPlayerProfile {
    FrontierPokemon mons[3];
    charcode_t name[7 + 1];
    u8 version;
    u8 language;
    u8 country;
    u8 region;
    u8 trainerInfoId[4];
    s8 no1Sentence[8];
    union {
        struct {
            u8 sLotoId : 1;
            u8 gender : 1;
            u8 : 6;
        };
        u8 unused2;
    };
    u8 appearance;
    s8 battleSentences[24];
    u16 ratingScore;
} WifiPlayerProfile;

typedef struct WifiBattleTowerDownloadBuffer {
    WifiPlayerProfile wifiPlayerProfile[7];
    WifiBattleTowerMatchCandidate matchCandidates[30];
} WifiBattleTowerDownloadBuffer;

typedef struct {
    WifiBattleTowerRecord *record;
    WifiBattleTowerDownloadData *downloadData;
    SysInfo *systemData;
    SaveData *saveData;
    Options *options;
    DWCUserData *userData;
    int mode;
    int profileId;
    int unk_20;
    int unk_24;
} UnkStruct_0206BC70;

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

typedef struct Menu Menu;

enum WifiBattleTowerState {
    BT_STATE_INITIAL_PROMPT = 0,
    BT_STATE_INITIAL_YES_NO_MENU,
    BT_STATE_INIT_INET,
    BT_STATE_WAIT_INET_CONNECT,
    BT_STATE_INIT_NAS_LOGIN,
    BT_STATE_WAIT_NAS_LOGIN,
    BT_STATE_INIT_HTTP,
    BT_STATE_REQUEST_INFO,
    BT_STATE_WAIT_INFO,
    BT_STATE_SET_PROFILE,
    BT_STATE_WAIT_SET_PROFILE,
    BT_STATE_REQUEST_ROOM_COUNT,
    BT_STATE_WAIT_ROOM_COUNT,
    BT_STATE_SELECT_OPPONENT_PROMPT,
    BT_STATE_DRAW_OPPONENT_SELECT,
    BT_STATE_SELECT_OPPONENT,
    BT_STATE_DOWNLOAD_PROMPT,
    BT_STATE_REQUEST_DOWNLOAD,
    BT_STATE_WAIT_DOWNLOAD,
    BT_STATE_DOWNLOAD_COMPLETE_PROMPT,
    BT_STATE_ALREADY_CHALLENGED_PROMPT,
    BT_STATE_EXIT_PROMPT,
    BT_STATE_EXIT_YES_NO,
    BT_STATE_EXIT_MATCH_LIST_PROMPT,
    BT_STATE_EXIT_MATCH_LIST_YES_NO,
    BT_STATE_UPLOAD_PROMPT,
    BT_STATE_REQUEST_UPLOAD,
    BT_STATE_WAIT_UPLOAD,
    BT_STATE_UPLOAD_COMPLETE_PROMPT,
    BT_STATE_SELECT_RANK_PROMPT,
    BT_STATE_DRAW_RANK_SELECT,
    BT_STATE_SELECT_RANK,
    BT_STATE_MATCH_LIST_SELECT_PROMPT,
    BT_STATE_DRAW_MATCH_LIST_SELECT,
    BT_STATE_SELECT_MATCH_LIST,
    BT_STATE_DOWNLOAD_MATCH_LIST_PROMPT,
    BT_STATE_REQUEST_MATCH_LIST,
    BT_STATE_WAIT_MATCH_LIST,
    BT_STATE_MATCH_LIST_COMPLETE_PROMPT,
    BT_STATE_INIT_SAVE,
    BT_STATE_WAIT_SAVE,
    BT_STATE_GOODBYE_PROMPT,
    BT_STATE_CLEANUP_INET,
    BT_STATE_DISCONNECTING_PROMPT,
    BT_STATE_IDLE,
    BT_STATE_DISCONNECT_ENTRY,
    BT_STATE_END_SESSION,
    BT_STATE_WAIT_TEXT,
    BT_STATE_WAIT_TEXT_YES_NO,
    BT_STATE_WAIT_TEXT_TIMED,
    BT_STATE_DISCONNECT_YES_NO,
    BT_STATE_CONNECTED_PROMPT,
    BT_STATE_CONNECTED_YES_NO_MENU,
    BT_STATE_SHOW_ERROR,
    BT_STATE_ERROR_CLEANUP,
    BT_STATE_SHOW_DWC_ERROR,
    BT_STATE_DWC_ERROR_ACK,
};

typedef struct WifiBattleTowerAppState {
    UnkStruct_0206BC70 *args;
    BgConfig *bgConfig;
    int unk_08;
    int unk_0C;
    int screenMode;
    int exitMode;
    int unused_18;
    enum WifiBattleTowerState state;
    enum WifiBattleTowerState nextState;
    void *dwcHeapBuffer;
    NNSFndHeapHandle dwcHeap;
    DWCInetControl inetControl;
    int playerRank;
    int selectedRank;
    int matchCount;
    int selectedOpponentIdx;
    WifiBattleTowerDownloadBuffer downloadBuffer;
    WifiPlayerProfile wifiPlayerProfile;
    u16 unk_BB8;
    u16 unk_BBA;
    u16 unk_BBC;
    u16 unk_BBE;
    BoxPokemon *unk_BC0;
    int unk_BC4;
    int unk_BC8;
    MessageFormat *stringTemplate;
    MsgData *msgLoader1;
    MsgData *msgLoader2;
    MsgData *msgLoader3;
    String *msgString;
    String *titleString;
    String *opponentIdxString;
    String *rankString;
    String *errorString;
    int textPrinterID;
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *resourceCollection[4];
    SpriteResource *spriteResource[3][4];
    SpriteResourcesHeader arrowSpriteHeader;
    SpriteResourcesHeader unk_DE8;
    SpriteResourcesHeader unk_E0C;
    Sprite *selectionArrows[2];
    Window msgWindow;
    Window rankWindow;
    Window opponentWindow;
    Window titleWindow;
    Window popupWindow;
    Window unk_E88[3];
    Window unk_EB8[11];
    Menu *yesNoMenu;
    void *waitDial;
    u8 unk_F10[64];
    int dwcError;
    int dwcErrorDetail;
    int dwcErrorType;
    int asyncResult;
    int waitTimer;
    WorldExchangeTrainer weTrainer;
    WorldExchangeTrainerError weTrainerErr;
    s16 subStep;
    s16 subTimer;
    int timeoutCounter;
    UnkStruct_ov61_0222C3B0 unk_FF8;
} WifiBattleTowerAppState;

enum WifiBattleTowerLoopState {
    BT_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER,
    BT_LOOP_STATE_INIT,
    BT_LOOP_STATE_WAIT_FADE,
    BT_LOOP_STATE_MAIN,
    BT_LOOP_STATE_FINISH,
    BT_LOOP_STATE_EXIT,
};

void sub_020399EC(void);

void ov72_0223A444(WifiBattleTowerAppState *appState);

int ov72_022392BC(WifiBattleTowerAppState *appState);

int ov72_022392BC(WifiBattleTowerAppState *appState) {
    switch (DWC_NASLoginProcess()) {
    case DWC_NASLOGIN_STATE_SUCCESS:
        appState->state = BT_STATE_INIT_HTTP;
        break;
    case DWC_NASLOGIN_STATE_ERROR:
    case DWC_NASLOGIN_STATE_CANCELED:
    case DWC_NASLOGIN_STATE_DIRTY:
        ov72_0223A444(appState);
        {
            int errCode;
            DWCErrorType errType;
            DWCError err;

            err = DWC_GetLastErrorEx(&errCode, &errType);
            appState->dwcError = err;
            appState->dwcErrorDetail = errCode;

            DWC_ClearError();
            DWC_CleanupInet();

            appState->state = BT_STATE_SHOW_DWC_ERROR;

            switch (errType) {
            case DWC_ETYPE_LIGHT:
            case DWC_ETYPE_SHOW_ERROR:
                appState->state = BT_STATE_SHOW_DWC_ERROR;
                break;
            case DWC_ETYPE_SHUTDOWN_GHTTP:
                DWC_ShutdownGHTTP();
                appState->state = BT_STATE_SHOW_DWC_ERROR;
                break;
            case DWC_ETYPE_DISCONNECT:
                appState->state = BT_STATE_SHOW_DWC_ERROR;
                break;
            case DWC_ETYPE_SHUTDOWN_FM:
                DWC_ShutdownFriendsMatch();
                appState->state = BT_STATE_SHOW_DWC_ERROR;
                break;
            case DWC_ETYPE_SHUTDOWN_ND:
            case DWC_ETYPE_FATAL:
                sub_020399EC();
                break;
            }

            if ((errCode < -20000) && (errCode >= -29999)) {
                appState->state = BT_STATE_SHOW_DWC_ERROR;
            }
        }
        break;
    }

    return BT_LOOP_STATE_MAIN;
}
