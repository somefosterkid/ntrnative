#include "global.h"
#include "sprite_resources_header.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_80_frontier_opponent.h"
#include "overlay_70_gtsapplication_state.h"
#include "message_format.h"
#include "unk_0202D230.h"
#include "sav_system_info.h"
#include "msgdata.h"
#include "save_frontier.h"

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

enum WifiBattleTowerMode {
    WIFI_BT_MODE_BROWSE,
    WIFI_BT_MODE_UPLOAD,
    WIFI_BT_MODE_SELECT_RANK,
};

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
    int dwcError;
    int dwcErrorDetail;
    int dwcErrorType;
    u8 pad_F1C[64];
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

void ov72_02238680(WifiBattleTowerAppState *appState, enum WifiBattleTowerState state, enum WifiBattleTowerState nextState);

BOOL ov72_02237B54(void);

s32 ov72_02237B74(void);

enum BattleTowerHttpResult {
    BT_CODE_SUCCESS,
    BT_CODE_UPLOAD_NEEDED,
    BT_CODE_RANK_SELECT_NEEDED,

    BT_CODE_UPLOAD_REJECTED = -1,
    BT_CODE_SERVER_MAINTENANCE = -2,
    BT_CODE_ALREADY_CHALLENGED = -3,
    BT_CODE_HTTP_ERROR = -4,
    BT_CODE_NETWORK_FAILURE = -5,
    BT_CODE_NOT_IN_DCF_PHASE = -6,
    BT_CODE_CONNECTION_FAILED = -7,
};

void sub_020399EC(void);

void ov72_0223A444(WifiBattleTowerAppState *appState);

int ov72_022394D8(WifiBattleTowerAppState *appState);

int ov72_022394D8(WifiBattleTowerAppState *appState) {
    if (ov72_02237B54()) {
        s32 result = ov72_02237B74();

        appState->timeoutCounter = 0;

        switch (result) {
        case BT_CODE_SUCCESS:
            ov72_0223A444(appState);

            switch (appState->weTrainerErr.validationError) {
            case 0:
                switch (appState->weTrainerErr.systemError) {
                case 0:
                    switch (appState->args->mode) {
                    case WIFI_BT_MODE_BROWSE:
                        ov72_02238680(appState, BT_STATE_REQUEST_ROOM_COUNT, BT_STATE_SELECT_OPPONENT_PROMPT);
                        break;
                    case WIFI_BT_MODE_UPLOAD:
                        appState->state = BT_STATE_UPLOAD_PROMPT;
                        break;
                    case WIFI_BT_MODE_SELECT_RANK:
                        appState->state = BT_STATE_SELECT_RANK_PROMPT;
                        break;
                    }
                    break;
                case 1:
                case 2:
                case 3:
                    appState->asyncResult = result;
                    appState->state = BT_STATE_SHOW_ERROR;
                    ov72_0223A444(appState);
                    break;
                }
                break;
            case 1:
            case 2:
                appState->asyncResult = result;
                appState->state = BT_STATE_SHOW_ERROR;
                ov72_0223A444(appState);
                break;
            default:
                ov72_0223A444(appState);
                sub_020399EC();
                break;
            }
            break;
        case BT_CODE_UPLOAD_NEEDED:
            appState->asyncResult = result;
            appState->state = BT_STATE_SHOW_ERROR;
            ov72_0223A444(appState);
            break;
        case BT_CODE_RANK_SELECT_NEEDED:
        case BT_CODE_UPLOAD_REJECTED:
            appState->asyncResult = result;
            appState->state = BT_STATE_SHOW_ERROR;
            ov72_0223A444(appState);
            break;
        case BT_CODE_CONNECTION_FAILED:
        case BT_CODE_HTTP_ERROR:
            appState->asyncResult = result;
            appState->state = BT_STATE_SHOW_ERROR;
            ov72_0223A444(appState);
            break;
        case BT_CODE_SERVER_MAINTENANCE:
        case BT_CODE_NOT_IN_DCF_PHASE:
            appState->asyncResult = result;
            appState->state = BT_STATE_SHOW_ERROR;
            ov72_0223A444(appState);
            break;
        case BT_CODE_NETWORK_FAILURE:
        default:
            ov72_0223A444(appState);
            sub_020399EC();
            break;
        }
    } else {
        appState->timeoutCounter++;

        if (appState->timeoutCounter == (30 * 60 * 2)) {
            sub_020399EC();
        }
    }

    return BT_LOOP_STATE_MAIN;
}
