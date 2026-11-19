#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_70_gtsapplication_state.h"
#include "unk_0202C034.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "evolution_data.h"
#include "list_menu.h"
#include "sav_chatot.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct BoxPokemon;

struct SOUND_CHATOT;

struct ColoredArrow;

struct DWCstApInfo;

struct DWCstInetControl;

struct EvolutionData;

struct G2dRenderer;

struct GTSApplicationState;

struct GTSBoxPokemonCriteria;

struct GTSCharpadScrollState;

struct GTSPlayerData;

struct GTSPokemonCriteria;

struct GTSPokemonListing;

struct GTSPokemonListingMon;

struct GTSPokemonRequirements;

struct GTSTradedPokemonLocation;

struct ListMenu;

struct ListMenuTemplate;

struct Menu;

struct MsgData;

struct NARC;

struct Options;

struct Pokemon;

struct PokemonSummary;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct TradeAnimationTemplate;

struct PlayerProfile;

struct WiFiList;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;


















void Heap_Free(void *ptr);

typedef u16 charcode_t;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);


typedef struct SysTaskManager SysTaskManager;


enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
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


typedef struct GTSPlayerData GTSPlayerData;





void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};

enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};


typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct Menu Menu;

enum GTSApplicationLoopState {
    GTS_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER = 0,
    GTS_LOOP_STATE_INIT,
    GTS_LOOP_STATE_WAIT_FADE,
    GTS_LOOP_STATE_MAIN,
    GTS_LOOP_STATE_FINISH,
    GTS_LOOP_STATE_EXIT
};







typedef struct GTSBoxPokemonCriteria GTSBoxPokemonCriteria;


enum GTSWFCInitScreenStates {
    GTSWFCINIT_ASK_TO_SETUP_CONNECTION,
    GTSWFCINIT_PROCESS_SETUP_CONFIRMATION,
    GTSWFCINIT_CONNECT,
    GTSWFCINIT_CHECK_CONNECTION,
    GTSWFCINIT_AUTHENTICATE,
    GTSWFCINIT_CHECK_AUTHENTICATION,
    GTSWFCINIT_GET_DWC_KEY,
    GTSWFCINIT_CONNECT_TO_REMOTE_SERVER,
    GTSWFCINIT_WAIT_FOR_SERVER_RESPONSE,
    GTSWFCINIT_SET_PROFILE_REQUEST,
    GTSWFCINIT_SET_PROFILE_RESPONSE,
    GTSWFCINIT_EXIT_SCREEN,
    GTSWFCINIT_WAIT_FOR_TEXT,
    GTSWFCINIT_WAIT_FOR_TEXT_THEN_YES_NO_MENU,
    GTSWFCINIT_HANDLE_RETRY_MENU_INPUT,
    GTSWFCINIT_ASK_TO_DISCONNECT,
    GTSWFCINIT_RESTART_OR_EXIT,
    GTSWFCINIT_SHOW_DISCONNECTING_MESSAGE,
    GTSWFCINIT_CLEANUP_NETWORKING,
    GTSWFCINIT_SHOW_DISCONNECTED_MESSAGE,
    GTSWFCINIT_WAIT_FOR_TEXT_DELAYED,
    GTSWFCINIT_FATAL_ERROR,
    GTSWFCINIT_FATAL_ERROR_DISCONNECT_MESSAGE,
    GTSWFCINIT_SHOW_NETWORK_ERROR,
    GTSWFCINIT_RESTART_CONNECTION
};

void ov70_02238F80(GTSApplicationState *appState);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

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

int ov70_022448C0(GTSApplicationState *appState);

int ov70_022448C0(GTSApplicationState *appState) {
    DWC_ProcessInet();

    if (DWC_CheckInet()) {
        switch (DWC_GetInetStatus()) {
        case DWC_CONNECTINET_STATE_ERROR: {
            DWCError dwcErr;
            int errCode;
            DWCErrorType errType;

            dwcErr = DWC_GetLastErrorEx(&errCode, &errType);

            appState->dwcErr = dwcErr;
            appState->dwcErrCode = errCode;
            appState->dwcErrType = errType;
        }
            DWC_ClearError();
            DWC_CleanupInet();
            ov70_02238F80(appState);

            appState->currentScreenInstruction = GTSWFCINIT_SHOW_NETWORK_ERROR;

            break;
        case DWC_CONNECTINET_STATE_NOT_INITIALIZED:
        case DWC_CONNECTINET_STATE_IDLE:
        case DWC_CONNECTINET_STATE_OPERATING:
        case DWC_CONNECTINET_STATE_OPERATED:
        case DWC_CONNECTINET_STATE_DISCONNECTING:
        case DWC_CONNECTINET_STATE_DISCONNECTED:
        default:
        case DWC_CONNECTINET_STATE_FATAL_ERROR:
            DWCError unusedErr;
            int unusedErrCode;

            unusedErr = DWC_GetLastError(&unusedErrCode);
            ov70_02238F80(appState);

            appState->currentScreenInstruction = GTSWFCINIT_FATAL_ERROR;
            appState->commsErrorMessage = -2;
            break;
        case DWC_CONNECTINET_STATE_CONNECTED:
            DWCApInfo apInfo;
            DWC_GetApInfo(&apInfo);

            if (apInfo.aptype == DWC_APINFO_TYPE_SHOP) {
                (void)0;
            }

            appState->currentScreenInstruction = GTSWFCINIT_AUTHENTICATE;
            break;
        }
    }

    return GTS_LOOP_STATE_MAIN;
}


int ov70_02244944(GTSApplicationState *appState);

int ov70_02244944(GTSApplicationState *appState) {
    DWC_NASLoginAsync();
    appState->currentScreenInstruction = GTSWFCINIT_CHECK_AUTHENTICATION;

    return GTS_LOOP_STATE_MAIN;
}





void sub_020399EC(void);

int ov70_02244954(GTSApplicationState *appState);

int ov70_02244954(GTSApplicationState *appState) {
    switch (DWC_NASLoginProcess()) {
    case DWC_NASLOGIN_STATE_SUCCESS:
        appState->currentScreenInstruction = GTSWFCINIT_GET_DWC_KEY;
        break;
    case DWC_NASLOGIN_STATE_ERROR:
    case DWC_NASLOGIN_STATE_CANCELED:
    case DWC_NASLOGIN_STATE_DIRTY:
        ov70_02238F80(appState);

        int errCode;
        DWCErrorType errType;
        DWCError dwcErr;

        dwcErr = DWC_GetLastErrorEx(&errCode, &errType);

        appState->dwcErr = dwcErr;
        appState->dwcErrCode = errCode;

        DWC_ClearError();
        DWC_CleanupInet();

        appState->currentScreenInstruction = GTSWFCINIT_SHOW_NETWORK_ERROR;

        switch (errType) {
        case DWC_ETYPE_LIGHT:
        case DWC_ETYPE_SHOW_ERROR:
            appState->currentScreenInstruction = GTSWFCINIT_SHOW_NETWORK_ERROR;
            break;
        case DWC_ETYPE_SHUTDOWN_GHTTP:
            DWC_ShutdownGHTTP();
            appState->currentScreenInstruction = GTSWFCINIT_SHOW_NETWORK_ERROR;
            break;
        case DWC_ETYPE_DISCONNECT:
            appState->currentScreenInstruction = GTSWFCINIT_SHOW_NETWORK_ERROR;
            break;
        case DWC_ETYPE_SHUTDOWN_FM:
            DWC_ShutdownFriendsMatch();
            appState->currentScreenInstruction = GTSWFCINIT_SHOW_NETWORK_ERROR;
            break;
        case DWC_ETYPE_SHUTDOWN_ND:
        case DWC_ETYPE_FATAL:
            sub_020399EC();
            break;
        }

        if ((errCode < -20000) && (errCode >= -29999)) {
            appState->currentScreenInstruction = GTSWFCINIT_SHOW_NETWORK_ERROR;
        }
        break;
    }

    return GTS_LOOP_STATE_MAIN;
}
