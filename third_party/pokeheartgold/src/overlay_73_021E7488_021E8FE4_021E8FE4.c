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
#include "overlay_80_frontier_opponent.h"
#include "unk_0202D230.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "message_format.h"
#include "msgdata.h"

struct BgConfig;

struct BoxPokemon;

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


typedef u16 charcode_t;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct Menu Menu;

enum WifiBattleTowerLoopState {
    BT_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER,
    BT_LOOP_STATE_INIT,
    BT_LOOP_STATE_WAIT_FADE,
    BT_LOOP_STATE_MAIN,
    BT_LOOP_STATE_FINISH,
    BT_LOOP_STATE_EXIT,
};

void ov73_021E83EC(WifiBattleTowerAppState *appState, enum NintendoWfcState state, enum NintendoWfcState nextState);

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

void ov73_021EA180(WifiBattleTowerAppState *appState);

int ov73_021E8FE4(WifiBattleTowerAppState *appState);

int ov73_021E8FE4(WifiBattleTowerAppState *appState) {
    if (ov72_02237B54()) {
        s32 result = ov72_02237B74();

        appState->timeoutCounter = 0;

        switch (result) {
        case BT_CODE_SUCCESS:
            ov73_021EA180(appState);

            switch (appState->weTrainerErr.validationError) {
            case 0:
                switch (appState->weTrainerErr.systemError) {
                case 0:
                    switch (appState->args->mode) {
                    case WIFI_BT_MODE_BROWSE:
                        ov73_021E83EC(appState, NINTENDO_WFC_STATE_REQUEST_ROOM_COUNT, NINTENDO_WFC_STATE_ASK_CHALLENGE_ROOM);
                        break;
                    case WIFI_BT_MODE_UPLOAD:
                        appState->state = NINTENDO_WFC_STATE_PRINT_SENDING_RECORD;
                        break;
                    case WIFI_BT_MODE_SELECT_RANK:
                        appState->state = NINTENDO_WFC_STATE_ASK_LEADERS_RANK;
                        break;
                    }
                    break;
                case 1:
                case 2:
                case 3:
                    appState->asyncResult = result;
                    appState->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
                    ov73_021EA180(appState);
                    break;
                }
                break;
            case 1:
            case 2:
                appState->asyncResult = result;
                appState->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
                ov73_021EA180(appState);
                break;
            default:
                ov73_021EA180(appState);
                sub_020399EC();
                break;
            }
            break;
        case BT_CODE_UPLOAD_NEEDED:
            appState->asyncResult = result;
            appState->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(appState);
            break;
        case BT_CODE_RANK_SELECT_NEEDED:
        case BT_CODE_UPLOAD_REJECTED:
            appState->asyncResult = result;
            appState->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(appState);
            break;
        case BT_CODE_CONNECTION_FAILED:
        case BT_CODE_HTTP_ERROR:
            appState->asyncResult = result;
            appState->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(appState);
            break;
        case BT_CODE_SERVER_MAINTENANCE:
        case BT_CODE_NOT_IN_DCF_PHASE:
            appState->asyncResult = result;
            appState->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(appState);
            break;
        case BT_CODE_NETWORK_FAILURE:
        default:
            ov73_021EA180(appState);
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
