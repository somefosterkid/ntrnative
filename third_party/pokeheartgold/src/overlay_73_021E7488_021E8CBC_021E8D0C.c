#include <dwc.h>

#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
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

extern void DWC_InitInetEx(DWCInetControl *inetCntl, u16 dmaNo, u16 powerMode, u16 ssl_priority);


extern void DWC_ConnectInetAsync(void);


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

int ov73_021E8D0C(WifiBattleTowerAppState *appState);

int ov73_021E8D0C(WifiBattleTowerAppState *appState) {
    DWC_InitInetEx(&appState->inetControl, 2, 1, 20);
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
    DWC_ConnectInetAsync();

    appState->state = NINTENDO_WFC_STATE_WAIT_CONNECT;

    return BT_LOOP_STATE_MAIN;
}

struct DWCstApInfo;

BOOL DWC_GetApInfo(DWCApInfo *apinfo);

extern BOOL DWC_CheckInet(void);

extern void DWC_ProcessInet(void);


extern void DWC_CleanupInet(void);


extern void DWC_ClearError(void);

void ov73_021EA180(WifiBattleTowerAppState *appState);

int ov73_021E8D2C(WifiBattleTowerAppState *appState);

int ov73_021E8D2C(WifiBattleTowerAppState *appState) {
    DWC_ProcessInet();

    if (DWC_CheckInet()) {
        int status = DWC_GetInetStatus();

        switch (status) {
        case DWC_CONNECTINET_STATE_ERROR: {
            DWCError err;
            int errDetail;

            err = DWC_GetLastError(&errDetail);

            appState->dwcError = err;
            appState->dwcErrorDetail = errDetail;
        }
            DWC_ClearError();
            DWC_CleanupInet();

            ov73_021EA180(appState);
            appState->state = NINTENDO_WFC_STATE_SHOW_DWC_ERROR;
            break;
        case DWC_CONNECTINET_STATE_NOT_INITIALIZED:
        case DWC_CONNECTINET_STATE_IDLE:
        case DWC_CONNECTINET_STATE_OPERATING:
        case DWC_CONNECTINET_STATE_OPERATED:
        case DWC_CONNECTINET_STATE_DISCONNECTING:
        case DWC_CONNECTINET_STATE_DISCONNECTED:
        default:
        case DWC_CONNECTINET_STATE_FATAL_ERROR: {
            DWCError err2;
            int errCode;

            err2 = DWC_GetLastError(&errCode);

            ov73_021EA180(appState);
            appState->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            appState->asyncResult = -2;
        } break;
        case DWC_CONNECTINET_STATE_CONNECTED: {
            DWCApInfo unused;

            DWC_GetApInfo(&unused);
        }
            appState->state = NINTENDO_WFC_STATE_START_AUTH;
            break;
        }
    }

    return BT_LOOP_STATE_MAIN;
}

BOOL DWC_NASLoginAsync(void);

int ov73_021E8DB8(WifiBattleTowerAppState *appState);

int ov73_021E8DB8(WifiBattleTowerAppState *appState) {
    DWC_NASLoginAsync();
    appState->state = NINTENDO_WFC_STATE_WAIT_AUTH;

    return BT_LOOP_STATE_MAIN;
}


DWCNasLoginState DWC_NASLoginProcess(void);

BOOL DWC_ShutdownGHTTP(void);

extern void DWC_ShutdownFriendsMatch(void);

void sub_020399EC(void);

int ov73_021E8DC8(WifiBattleTowerAppState *appState);

int ov73_021E8DC8(WifiBattleTowerAppState *appState) {
    switch (DWC_NASLoginProcess()) {
    case DWC_NASLOGIN_STATE_SUCCESS:
        appState->state = NINTENDO_WFC_STATE_INIT_HTTP;
        break;
    case DWC_NASLOGIN_STATE_ERROR:
    case DWC_NASLOGIN_STATE_CANCELED:
    case DWC_NASLOGIN_STATE_DIRTY:
        ov73_021EA180(appState);
        {
            int errCode;
            DWCErrorType errType;
            DWCError err;

            err = DWC_GetLastErrorEx(&errCode, &errType);
            appState->dwcError = err;
            appState->dwcErrorDetail = errCode;

            DWC_ClearError();
            DWC_CleanupInet();

            appState->state = NINTENDO_WFC_STATE_SHOW_DWC_ERROR;

            switch (errType) {
            case DWC_ETYPE_LIGHT:
            case DWC_ETYPE_SHOW_ERROR:
                appState->state = NINTENDO_WFC_STATE_SHOW_DWC_ERROR;
                break;
            case DWC_ETYPE_SHUTDOWN_GHTTP:
                DWC_ShutdownGHTTP();
                appState->state = NINTENDO_WFC_STATE_SHOW_DWC_ERROR;
                break;
            case DWC_ETYPE_DISCONNECT:
                appState->state = NINTENDO_WFC_STATE_SHOW_DWC_ERROR;
                break;
            case DWC_ETYPE_SHUTDOWN_FM:
                DWC_ShutdownFriendsMatch();
                appState->state = NINTENDO_WFC_STATE_SHOW_DWC_ERROR;
                break;
            case DWC_ETYPE_SHUTDOWN_ND:
            case DWC_ETYPE_FATAL:
                sub_020399EC();
                break;
            }

            if ((errCode < -20000) && (errCode >= -29999)) {
                appState->state = NINTENDO_WFC_STATE_SHOW_DWC_ERROR;
            }
        }
        break;
    }

    return BT_LOOP_STATE_MAIN;
}




void ov72_022378C0(s32 profileId, u64 friendKey);

int ov73_021E8E7C(WifiBattleTowerAppState *appState);

int ov73_021E8E7C(WifiBattleTowerAppState *appState) {
    s32 profileId = Save_SysInfo_GetDwcProfileId(appState->args->sysInfo);

    if (profileId == 0) {
        Save_SysInfo_SetDwcProfileId(appState->args->sysInfo, appState->args->profileId);
    }

    profileId = Save_SysInfo_GetDwcProfileId(appState->args->sysInfo);
    ov72_022378C0(profileId, DWC_CreateFriendKey(appState->args->userData));

    appState->state = NINTENDO_WFC_STATE_REQUEST_INFO;

    return BT_LOOP_STATE_MAIN;
}
